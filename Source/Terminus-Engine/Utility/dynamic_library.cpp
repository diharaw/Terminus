#include <Utility/dynamic_library.h>
#include <Core/config.h>
#include <string>
#include <cwchar>

wchar_t* get_wide_string(const char* str)
{
	size_t wn = std::mbsrtowcs(NULL, &str, 0, NULL);
	wchar_t* buf = new wchar_t[wn + 1];
	wn = std::mbsrtowcs(buf, &str, wn + 1, NULL);
	return buf;
}

namespace dynamic_library
{
    
#if defined(_MSC_VER)

    LibHandle open(const char* lib_name)
    {
        std::string formatted_name = std::string(lib_name);
        formatted_name += ".dll";

		//wchar_t* str = get_wide_string(formatted_name.c_str());

        LibHandle handle;
        handle._hndl = LoadLibrary(formatted_name.c_str());
		//delete str;

        return handle;
    }
    
    void* get_function(const char* func_name, LibHandle& handle)
    {
        return (void*)GetProcAddress(handle._hndl, func_name);
    }
    
    void close(LibHandle& handle)
    {
        FreeLibrary(handle._hndl);
    }
    
#elif defined(__GNUC__) || defined(__clang__)

    LibHandle open(const char* lib_name)
    {
        std::string formatted_name = std::string(lib_name);
#if defined(TERMINUS_PLATFORM_MACOS)
        formatted_name += ".dylib";
#else
        formatted_name += ".so;
#endif
        LibHandle handle;
        handle._hndl = dlopen(formatted_name.c_str(), RTLD_NOW);
        return handle;
    }
    
    void* get_function(const char* func_name, LibHandle& hndl)
    {
        return dlsym(hndl._hndl, func_name);
    }
    
    void close(LibHandle& handle)
    {
        dlclose(handle._hndl);
    }
    
#endif

	bool valid_handle(LibHandle& handle)
	{
		if (handle._hndl)
			return true;
		else
			return false;
    }

}
