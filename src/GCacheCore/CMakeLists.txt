set(GC_CORE_SOURCES
    GCacheCore.hpp
    MD5.cpp
    MD5.hpp
    RecursiveDirectoryIterator.cpp
    RecursiveDirectoryIterator.hpp
)
source_group(src FILES ${GC_CORE_SRC})

add_definitions(-DGCACHECORE_EXPORTS)

add_library(GCacheCore SHARED ${GC_CORE_SOURCES})
install(TARGETS GCacheCore)
