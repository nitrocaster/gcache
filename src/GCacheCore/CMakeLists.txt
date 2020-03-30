set(GC_CORE_SOURCES
    GCacheCore.hpp
    MD5.cpp
    MD5.hpp
    RecursiveDirectoryIterator.cpp
    RecursiveDirectoryIterator.hpp
)
source_group(src FILES ${GC_CORE_SRC})

add_library(GCacheCore SHARED ${GC_CORE_SOURCES})

target_compile_definitions(GCacheCore PRIVATE
    GCACHECORE_EXPORTS=1
    DOCTEST_CONFIG_DISABLE=1 # disable tests for normal shared library
)
target_include_directories(GCacheCore PUBLIC "../")
target_compile_features(GCacheCore PUBLIC cxx_std_17)
set_target_properties(GCacheCore PROPERTIES CXX_VISIBILITY_PRESET hidden)

install(TARGETS GCacheCore)

if(BUILD_TESTING)
    set(GC_CORE_TEST_SOURCES
        ${GC_CORE_SOURCES}
        Test.cpp
    )
    add_executable(GCacheCoreTest ${GC_CORE_TEST_SOURCES})
    target_compile_definitions(GCacheCoreTest PRIVATE
        GCACHECORE_EXPORTS=1 # because GCacheCore symbols are not being imported
    )
    target_include_directories(GCacheCoreTest PRIVATE "../")
    target_compile_features(GCacheCoreTest PRIVATE cxx_std_17)
    add_test(NAME GCacheCore COMMAND $<TARGET_FILE:GCacheCoreTest>)
endif()
