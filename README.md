# gcache

This little tool maintains cache of file timestamps, and restores changed
timestamps with cached ones for files that didn't actually change. This
can be useful to keep incremental builds working when stashing or
rebasing in a git repository.

Files and directories which names start with dot are ignored. The cache
itself is stored in `.hash_cache.txt`.

## Prerequisites

- C++17 compiler (tested with GCC 9.1 and MSVC 19.25)
- [CMake](https://cmake.org) 3.16+
- [Conan](https://conan.io)

## Installation

```
git clone https://github.com/nitrocaster/gcache.git
cd gcache
mkdir build && cd build
conan install ..
cmake -DCMAKE_INSTALL_PREFIX="../pkg/" -DBUILD_TESTING=1 ..
cmake --build . --config Release
cmake --install .
ctest -C Release
```
Once done, binaries can be found in `pkg/bin`.

To build with ninja on Windows, run the following commands in
*Developer Command Prompt for VS*:
```
git clone https://github.com/nitrocaster/gcache.git
cd gcache
mkdir build && cd build
conan install ..
cmake -GNinja -DCMAKE_INSTALL_PREFIX="../pkg/" -DBUILD_TESTING=1 ..
cmake --build . --config Release
cmake --install .
ctest -C Release
```
