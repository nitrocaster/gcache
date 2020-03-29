set(GC_MAIN_SOURCES
    GCache.cpp
)
source_group(src FILES ${GC_MAIN_SOURCES})

set(GC_LIBRARIES
    GCacheCore
)

add_executable(GCache ${GC_MAIN_SOURCES})
target_link_libraries(GCache ${GC_LIBRARIES})

target_include_directories(GCache PRIVATE "../")
target_compile_features(GCache PRIVATE cxx_std_17)
set_target_properties(GCache PROPERTIES CXX_VISIBILITY_PRESET hidden)

include(GNUInstallDirs)
set_target_properties(GCache PROPERTIES INSTALL_RPATH "\$ORIGIN/../${CMAKE_INSTALL_LIBDIR}")

install(TARGETS GCache)
