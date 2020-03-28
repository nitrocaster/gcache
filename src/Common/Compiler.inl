// MIT License
// Copyright (c) 2020 Pavel Kovalenko

#if !defined(__GNUC__) && !defined(_MSC_VER)
#error Unsupported compiler
#endif

#if defined(__GNUC__)

#define GC_EXPORT __attribute__((visibility("default")))
#define GC_IMPORT __attribute__((visibility("default")))

#elif defined(_MSC_VER)

#define GC_EXPORT __declspec(dllexport)
#define GC_IMPORT __declspec(dllimport)

#endif
