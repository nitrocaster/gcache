# gcache

This little tool maintains cache of file timestamps, and restores changed timestamps with cached ones for files that didn't actually change. This can be useful to keep incremental builds working when stashing or rebasing in a git repository.

Files and directories which names start with dot are ignored. The cache itself is stored in `.hash_cache.txt`.

## Installation

```
git clone https://github.com/nitrocaster/gcache.git
cd gcache
mkdir build && cd build
cmake -DCMAKE_INSTALL_PREFIX="../pkg/" ..
cmake --build . --config Release
cmake --install .
```
Once done, binaries can be found in `pkg/bin`.

To build with ninja on Windows, run the following commands in *Developer Command Prompt for VS*:
```
git clone https://github.com/nitrocaster/gcache.git
cd gcache
mkdir build && cd build
cmake -GNinja -DCMAKE_INSTALL_PREFIX="../pkg/" ..
cmake --build . --config Release
cmake --install .
```
