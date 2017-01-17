#if defined(_MSC_VER)
#include <windows.h>
#elif defined(__GNUC__) || defined(__clang__)
#include <dlfcn.h>
#else
#error "unsupported platform"
#endif

namespace dynamic_library
{
#if defined(_MSC_VER)
    
    struct LibHandle
    {
        HINSTANCE _hndl;
    };

#elif defined(__GNUC__) || defined(__clang__)
    
    struct LibHandle
    {
        void* _hndl;
    };
    
#endif
    
    extern LibHandle open(const char* lib_name);
    
    extern void close(LibHandle& handle);
    
    extern bool valid_handle(LibHandle& handle);

	extern void* get_function(const char* func_name, LibHandle& hndl);

	template<typename T>
	T* create_instance_from_factory(const char* func_name, LibHandle& handle)
	{
		typedef T* (*CreateInstanceFunc)();
		void* func = get_function(func_name, handle);

		if (func)
		{
			CreateInstanceFunc func_ptr = reinterpret_cast<CreateInstanceFunc>(func);
			return func_ptr();
		}
		else
			return nullptr;
	}
}
