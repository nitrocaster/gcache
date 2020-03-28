set(GC_MAIN_SOURCES
    GCache.cpp
)
source_group(src FILES ${GC_MAIN_SOURCES})

set(GC_LIBRARIES
    GCacheCore
)

add_executable(GCache ${GC_MAIN_SOURCES})
target_link_libraries(GCache ${GC_LIBRARIES})
