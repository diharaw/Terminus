#include <io/include/binary_serializer.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

BinarySerializer::BinarySerializer(IStream& stream, IAllocator* allocator) : ISerializer(stream, allocator)
{

}

BinarySerializer::~BinarySerializer()
{

}

void BinarySerializer::serialize(const char* name, bool& value)
{

}

void BinarySerializer::serialize(const char* name, int8_t& value)
{

}

void BinarySerializer::serialize(const char* name, uint8_t& value)
{

}

void BinarySerializer::serialize(const char* name, int16_t& value)
{

}

void BinarySerializer::serialize(const char* name, uint16_t& value)
{

}

void BinarySerializer::serialize(const char* name, int32_t& value)
{

}

void BinarySerializer::serialize(const char* name, uint32_t& value)
{

}

void BinarySerializer::serialize(const char* name, float& value)
{

}

void BinarySerializer::serialize(const char* name, double& value)
{

}

void BinarySerializer::serialize(const char* name, std::string& value)
{

}

void BinarySerializer::serialize(const char* name, const char* value)
{

}

void BinarySerializer::begin_serialize_struct(const char* name)
{

}

void BinarySerializer::end_serialize_struct(const char* name)
{

}

void BinarySerializer::begin_serialize_array(const char* name, int count)
{

}

void BinarySerializer::end_serialize_array(const char* name)
{

}

void BinarySerializer::deserialize(const char* name, bool& value)
{

}

void BinarySerializer::deserialize(const char* name, int8_t& value)
{

}

void BinarySerializer::deserialize(const char* name, uint8_t& value)
{

}

void BinarySerializer::deserialize(const char* name, int16_t& value)
{

}

void BinarySerializer::deserialize(const char* name, uint16_t& value)
{

}

void BinarySerializer::deserialize(const char* name, int32_t& value)
{

}

void BinarySerializer::deserialize(const char* name, uint32_t& value)
{

}

void BinarySerializer::deserialize(const char* name, float& value)
{

}

void BinarySerializer::deserialize(const char* name, double& value)
{

}

void BinarySerializer::deserialize(const char* name, std::string& value, bool is_static)
{

}

void BinarySerializer::deserialize(const char* name, char** value, bool is_static)
{

}

void BinarySerializer::begin_deserialize_struct(const char* name)
{

}

void BinarySerializer::end_deserialize_struct(const char* name)
{

}

int BinarySerializer::begin_deserialize_array(const char* name)
{
	return 0;
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

}

void BinarySerializer::raw_deserialize(void* data, const size_t& size)
{

}

void BinarySerializer::flush_to_stream()
{
	
}

TE_END_TERMINUS_NAMESPACE