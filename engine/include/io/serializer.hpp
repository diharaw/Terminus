#pragma once

#include <stdint.h>
#include <string>
#include <io/reflection.hpp>
#include <io/stream.hpp>

#include <stack>

TE_BEGIN_TERMINUS_NAMESPACE

class ISerializer
{
public:
	ISerializer(IStream& stream) : m_stream(stream) {}
	virtual ~ISerializer() {}
	virtual void serialize(const char* name, bool& value)								  = 0;
	virtual void serialize(const char* name, int8_t& value)								  = 0;
	virtual void serialize(const char* name, uint8_t& value)							  = 0;
	virtual void serialize(const char* name, int16_t& value)							  = 0;
	virtual void serialize(const char* name, uint16_t& value)							  = 0;
	virtual void serialize(const char* name, int32_t& value)							  = 0;
	virtual void serialize(const char* name, uint32_t& value)							  = 0;
	virtual void serialize(const char* name, float& value)								  = 0;
	virtual void serialize(const char* name, double& value)								  = 0;
	virtual void serialize(const char* name, std::string& value)						  = 0;
	virtual void serialize(const char* name, const char* value)							  = 0;
																						  
	virtual void begin_serialize_struct(const char* name)								  = 0;
	virtual void end_serialize_struct(const char* name)									  = 0;
	virtual void begin_serialize_array(const char* name, int count)						  = 0;
	virtual void end_serialize_array(const char* name)									  = 0;
																						  
	virtual void deserialize(const char* name, bool& value)								  = 0;
	virtual void deserialize(const char* name, int8_t& value)							  = 0;
	virtual void deserialize(const char* name, uint8_t& value)							  = 0;
	virtual void deserialize(const char* name, int16_t& value)							  = 0;
	virtual void deserialize(const char* name, uint16_t& value)							  = 0;
	virtual void deserialize(const char* name, int32_t& value)							  = 0;
	virtual void deserialize(const char* name, uint32_t& value)							  = 0;
	virtual void deserialize(const char* name, float& value)							  = 0;
	virtual void deserialize(const char* name, double& value)							  = 0;
	virtual void deserialize(const char* name, std::string& value)						  = 0;
	virtual void deserialize(const char* name, char** value, bool is_static = true)		  = 0;

	virtual void begin_deserialize_struct(const char* name)								  = 0;
	virtual void end_deserialize_struct(const char* name)								  = 0;
	virtual int  begin_deserialize_array(const char* name)								  = 0;
	virtual void end_deserialize_array(const char* name)								  = 0;

	virtual bool is_raw_serializable()													  = 0;

	virtual void raw_serialize(void* data, const size_t& size)							  = 0;
	virtual void raw_deserialize(void* data, const size_t& size)						  = 0;

	virtual void flush_to_stream()														  = 0;

	inline void push_array_index(int idx) { m_index_stack.push(idx); }
	inline void pop_array_index()		  { m_index_stack.pop();	 }

	// ----------------------------------------------------------
	// Reflection-based API
	// ----------------------------------------------------------

	inline void serialize(void* obj, TypeDescriptor_Struct* desc, bool trivial)
	{
		if (trivial && is_raw_serializable())
			raw_serialize(&obj, desc->m_size);
		else
		{
			char* ptr = static_cast<char*>(obj);

			for (int i = 0; i < desc->m_num_members; i++)
				desc->m_members[i].m_type->serialize(ptr + desc->m_members[i].m_offset, desc->m_members[i].m_name, this);
		}
	}

	template <typename T>
	void save(T& obj)
	{
		serialize(&obj, &T::Reflection, false);
	}

	template <typename T>
	void save(const T& obj)
	{
		serialize((void*)&obj, &T::Reflection, false);
	}

	inline void deserialize(void* obj, TypeDescriptor_Struct* desc, bool trivial)
	{
		if (trivial && is_raw_serializable())
			raw_deserialize(&obj, desc->m_size);
		else
		{
			char* ptr = static_cast<char*>(obj);

			for (int i = 0; i < desc->m_num_members; i++)
				desc->m_members[i].m_type->deserialize(ptr + desc->m_members[i].m_offset, desc->m_members[i].m_name, this);
		}
	}

	template <typename T>
	void load(T& obj)
	{
		deserialize(&obj, &T::Reflection, false);
	}

protected:
	std::stack<int> m_index_stack;
	IStream&		m_stream;
};

TE_END_TERMINUS_NAMESPACE