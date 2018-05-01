#pragma once

#include <io/include/serializer_macros.hpp>
#include <io/include/serializer.hpp>

// Json Includes
#include <json.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

class JsonSerializer : public ISerializer
{
public:
	JsonSerializer(IStream& stream, IAllocator* allocator);
	~JsonSerializer();
	void serialize(const char* name, bool& value) override;
	void serialize(const char* name, int8_t& value) override;
	void serialize(const char* name, uint8_t& value) override;
	void serialize(const char* name, int16_t& value) override;
	void serialize(const char* name, uint16_t& value) override;
	void serialize(const char* name, int32_t& value) override;
	void serialize(const char* name, uint32_t& value) override;
	void serialize(const char* name, float& value) override;
	void serialize(const char* name, double& value) override;
	void serialize(const char* name, std::string& value) override;
	void serialize(const char* name, const char* value) override;

	void begin_serialize_struct(const char* name) override;
	void end_serialize_struct(const char* name) override;
	void begin_serialize_array(const char* name, int count) override;
	void end_serialize_array(const char* name) override;

	void deserialize(const char* name, bool& value) override;
	void deserialize(const char* name, int8_t& value) override;
	void deserialize(const char* name, uint8_t& value) override;
	void deserialize(const char* name, int16_t& value) override;
	void deserialize(const char* name, uint16_t& value) override;
	void deserialize(const char* name, int32_t& value) override;
	void deserialize(const char* name, uint32_t& value) override;
	void deserialize(const char* name, float& value) override;
	void deserialize(const char* name, double& value) override;
	void deserialize(const char* name, std::string& value, bool is_static = true) override;
	void deserialize(const char* name, char** value, bool is_static = true) override;

	void begin_deserialize_struct(const char* name) override;
	void end_deserialize_struct(const char* name) override;
	int  begin_deserialize_array(const char* name) override;
	void end_deserialize_array(const char* name) override;

	bool is_raw_serializable() override;
	void raw_serialize(void* data, const size_t& size) override;
	void raw_deserialize(void* data, const size_t& size) override;

	void flush_to_stream() override;

	void print();

private:
	std::stack<nlohmann::json> m_object_stack;
};

TE_END_TERMINUS_NAMESPACE