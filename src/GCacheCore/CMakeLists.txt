set(GC_CORE_SOURCES
    GCacheCore.hpp
    MD5.cpp
    MD5.hpp
    RecursiveDirectoryIterator.cpp
    RecursiveDirectoryIterator.hpp
)
source_group(src FILES ${GC_CORE_SRC})

add_library(GCacheCoreObj OBJECT ${GC_CORE_SOURCES})

target_compile_definitions(GCacheCoreObj PRIVATE
    GCACHECORE_EXPORTS=1
    DOCTEST_CONFIG_DISABLE=1 # disable tests for normal shared library
)
target_include_directories(GCacheCoreObj PUBLIC "../")
target_compile_features(GCacheCoreObj PUBLIC cxx_std_17)
set_target_properties(GCacheCoreObj PROPERTIES CXX_VISIBILITY_PRESET hidden)

add_library(GCacheCore SHARED $<TARGET_OBJECTS:GCacheCoreObj>)

install(TARGETS GCacheCore)

if(BUILD_TESTING)
    include("${CONAN_DOCTEST_ROOT}/lib/cmake/doctest/doctest.cmake")
    set(GC_CORE_TEST_SOURCES
        Test.cpp
    )
    add_executable(GCacheCoreTest ${GC_CORE_TEST_SOURCES} $<TARGET_OBJECTS:GCacheCoreObj>)
    target_compile_definitions(GCacheCoreTest PRIVATE
        GCACHECORE_EXPORTS=1 # because GCacheCore symbols are not being imported
    )
    target_include_directories(GCacheCoreTest PRIVATE "../")
    target_compile_features(GCacheCoreTest PRIVATE cxx_std_17)
    target_link_libraries(GCacheCoreTest PRIVATE CONAN_PKG::doctest)
    doctest_discover_tests(GCacheCoreTest)
endif()
