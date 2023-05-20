cmake_minimum_required(VERSION 3.15)
project(FinalProjExamples)

# filesystem.h require C++17
set(CMAKE_CXX_STANDARD 17)

file(COPY sample_data/ DESTINATION sample_data/)

# show compiler output and enable warnings
set(CMAKE_VERBOSE_MAKEFILE ON)
add_compile_options(-Wall -Wextra -pedantic)

add_executable(rapidJSONExample rapidJSONExample.cpp)
add_executable(supersearch porter2_stemmer.cpp main.cpp Persist.h)

include(CTest)
add_executable(tests_tree test/test_AVLtree.cpp)
add_test(NAME TestTree COMMAND tests_tree)
add_executable(tests_persistence porter2_stemmer.cpp test/test_Persistence.cpp)
add_test(NAME TestPersistence COMMAND tests_persistence)

# this makes sure we also link rapidjson
target_include_directories(rapidJSONExample PRIVATE rapidjson/)


set(CMAKE_VERBOSE_MAKEFILE OFF)
