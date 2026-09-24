cmake_minimum_required(VERSION 3.23)
project(ip-prefix-registry C)

add_executable(ip-registry-test)

set (SOURCE_FILES
    src/radix_uint32_trie.c
    src/uint32_util.c
    src/singly_list.c
)

target_sources(ip-registry-test
    PRIVATE
    lib/munit/munit.c
    src/test/test.c
    src/test/uint32_util_tests.c
    src/test/singly_list_tests.c
    ${SOURCE_FILES})

add_executable(ip-registry-demo)

target_sources(ip-registry-demo
    PRIVATE
    src/main.c
    ${SOURCE_FILES})