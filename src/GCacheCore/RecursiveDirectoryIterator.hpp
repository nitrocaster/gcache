// MIT License
// Copyright (c) 2020 Pavel Kovalenko

#pragma once

#include "Common/Config.hpp"
#include "GCacheCore.hpp"
#include <filesystem>

class GCACHECORE_API RecursiveDirectoryIterator
{
public:
	RecursiveDirectoryIterator(std::filesystem::path path);
	RecursiveDirectoryIterator &operator++();
	operator bool() const;
	std::filesystem::path Path() const;
	bool Directory() const;
	void Skip();

private:
	std::filesystem::recursive_directory_iterator impl;
};
