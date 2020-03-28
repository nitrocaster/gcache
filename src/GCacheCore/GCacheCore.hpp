// MIT License
// Copyright (c) 2020 Pavel Kovalenko

#pragma once

#include "Common/Config.hpp"
#include "Common/Platform.hpp"

#ifdef GCACHECORE_EXPORTS
#define GCACHECORE_API GC_EXPORT
#else
#define GCACHECORE_API GC_IMPORT
#endif
