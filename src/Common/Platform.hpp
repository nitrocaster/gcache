// MIT License
// Copyright (c) 2020 Pavel Kovalenko

#pragma once

#include "Common/Config.hpp"

#if defined(__linux__)
#define LINUX
#elif defined(_WIN32)
#define WINDOWS
#else
#error Unsupported platform
#endif

#include "Common/Compiler.inl"

#ifdef WINDOWS
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#endif
