#ifndef VECTOR_VECTOR_EXPORT_H
#define VECTOR_VECTOR_EXPORT_H

#ifdef _WIN32
    #ifdef VECTOR_EXPORTS
        #define VECTOR_API __declspec(dllexport)
    #else
        #define VECTOR_API __declspec(dllimport)
    #endif
#else
    #ifdef VECTOR_EXPORTS
        #define VECTOR_API __attribute__((visibility("default")))
    #else
        #define VECTOR_API
    #endif
#endif

#endif //VECTOR_VECTOR_EXPORT_H