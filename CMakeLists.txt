cmake_minimum_required(VERSION 3.23)
project(ip-prefix-registry C)

add_executable(ip-registry-test)

set (SOURCE_FILES
    src/radix_uint32_trie.c
    src/uint32_util.c
)

target_sources(ip-registry-test
    PRIVATE
    lib/munit/munit.c
    src/test/test.c
    ${SOURCE_FILES})