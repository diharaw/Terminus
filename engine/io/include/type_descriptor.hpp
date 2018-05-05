#pragma once

#include <core/include/terminus_macros.hpp>
#include <stl/include/vector.hpp>
#include <stl/include/array.hpp>

#include <stdio.h>
#include <iostream>

TE_BEGIN_TERMINUS_NAMESPACE

class ISerializer;

struct TypeDescriptor
{
    const char* m_name;
    size_t		m_size;
    
    TypeDescriptor(const char* name, size_t size) : m_name(name), m_size(size) {}
	virtual void serialize(void* obj, const char* name, ISerializer* serializer) = 0;
	virtual void deserialize(void* obj, const char* name, ISerializer* serializer) = 0;
	virtual bool is_trivial() = 0;
};

template <typename T>
TypeDescriptor* get_primitive_descriptor();

struct DefaultResolver 
{
	template <typename T> static char func(decltype(&T::Reflection));
	template <typename T> static int func(...);
	template <typename T>
	struct IsReflected 
	{
		enum { value = (sizeof(func<T>(nullptr)) == sizeof(char)) };
	};

	// This version is called if T has a static member named "Reflection":
	template <typename T, typename std::enable_if<IsReflected<T>::value, int>::type = 0>
	static TypeDescriptor* get() 
	{
		return &T::Reflection;
	}

	// This version is called otherwise:
	template <typename T, typename std::enable_if<!IsReflected<T>::value, int>::type = 0>
	static TypeDescriptor* get() 
	{
		return get_primitive_descriptor<T>();
	}
};

template <typename T>
struct TypeResolver 
{
	static TypeDescriptor* get() 
	{
		return DefaultResolver::get<T>();
	}
};

// Specializations

struct TypeDescriptor_Struct : public TypeDescriptor
{
    struct Member
    {
        const char*     m_name;
        size_t          m_offset;
        TypeDescriptor* m_type;
        bool            m_pointer;
        
        Member(const char* name, size_t offset, TypeDescriptor* type, bool pointer) : m_name(name), m_offset(offset), m_type(type), m_pointer(pointer) {}
    };
    
    int m_num_members;
    Member* m_members;
    
    TypeDescriptor_Struct(const char* name, size_t size, void(*initialize)());
    void init(Member* members, int num_members);
	void serialize(void* obj, const char* name, ISerializer* serializer) override;
	void deserialize(void* obj, const char* name, ISerializer* serializer) override;
	bool is_trivial() override;
};

struct TypeDescriptor_Enum : public TypeDescriptor
{
    struct Constant
    {
        const char* m_name;
        const char* m_desc;
        int         m_value;
        
        Constant(const char* name, const char* desc, int value) : m_name(name), m_desc(desc), m_value(value) {}
    };
    
    TypeDescriptor_Enum(const char* name, size_t size);
    int current_value_index(int value);
	void serialize(void* obj, const char* name, ISerializer* serializer) override;
	void deserialize(void* obj, const char* name, ISerializer* serializer) override;
	bool is_trivial() override;

    int       m_num_constants;
    Constant* m_constants;
};

struct TypeDescriptor_Container : public TypeDescriptor
{
	TypeDescriptor_Container(const char* name, size_t size) : TypeDescriptor(name, size) {}

	TypeDescriptor* m_object_desc;
	bool m_pointer;
	size_t(*get_size)(void*);
	void* (*get_item)(void*, size_t);
};

struct TypeDescriptor_Vector : public TypeDescriptor_Container
{
	template <typename T>
	TypeDescriptor_Vector(T*) : TypeDescriptor_Container{ "Vector", sizeof(Vector<T>) }
	{
		m_object_desc = TypeResolver<StripPointer<T>::Type>::get();

		get_size = [](void* obj) -> size_t {
			return ((Vector<T>*)obj)->size();
		};
		get_item = [](void* obj, size_t idx) -> void* {
			return (void*)&(*((Vector<T>*)obj))[idx];
		};
	}

	void serialize(void* obj, const char* name, ISerializer* serializer) override;
	void deserialize(void* obj, const char* name, ISerializer* serializer) override;
	bool is_trivial();
};

template <typename T>
class TypeResolver<Vector<T>>
{
public:
	static TypeDescriptor* get()
	{
		static TypeDescriptor_Vector typeDesc{ (T*)nullptr };
		return &typeDesc;
	}
};

template <typename T, size_t N>
struct TypeDescriptor_Array : TypeDescriptor_Container
{
	TypeDescriptor_Array(T* obj) : TypeDescriptor_Container{ "Array", sizeof(T) * N }
	{
		m_object_desc = TypeResolver<StripPointer<T>::Type>::get();

		get_size = [](void* obj) -> size_t {
			return N;
		};
		get_item = [](void* obj, size_t idx) -> void* {
			return (void*)&(*((Array<T, N>*)obj))[idx];
		};
	}

	void serialize(void* obj, const char* name, ISerializer* serializer)
	{
		if (m_object_desc->is_trivial() && !m_pointer && serializer->is_raw_serializable())
			serializer->raw_serialize(get_item(obj, 0), m_object_desc->m_size * get_size(obj));
		else
		{
			size_t n = get_size(obj);

			serializer->begin_serialize_array(name, n);

			for (int i = 0; i < n; i++)
				m_object_desc->serialize(get_item(obj, i), name, serializer);

			serializer->end_serialize_array(name);
		}
	}

	void deserialize(void* obj, const char* name, ISerializer* serializer)
	{
		if (is_trivial() && serializer->is_raw_serializable())
			serializer->raw_deserialize(get_item(obj, 0), m_object_desc->m_size * get_size(obj));
		else
		{
			int n = serializer->begin_deserialize_array(name);

			for (int i = 0; i < n; i++)
			{
				serializer->push_array_index(i);
				m_object_desc->deserialize(get_item(obj, i), name, serializer);
				serializer->pop_array_index();
			}

			serializer->end_deserialize_array(name);
		}
	}

	bool is_trivial()
	{
		return m_object_desc->is_trivial() && !m_pointer;
	}
};

template <typename T, size_t N>
class TypeResolver<Array<T, N>>
{
public:
	static TypeDescriptor* get()
	{
		static TypeDescriptor_Array<T, N> typeDesc{ (T*)nullptr };
		return &typeDesc;
	}
};

TE_END_TERMINUS_NAMESPACE