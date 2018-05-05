#include <io/include/type_descriptor.hpp>
#include <io/include/serializer.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

TypeDescriptor_Struct::TypeDescriptor_Struct(const char* name, size_t size, void(*initialize)()) : TypeDescriptor(name, size)
{
	initialize();
}

void TypeDescriptor_Struct::init(Member* members, int num_members)
{
	m_members = members;
	m_num_members = num_members;
}

void TypeDescriptor_Struct::serialize(void* obj, const char* name, ISerializer* serializer)
{
	if (is_trivial() && serializer->is_raw_serializable())
		serializer->raw_serialize(&obj, m_size);
	else
	{
		char* ptr = static_cast<char*>(obj);

		serializer->begin_serialize_struct(name);

		for (int i = 0; i < m_num_members; i++)
			m_members[i].m_type->serialize(ptr + m_members[i].m_offset, m_members[i].m_name, serializer);

		serializer->end_serialize_struct(name);
	}
}

void TypeDescriptor_Struct::deserialize(void* obj, const char* name, ISerializer* serializer)
{
	if (is_trivial() && serializer->is_raw_serializable())
		serializer->raw_deserialize(&obj, m_size);
	else
	{
		char* ptr = static_cast<char*>(obj);

		serializer->begin_deserialize_struct(name);

		for (int i = 0; i < m_num_members; i++)
			m_members[i].m_type->deserialize(ptr + m_members[i].m_offset, m_members[i].m_name, serializer);

		serializer->end_deserialize_struct(name);
	}
}

bool TypeDescriptor_Struct::is_trivial()
{
	for (int i = 0; i < m_num_members; i++)
	{
		if (!m_members[i].m_type->is_trivial())
			return false;
	}

	return true;
}

TypeDescriptor_Enum::TypeDescriptor_Enum(const char* name, size_t size) : TypeDescriptor(name, size) {}

int TypeDescriptor_Enum::current_value_index(int value)
{
	for (int i = 0; i < m_num_constants; i++)
	{
		if (m_constants[i].m_value == value)
			return i;
	}

	return 0;
}

void TypeDescriptor_Enum::serialize(void* obj, const char* name, ISerializer* serializer)
{
	int32_t& value = *static_cast<int32_t*>(obj);
	serializer->serialize(name, value);
}

void TypeDescriptor_Enum::deserialize(void* obj, const char* name, ISerializer* serializer)
{
	int32_t& value = *static_cast<int32_t*>(obj);
	serializer->deserialize(name, value);
}

bool TypeDescriptor_Enum::is_trivial()
{
	return true;
}

struct TypeDescriptor_Int : TypeDescriptor
{
    TypeDescriptor_Int() : TypeDescriptor{"int32_t", sizeof(int32_t)} {}

	void serialize(void* obj, const char* name, ISerializer* serializer) override
	{
		int32_t& value = *static_cast<int32_t*>(obj);
		serializer->serialize(name, value);
	}

	void deserialize(void* obj, const char* name, ISerializer* serializer) override
	{
		int32_t& value = *static_cast<int32_t*>(obj);
		serializer->deserialize(name, value);
	}

	bool is_trivial()
	{
		return true;
	}
};

template <>
TypeDescriptor* get_primitive_descriptor<int>()
{
    static TypeDescriptor_Int typeDesc;
    return &typeDesc;
}

struct TypeDescriptor_Double : TypeDescriptor
{
    TypeDescriptor_Double() : TypeDescriptor{"double", sizeof(double)} {}

	void serialize(void* obj, const char* name, ISerializer* serializer) override
	{
		double& value = *static_cast<double*>(obj);
		serializer->serialize(name, value);
	}

	void deserialize(void* obj, const char* name, ISerializer* serializer) override
	{
		double& value = *static_cast<double*>(obj);
		serializer->serialize(name, value);
	}

	bool is_trivial()
	{
		return true;
	}
};

template <>
TypeDescriptor* get_primitive_descriptor<double>()
{
    static TypeDescriptor_Double typeDesc;
    return &typeDesc;
}

struct TypeDescriptor_Bool : TypeDescriptor
{
    TypeDescriptor_Bool() : TypeDescriptor{"bool", sizeof(bool)} {}

	void serialize(void* obj, const char* name, ISerializer* serializer) override
	{
		bool& value = *static_cast<bool*>(obj);
		serializer->serialize(name, value);
	}

	void deserialize(void* obj, const char* name, ISerializer* serializer) override
	{
		bool& value = *static_cast<bool*>(obj);
		serializer->serialize(name, value);
	}

	bool is_trivial()
	{
		return true;
	}
};

template <>
TypeDescriptor* get_primitive_descriptor<bool>()
{
    static TypeDescriptor_Bool typeDesc;
    return &typeDesc;
}

struct TypeDescriptor_Float : TypeDescriptor
{
    TypeDescriptor_Float() : TypeDescriptor{"float", sizeof(float)} {}

	void serialize(void* obj, const char* name, ISerializer* serializer) override
	{
		float& value = *static_cast<float*>(obj);
		serializer->serialize(name, value);
	}

	void deserialize(void* obj, const char* name, ISerializer* serializer) override
	{
		float& value = *static_cast<float*>(obj);
		serializer->deserialize(name, value);
	}

	bool is_trivial()
	{
		return true;
	}
};

template <>
TypeDescriptor* get_primitive_descriptor<float>()
{
    static TypeDescriptor_Float typeDesc;
    return &typeDesc;
}

void TypeDescriptor_Vector::serialize(void* obj, const char* name, ISerializer* serializer)
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

void TypeDescriptor_Vector::deserialize(void* obj, const char* name, ISerializer* serializer)
{
	if (m_object_desc->is_trivial() && !m_pointer && serializer->is_raw_serializable())
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

bool TypeDescriptor_Vector::is_trivial()
{
	return false;
}

TE_END_TERMINUS_NAMESPACE