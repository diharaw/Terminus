#include <io/binary_serializer.hpp>
#include <memory/allocator.hpp>
#include <core/engine_core.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

BinarySerializer::BinarySerializer(IStream& stream) : ISerializer(stream)
{
	m_stream.set_auto_move_write(true);
	m_stream.set_auto_move_read(true);
}

BinarySerializer::~BinarySerializer()
{

}

void BinarySerializer::serialize(const char* name, bool& value)
{
	m_stream.write(&value, sizeof(bool));
}

void BinarySerializer::serialize(const char* name, int8_t& value)
{
	m_stream.write(&value, sizeof(int8_t));
}

void BinarySerializer::serialize(const char* name, uint8_t& value)
{
	m_stream.write(&value, sizeof(uint8_t));
}

void BinarySerializer::serialize(const char* name, int16_t& value)
{
	m_stream.write(&value, sizeof(int16_t));
}

void BinarySerializer::serialize(const char* name, uint16_t& value)
{
	m_stream.write(&value, sizeof(uint16_t));
}

void BinarySerializer::serialize(const char* name, int32_t& value)
{
	m_stream.write(&value, sizeof(int32_t));
}

void BinarySerializer::serialize(const char* name, uint32_t& value)
{
	m_stream.write(&value, sizeof(int32_t));
}

void BinarySerializer::serialize(const char* name, float& value)
{
	m_stream.write(&value, sizeof(float));
}

void BinarySerializer::serialize(const char* name, double& value)
{
	m_stream.write(&value, sizeof(double));
}

void BinarySerializer::serialize(const char* name, std::string& value)
{
	begin_serialize_array(nullptr, value.length());
	m_stream.write((void*)value.c_str(), value.length());
}

void BinarySerializer::serialize(const char* name, const char* value)
{
	begin_serialize_array(nullptr, strlen(value));
	m_stream.write((void*)value, strlen(value));
}

void BinarySerializer::begin_serialize_struct(const char* name)
{

}

void BinarySerializer::end_serialize_struct(const char* name)
{

}

void BinarySerializer::begin_serialize_array(const char* name, int count)
{
	m_stream.write(&count, sizeof(uint32_t));
}

void BinarySerializer::end_serialize_array(const char* name)
{

}

void BinarySerializer::deserialize(const char* name, bool& value)
{
	m_stream.read(&value, sizeof(bool));
}

void BinarySerializer::deserialize(const char* name, int8_t& value)
{
	m_stream.read(&value, sizeof(int8_t));
}

void BinarySerializer::deserialize(const char* name, uint8_t& value)
{
	m_stream.read(&value, sizeof(uint8_t));
}

void BinarySerializer::deserialize(const char* name, int16_t& value)
{
	m_stream.read(&value, sizeof(int16_t));
}

void BinarySerializer::deserialize(const char* name, uint16_t& value)
{
	m_stream.read(&value, sizeof(uint16_t));
}

void BinarySerializer::deserialize(const char* name, int32_t& value)
{
	m_stream.read(&value, sizeof(int32_t));
}

void BinarySerializer::deserialize(const char* name, uint32_t& value)
{
	m_stream.read(&value, sizeof(uint32_t));
}

void BinarySerializer::deserialize(const char* name, float& value)
{
	m_stream.read(&value, sizeof(float));
}

void BinarySerializer::deserialize(const char* name, double& value)
{
	m_stream.read(&value, sizeof(double));
}

void BinarySerializer::deserialize(const char* name, std::string& value)
{
	int32_t size = begin_deserialize_array(nullptr);
	
	char* str = (char*)TE_HEAP_ALLOC(size + 1);
	m_stream.read(&str, sizeof(double));
	value = str;

	TE_HEAP_DEALLOC(str);
}

void BinarySerializer::deserialize(const char* name, char** value, bool is_static)
{
	int32_t size = begin_deserialize_array(nullptr);

	if (!is_static)
		*value = (char*)TE_HEAP_ALLOC(size + 1);

	m_stream.read(*value, sizeof(double));
}

void BinarySerializer::begin_deserialize_struct(const char* name)
{

}

void BinarySerializer::end_deserialize_struct(const char* name)
{

}

int BinarySerializer::begin_deserialize_array(const char* name)
{
	int32_t size = 0;
	m_stream.read(&size, sizeof(int32_t));
	return size;
}

void BinarySerializer::end_deserialize_array(const char* name)
{

}

bool BinarySerializer::is_raw_serializable()
{
	return true;
}

void BinarySerializer::raw_serialize(void* data, const size_t& size)
{
	m_stream.write(data, size);
}

void BinarySerializer::raw_deserialize(void* data, const size_t& size)
{
	m_stream.read(data, size);
}

void BinarySerializer::flush_to_stream()
{
	
}

TE_END_TERMINUS_NAMESPACE