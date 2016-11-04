#ifndef CONFIG_H
#define CONFIG_H

#ifdef __APPLE__
    #if !defined(TERMINUS_OPENGL) && !defined(TERMINUS_METAL)
        #define TERMINUS_OPENGL
    #endif
#else
    #ifdef WIN32
        #if !defined(TERMINUS_OPENGL) && !defined(TERMINUS_DIRECT3D11)
            #define TERMINUS_DIRECT3D11
        #endif
    #else
        #if !defined(TERMINUS_OPENGL) && !defined(TERMINUS_VULKAN)
            #define TERMINUS_OPENGL
        #endif
    #endif
#endif

#endif
