#include <memory/src/buffer_linear_allocator.hpp>
#include <memory/include/memory_macros.hpp>

struct Test
{
	float a;
	float b;
};

int main()
{
	te::BufferLinearAllocator<TE_KILOBYTE(1)> alloc;

	Test* test = TE_NEW(&alloc) Test();

	test->a = 5336.3f;
	test->b = 2584.2f;

	return 0;
}
