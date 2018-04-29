#include <io/include/json_serializer.hpp>
#include <iostream>

TE_BEGIN_TERMINUS_NAMESPACE

JsonSerializer::JsonSerializer()
{
	m_object_stack.push(nlohmann::json());
}

JsonSerializer::~JsonSerializer()
{

}

void JsonSerializer::serialize(const char* name, bool& value)
{
	if (m_object_stack.top().is_array())
		m_object_stack.top().push_back(value);
	else
		m_object_stack.top()[name] = value;
}

void JsonSerializer::serialize(const char* name, int8_t& value)
{
	if (m_object_stack.top().is_array())
		m_object_stack.top().push_back(value);
	else
		m_object_stack.top()[name] = value;
}

void JsonSerializer::serialize(const char* name, uint8_t& value)
{
	if (m_object_stack.top().is_array())
		m_object_stack.top().push_back(value);
	else
		m_object_stack.top()[name] = value;
}

void JsonSerializer::serialize(const char* name, int16_t& value)
{
	if (m_object_stack.top().is_array())
		m_object_stack.top().push_back(value);
	else
		m_object_stack.top()[name] = value;
}

void JsonSerializer::serialize(const char* name, uint16_t& value)
{
	if (m_object_stack.top().is_array())
		m_object_stack.top().push_back(value);
	else
		m_object_stack.top()[name] = value;
}

void JsonSerializer::serialize(const char* name, int32_t& value)
{
	if (m_object_stack.top().is_array())
		m_object_stack.top().push_back(value);
	else
		m_object_stack.top()[name] = value;
}

void JsonSerializer::serialize(const char* name, uint32_t& value)
{
	if (m_object_stack.top().is_array())
		m_object_stack.top().push_back(value);
	else
		m_object_stack.top()[name] = value;
}

void JsonSerializer::serialize(const char* name, float& value)
{
	if (m_object_stack.top().is_array())
		m_object_stack.top().push_back(value);
	else
		m_object_stack.top()[name] = value;
}

void JsonSerializer::serialize(const char* name, double& value)
{
	if (m_object_stack.top().is_array())
		m_object_stack.top().push_back(value);
	else
		m_object_stack.top()[name] = value;
}

void JsonSerializer::serialize(const char* name, std::string& value)
{
	if (m_object_stack.top().is_array())
		m_object_stack.top().push_back(value);
	else
		m_object_stack.top()[name] = value;
}

void JsonSerializer::serialize(const char* name, const char* value)
{
	if (m_object_stack.top().is_array())
		m_object_stack.top().push_back(value);
	else
		m_object_stack.top()[name] = value;
}

bool JsonSerializer::is_raw_serializable()
{
	return false;
}

void JsonSerializer::raw_serialize(void* data, const size_t& size)
{
	// NOT ALLOWED.
	assert(false);
}

void JsonSerializer::begin_serialize_struct(const char* name)
{
	m_object_stack.push(nlohmann::json());
}

void JsonSerializer::end_serialize_struct(const char* name)
{
	nlohmann::json top = m_object_stack.top();
	m_object_stack.pop();

	if (m_object_stack.top().is_array())
		m_object_stack.top().push_back(top);
	else
		m_object_stack.top()[name] = top;
}

void JsonSerializer::begin_serialize_array(const char* name, int count)
{
	nlohmann::json array_container;
	array_container["size"] = count;
	m_object_stack.push(array_container);
	m_object_stack.push(nlohmann::json::array());
}

void JsonSerializer::end_serialize_array(const char* name)
{
	nlohmann::json array = m_object_stack.top();
	m_object_stack.pop();
	nlohmann::json array_container = m_object_stack.top();
	m_object_stack.pop();

	array_container["data"] = array;

	m_object_stack.top()[name] = array_container;
}

void JsonSerializer::print()
{
	std::cout << m_object_stack.top().dump(4) << std::endl;
}

void JsonSerializer::deserialize(const char* name, bool& value)
{
	if (m_object_stack.top().is_array() && m_index_stack.size() > 0)
		value = m_object_stack.top()[m_index_stack.top()];
	else
		value = m_object_stack.top()[name];
}

void JsonSerializer::deserialize(const char* name, int8_t& value)
{
	if (m_object_stack.top().is_array() && m_index_stack.size() > 0)
		value = m_object_stack.top()[m_index_stack.top()];
	else
		value = m_object_stack.top()[name];
}

void JsonSerializer::deserialize(const char* name, uint8_t& value)
{
	if (m_object_stack.top().is_array() && m_index_stack.size() > 0)
		value = m_object_stack.top()[m_index_stack.top()];
	else
		value = m_object_stack.top()[name];
}

void JsonSerializer::deserialize(const char* name, int16_t& value)
{
	if (m_object_stack.top().is_array() && m_index_stack.size() > 0)
		value = m_object_stack.top()[m_index_stack.top()];
	else
		value = m_object_stack.top()[name];
}

void JsonSerializer::deserialize(const char* name, uint16_t& value)
{
	if (m_object_stack.top().is_array() && m_index_stack.size() > 0)
		value = m_object_stack.top()[m_index_stack.top()];
	else
		value = m_object_stack.top()[name];
}

void JsonSerializer::deserialize(const char* name, int32_t& value)
{
	if (m_object_stack.top().is_array() && m_index_stack.size() > 0)
		value = m_object_stack.top()[m_index_stack.top()];
	else
		value = m_object_stack.top()[name];
}

void JsonSerializer::deserialize(const char* name, uint32_t& value)
{
	if (m_object_stack.top().is_array() && m_index_stack.size() > 0)
		value = m_object_stack.top()[m_index_stack.top()];
	else
		value = m_object_stack.top()[name];
}

void JsonSerializer::deserialize(const char* name, float& value)
{
	if (m_object_stack.top().is_array() && m_index_stack.size() > 0)
		value = m_object_stack.top()[m_index_stack.top()];
	else
		value = m_object_stack.top()[name];
}

void JsonSerializer::deserialize(const char* name, double& value)
{
	if (m_object_stack.top().is_array() && m_index_stack.size() > 0)
		value = m_object_stack.top()[m_index_stack.top()];
	else
		value = m_object_stack.top()[name];
}

void JsonSerializer::deserialize(const char* name, std::string& value, bool is_static)
{
	if (m_object_stack.top().is_array() && m_index_stack.size() > 0)
	{
		std::string str = m_object_stack.top()[m_index_stack.top()];
		value = str;
	}
	else
	{
		std::string str = m_object_stack.top()[name];
		value = str;
	}
}

void JsonSerializer::deserialize(const char* name, char** value, bool is_static)
{
	std::string str = "";

	if (m_object_stack.top().is_array() && m_index_stack.size() > 0)
	{
		std::string temp = m_object_stack.top()[m_index_stack.top()];
		str = temp;
	}
	else
	{
		std::string temp = m_object_stack.top()[name];
		str = temp;
	}

	if (!is_static)
		*value = new char[str.length()];

	strcpy(*value, str.c_str());
}

void JsonSerializer::begin_deserialize_struct(const char* name)
{
	if (m_object_stack.top().is_array())
	{
		nlohmann::json obj = m_object_stack.top()[m_index_stack.top()];
		m_object_stack.push(obj);
	}
	else
	{
		nlohmann::json obj = m_object_stack.top()[name];
		m_object_stack.push(obj);
	}
}

void JsonSerializer::end_deserialize_struct(const char* name)
{
	m_object_stack.pop();
}

int JsonSerializer::begin_deserialize_array(const char* name)
{
	nlohmann::json array_container = m_object_stack.top()[name];
	nlohmann::json data = array_container["data"];
	int size = array_container["size"];
	m_object_stack.push(data);
	return size;
}

void JsonSerializer::end_deserialize_array(const char* name)
{
	m_object_stack.pop();
}

void JsonSerializer::raw_deserialize(void* data, const size_t& size)
{

}

TE_END_TERMINUS_NAMESPACE