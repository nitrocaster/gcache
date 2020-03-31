// MIT License
// Copyright (c) 2020 Pavel Kovalenko

#include "Common/Config.hpp"
#include "RecursiveDirectoryIterator.hpp"

namespace GCache
{
namespace fs = std::filesystem;

RecursiveDirectoryIterator::operator bool() const
{ return impl != end(impl); }

std::filesystem::path RecursiveDirectoryIterator::Path() const
{ return impl->path(); }

bool RecursiveDirectoryIterator::Directory() const
{ return impl->is_directory(); }

void RecursiveDirectoryIterator::Skip()
{ impl.disable_recursion_pending(); }

RecursiveDirectoryIterator::RecursiveDirectoryIterator(fs::path path) :
    impl(path)
{}

RecursiveDirectoryIterator &RecursiveDirectoryIterator::operator++()
{
    ++impl;
    return *this;
}
} // namespace GCache
