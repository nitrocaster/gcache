// MIT License
// Copyright (c) 2020 Pavel Kovalenko

#if !defined(__GNUC__) && !defined(_MSC_VER)
#error Unsupported compiler
#endif

#if defined(__GNUC__)

#define GC_EXPORT __attribute__((visibility("default")))
#define GC_IMPORT __attribute__((visibility("default")))

#define MSVC_WARN_PUSH(id)
#define MSVC_WARN_PUSH_DISABLE(id)
#define MSVC_WARN_POP

#elif defined(_MSC_VER)

#define GC_EXPORT __declspec(dllexport)
#define GC_IMPORT __declspec(dllimport)

#define MSVC_WARN_PUSH(id)\
    __pragma(warning(push))

#define MSVC_WARN_PUSH_DISABLE(id)\
    __pragma(warning(push))\
    __pragma(warning(disable:id))

#define MSVC_WARN_POP\
    __pragma(warning(pop))

#endif
