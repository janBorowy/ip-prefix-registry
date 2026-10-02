cmake_minimum_required(VERSION 3.23)
project(ip-prefix-registry C)

set(CMAKE_C_FLAGS_DEBUG "-g -DDEBUG")

add_executable(ip-registry-test)

set (SOURCE_FILES
    src/radix_binary_trie.c
    src/binary_string.c
    src/singly_list.c
    src/ipv4_subnet_registry.c
)

target_sources(ip-registry-test
    PRIVATE
    lib/munit/munit.c
    src/test/test.c
    src/test/binary_string_tests.c
    src/test/singly_list_tests.c
    src/test/radix_binary_trie_tests.c
    src/test/ipv4_subnet_registry_tests.c
    ${SOURCE_FILES})
