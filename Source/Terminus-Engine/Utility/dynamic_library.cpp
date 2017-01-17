#include "library_loader.h"

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
		wchar_t* str = get_wide_string(lib_name);

        LibHandle handle;
        handle._hndl = LoadLibrary(str);
		delete str;

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
        LibHandle handle;
        handle._hndl = dlopen(lib_name, RTLD_NOW);
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
