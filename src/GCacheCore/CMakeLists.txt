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

target_include_directories(GCacheCore PUBLIC "../")
target_compile_features(GCacheCore PUBLIC cxx_std_17)
set_target_properties(GCacheCore PROPERTIES CXX_VISIBILITY_PRESET hidden)

install(TARGETS GCacheCore)
