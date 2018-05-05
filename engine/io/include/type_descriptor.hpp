#pragma once

#include <core/include/terminus_macros.hpp>
#include <stl/include/vector.hpp>
#include <stl/include/array.hpp>
#include <stl/include/static_hash_map.hpp>

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
	void(*resize)(void*, size_t);

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

		resize = [](void* obj, size_t size) -> void {
			((Vector<T>*)obj)->resize(size);
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
	void(*resize)(void*, size_t);

	TypeDescriptor_Array() : TypeDescriptor_Container{ "Array", sizeof(Array<T, N>) }
	{
		m_object_desc = TypeResolver<StripPointer<T>::Type>::get();

		get_size = [](void* obj) -> size_t {
			Array<T, N>* array = (Array<T, N>*)obj;
			return array->size();
		};
		get_item = [](void* obj, size_t idx) -> void* {
			return (void*)&(*((Array<T, N>*)obj))[idx];
		};

		resize = [](void* obj, size_t size) -> void {
			((Array<T, N>*)obj)->resize(size);
		};
	}

	void serialize(void* obj, const char* name, ISerializer* serializer)
	{
		if (m_object_desc->is_trivial() && !m_pointer && serializer->is_raw_serializable())
		{
			serializer->begin_serialize_array(nullptr, m_size);
			serializer->raw_serialize(obj, m_size);
		}
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
		{
			uint32_t size = serializer->begin_deserialize_array(nullptr);
			serializer->raw_deserialize(obj, size);
		}
		else
		{
			int n = serializer->begin_deserialize_array(name);
			resize(obj, n);

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
		static TypeDescriptor_Array<T, N> typeDesc;
		return &typeDesc;
	}
};

template <typename KEY_TYPE, typename VALUE_TYPE, size_t N>
struct TypeDescriptor_StaticHashMap: TypeDescriptor_Container
{
	TypeDescriptor* m_key_desc;
	void* (*get_key)(void*, size_t);
	void (*deserialize_pair)(void*, TypeDescriptor*, TypeDescriptor*, ISerializer*);

	TypeDescriptor_StaticHashMap() : TypeDescriptor_Container{ "StaticHashMap", sizeof(StaticHashMap<KEY_TYPE, VALUE_TYPE, N>) }
	{
		m_object_desc = TypeResolver<StripPointer<VALUE_TYPE>::Type>::get();
		m_key_desc = TypeResolver<StripPointer<KEY_TYPE>::Type>::get();

		get_size = [](void* obj) -> size_t {
			StaticHashMap<KEY_TYPE, VALUE_TYPE, N>* hash_map = (StaticHashMap<KEY_TYPE, VALUE_TYPE, N>*)obj;
			return hash_map->size();
		};

		get_item = [](void* obj, size_t idx) -> void* {
			StaticHashMap<KEY_TYPE, VALUE_TYPE, N>* hash_map = (StaticHashMap<KEY_TYPE, VALUE_TYPE, N>*)obj;
			return &hash_map->m_value[idx];
		};

		get_key = [](void* obj, size_t idx) -> void* {
			StaticHashMap<KEY_TYPE, VALUE_TYPE, N>* hash_map = (StaticHashMap<KEY_TYPE, VALUE_TYPE, N>*)obj;
			return &hash_map->m_key[idx];
		};

		deserialize_pair = [](void* obj, TypeDescriptor* key_desc, TypeDescriptor* value_desc, ISerializer* serializer) -> void {
			StaticHashMap<KEY_TYPE, VALUE_TYPE, N>* hash_map = (StaticHashMap<KEY_TYPE, VALUE_TYPE, N>*)obj;
			
			KEY_TYPE key;
			VALUE_TYPE val;

			key_desc->deserialize(&key, "key", serializer);
			value_desc->deserialize(&val, "value", serializer);

			hash_map->set(key, val);
		};
	}

	void serialize(void* obj, const char* name, ISerializer* serializer)
	{
		if (is_trivial() && serializer->is_raw_serializable())
		{
			serializer->begin_serialize_array(nullptr, m_size);
			serializer->raw_serialize(obj, m_size);
		}
		else
		{
			const size_t n = get_size(obj);

			serializer->begin_serialize_array(name, n);

			for (int i = 0; i < n; i++)
			{
				serializer->begin_serialize_struct(nullptr);
				m_key_desc->serialize(get_key(obj, i), "key", serializer);
				m_object_desc->serialize(get_item(obj, i), "value", serializer);
				serializer->end_serialize_struct(nullptr);
			}

			serializer->end_serialize_array(name);
		}
	}

	void deserialize(void* obj, const char* name, ISerializer* serializer)
	{
		if (is_trivial() && serializer->is_raw_serializable())
		{
			uint32_t size = serializer->begin_deserialize_array(nullptr);
			serializer->raw_deserialize(obj, size);
		}
		else
		{
			int n = serializer->begin_deserialize_array(name);

			for (int i = 0; i < n; i++)
			{
				serializer->begin_deserialize_struct(nullptr);
				deserialize_pair(obj, m_key_desc, m_object_desc, serializer);
				serializer->end_deserialize_struct(nullptr)
			}

			serializer->end_deserialize_array(name);
		}
	}

	bool is_trivial()
	{
		return m_object_desc->is_trivial() && m_key_desc->is_trivial() && !m_pointer;
	}
};

template <typename KEY_TYPE, typename VALUE_TYPE, size_t N>
class TypeResolver<StaticHashMap<KEY_TYPE, VALUE_TYPE, N>>
{
public:
	static TypeDescriptor* get()
	{
		static TypeDescriptor_StaticHashMap<KEY_TYPE, VALUE_TYPE, N> typeDesc;
		return &typeDesc;
	}
};

TE_END_TERMINUS_NAMESPACE