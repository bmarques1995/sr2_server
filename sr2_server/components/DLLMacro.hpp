#pragma once

#ifdef LIB_BUILD
    #define LIB_API __declspec(dllexport)
#elif LIB_EXPORT
    #define LIB_API __declspec(dllimport)
#else
    #define LIB_API
#endif
