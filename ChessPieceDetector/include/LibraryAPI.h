#ifndef _NET_LIBRARY_LIBRARY_API_
    #ifdef _WIN32
        #ifdef NETWORKLIBRARY_EXPORTS
            #define NETWORK_API __declspec(dllexport)
        #else
            #define NETWORK_API __declspec(dllimport)
        #endif
    #elif __GNUC__
        #ifdef NETWORKLIBRARY_EXPORTS
            #define NETWORK_API __attribute__((visibility("default")))
        #else
            #define NETWORK_API 
        #endif
    #endif
#endif