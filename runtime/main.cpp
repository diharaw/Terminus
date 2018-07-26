#include <iostream>
#include <core/application.hpp>
#include <core/engine_core.hpp>
#include <core/terminus_macros.hpp>
#include <io/reflection.hpp>
#include <io/json_serializer.hpp>
#include <io/binary_serializer.hpp>
#include <io/memory_stream.hpp>
#include <io/file_stream.hpp>

TE_BEGIN_TERMINUS_NAMESPACE

void on_state_input(Event* e)
{
	if (e->data.input_state.hash == TE_HASH("fire"))
	{
		if (e->data.input_state.state == 1)
			std::cout << "Fire State ON" << std::endl;
		else
			std::cout << "Fire State OFF" << std::endl;
	}
}

void on_action_input(Event* e)
{
	if (e->data.input_action.hash == TE_HASH("block"))
		std::cout << "Block action" << std::endl;
}

void on_axis_input(Event* e)
{
	if (e->data.input_axis.hash == TE_HASH("forward"))
	{
		std::cout << "Forward axis : " << e->data.input_axis.value << std::endl;
	}
}

struct Bar
{
	int a;
	float b;

	REFLECT()
};

struct Foo
{
	Vector<Bar> test;
	Array<int, 10> list;
	StaticHashMap<StringBuffer32, int, 32> map;

	REFLECT()
};

BEGIN_DECLARE_REFLECT(Bar)
	REFLECT_MEMBER(a)
	REFLECT_MEMBER(b)
END_DECLARE_REFLECT()

BEGIN_DECLARE_REFLECT(Foo)
	REFLECT_MEMBER(test)
	REFLECT_MEMBER(list)
	REFLECT_MEMBER(map)
END_DECLARE_REFLECT()

void create_foo(Foo& foo)
{
	foo.list.resize(10);

	for (int i = 0; i < 10; i++)
		foo.list[i] = rand();

	for (int i = 0; i < 10; i++)
	{
		Bar obj;
		obj.a = rand();
		obj.b = rand();
		foo.test.push_back(obj);
	}

	foo.map.set("Jesse", 434);
	foo.map.set("Walter", 85);
}

void test_bin_serialize_fs(const Foo& foo)
{
	File* f = global::filesystem().open_file("test_f.bin", TE_FS_WRITE | TE_FS_BINARY);

	FileStream stream(f);
	BinarySerializer serializer(stream);

	serializer.save(foo);
	serializer.flush_to_stream();

	global::filesystem().close_file(f);
}

void test_bin_deserialize_fs()
{
	File* f = global::filesystem().open_file("test_f.bin", TE_FS_READ | TE_FS_BINARY);

	FileStream stream(f);
	BinarySerializer serializer(stream);

	Foo foo;
	serializer.load(foo);

	global::filesystem().close_file(f);
}

void test_serialize_fs(const Foo& foo)
{
	File* f = global::filesystem().open_file("test_f.json", TE_FS_WRITE | TE_FS_BINARY);

	FileStream stream(f);
	JsonSerializer serializer(stream);

	serializer.save(foo);
	serializer.flush_to_stream();

	global::filesystem().close_file(f);
}

void test_deserialize_fs()
{
	File* f = global::filesystem().open_file("test_f.json", TE_FS_READ | TE_FS_BINARY);

	FileStream stream(f);
	JsonSerializer serializer(stream);

	serializer.print();

	Foo foo;
	serializer.load(foo);

	global::filesystem().close_file(f);
}

void test_serialize_ms(const Foo& foo)
{
	File* f = global::filesystem().open_file("test_m.json", TE_FS_WRITE | TE_FS_BINARY);

	MemoryStream stream;
	JsonSerializer serializer(stream);

	serializer.save(foo);
	serializer.flush_to_stream();

	f->write(stream.data(), stream.size(), 1);
	global::filesystem().close_file(f);
}

void test_deserialize_ms()
{
	File* f = global::filesystem().open_file("test_m.json", TE_FS_READ | TE_FS_BINARY);

	size_t size = f->size();
	char* buf = (char*)TE_HEAP_ALLOC(size + 1);
	f->read(buf, size, 1);
	buf[size] = '\0';

	MemoryStream stream(buf, size);
	JsonSerializer serializer(stream);

	serializer.print();

	Foo foo;
	serializer.load(foo);

	global::filesystem().close_file(f);

	TE_HEAP_DEALLOC(buf);
}

class Runtime : public Application
{
public:
	InputMap input_map;

	Runtime()
	{
		m_title = "Terminus Runtime";
		m_width = 1280;
		m_height = 720;
		m_window_flags |= TE_WINDOW_RESIZABLE;
	}

	~Runtime()
	{

	}

	bool initialize() override
	{
		InputManager& input_manager = global::input_manager();
		EventManager& event_manager = global::event_manager();

		uint32_t player = input_manager.create_player();

		input_map.set_action(TE_HASH("block"), TE_MOUSE_LEFT, TE_UNASSIGNED_INPUT, TE_UNASSIGNED_INPUT);
		input_map.set_state(TE_HASH("fire"), TE_UNASSIGNED_INPUT, TE_KEYBOARD_A, TE_UNASSIGNED_INPUT);
		input_map.set_axis(TE_HASH("forward"), TE_UNASSIGNED_INPUT, TE_KEYBOARD_W, TE_KEYBOARD_S, TE_UNASSIGNED_INPUT);

		input_manager.set_input_map(&input_map, player);

		uint32_t callback_hndl = event_manager.register_callback(TE_EVENT_STATE_INPUT, on_state_input);

		event_manager.register_callback(TE_EVENT_AXIS_INPUT, on_axis_input);
		event_manager.register_callback(TE_EVENT_ACTION_INPUT, on_action_input);

		Foo foo;
		create_foo(foo);

		test_serialize_fs(foo);
		test_deserialize_fs();

		//test_serialize_ms();
		//test_deserialize_ms();

		test_bin_serialize_fs(foo);
		test_bin_deserialize_fs();

		return true;
	}

	void update() override
	{

	}

	void shutdown() override
	{

	}
};

TE_END_TERMINUS_NAMESPACE

TE_DECLARE_APP_MAIN(te::Runtime)