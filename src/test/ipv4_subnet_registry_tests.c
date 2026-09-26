#include "ipv4_subnet_registry_tests.h"
#include "../ipv4_subnet_registry.h"
#include "../../lib/munit/munit.h"

uint32_t ipv4_str_to_int(const char *str) {
    int ip[4];
    if (sscanf(str, "%d.%d.%d.%d", &ip[0], &ip[1], &ip[2], &ip[3]) != 4) {
        return 0;
    }
    return (uint32_t)ip[0] << 24 |
           (uint32_t)ip[1] << 16 |
           (uint32_t)ip[2] << 8 |
           (uint32_t)ip[3];
}

void ipv4_subnet_registry_test() {
    // Add a subnet and check
    struct Ipv4SubnetRegistry *reg = ipv4_subnet_registry_init();
    ipv4_subnet_registry_add(reg, ipv4_str_to_int("10.20.0.0"), 16);
    ipv4_subnet_registry_add(reg, ipv4_str_to_int("32.64.128.0"), 20);

    for (int i = ipv4_str_to_int("10.20.0.0"); i < ipv4_str_to_int("10.21.0.0"); i++) {
        munit_assert_int(ipv4_subnet_registry_check(reg, i), ==, 16);
    }
    for (int i = ipv4_str_to_int("32.64.128.0"); i < ipv4_str_to_int("32.64.144.0"); i++) {
        munit_assert_int(ipv4_subnet_registry_check(reg, i), ==, 20);
    }

    munit_assert_int(ipv4_subnet_registry_check(reg, ipv4_str_to_int("10.19.255.255")), ==, -1);
    munit_assert_int(ipv4_subnet_registry_check(reg, ipv4_str_to_int("10.21.0.0")), ==, -1);
    munit_assert_int(ipv4_subnet_registry_check(reg, ipv4_str_to_int("32.64.127.255")), ==, -1);
    munit_assert_int(ipv4_subnet_registry_check(reg, ipv4_str_to_int("32.64.144.0")), ==, -1);

    ipv4_subnet_registry_add(reg, ipv4_str_to_int("0.0.0.0"), 0);

    for (int i = ipv4_str_to_int("10.20.0.0"); i < ipv4_str_to_int("10.21.0.0"); i++) {
        munit_assert_int(ipv4_subnet_registry_check(reg, i), ==, 16);
    }
    for (int i = ipv4_str_to_int("32.64.128.0"); i < ipv4_str_to_int("32.64.144.0"); i++) {
        munit_assert_int(ipv4_subnet_registry_check(reg, i), ==, 20);
    }

    munit_assert_int(ipv4_subnet_registry_check(reg, ipv4_str_to_int("10.19.255.255")), ==, 0);
    munit_assert_int(ipv4_subnet_registry_check(reg, ipv4_str_to_int("10.21.0.0")), ==, 0);
    munit_assert_int(ipv4_subnet_registry_check(reg, ipv4_str_to_int("32.64.127.255")), ==, 0);
    munit_assert_int(ipv4_subnet_registry_check(reg, ipv4_str_to_int("32.64.144.0")), ==, 0);

    ipv4_subnet_registry_del(reg, ipv4_str_to_int("10.20.0.0"), 16);
    for (int i = ipv4_str_to_int("10.20.0.0"); i < ipv4_str_to_int("10.21.0.0"); i++) {
        munit_assert_int(ipv4_subnet_registry_check(reg, i), ==, 0);
    }
    for (int i = ipv4_str_to_int("32.64.128.0"); i < ipv4_str_to_int("32.64.144.0"); i++) {
        munit_assert_int(ipv4_subnet_registry_check(reg, i), ==, 20);
    }

    ipv4_subnet_registry_del(reg, ipv4_str_to_int("0.0.0.0"), 0);
    for (int i = ipv4_str_to_int("10.20.0.0"); i < ipv4_str_to_int("10.21.0.0"); i++) {
        munit_assert_int(ipv4_subnet_registry_check(reg, i), ==, -1);
    }
    for (int i = ipv4_str_to_int("32.64.128.0"); i < ipv4_str_to_int("32.64.144.0"); i++) {
        munit_assert_int(ipv4_subnet_registry_check(reg, i), ==, 20);
    }
    munit_assert_int(ipv4_subnet_registry_check(reg, ipv4_str_to_int("10.19.255.255")), ==, -1);
    munit_assert_int(ipv4_subnet_registry_check(reg, ipv4_str_to_int("10.21.0.0")), ==, -1);
    munit_assert_int(ipv4_subnet_registry_check(reg, ipv4_str_to_int("32.64.127.255")), ==, -1);
    munit_assert_int(ipv4_subnet_registry_check(reg, ipv4_str_to_int("32.64.144.0")), ==, -1);

    ipv4_subnet_registry_del(reg, ipv4_str_to_int("32.64.128.0"), 20);
    for (int i = ipv4_str_to_int("10.20.0.0"); i < ipv4_str_to_int("10.21.0.0"); i++) {
        munit_assert_int(ipv4_subnet_registry_check(reg, i), ==, -1);
    }
    for (int i = ipv4_str_to_int("32.64.128.0"); i < ipv4_str_to_int("32.64.144.0"); i++) {
        munit_assert_int(ipv4_subnet_registry_check(reg, i), ==, -1);
    }
    munit_assert_int(ipv4_subnet_registry_check(reg, ipv4_str_to_int("10.19.255.255")), ==, -1);
    munit_assert_int(ipv4_subnet_registry_check(reg, ipv4_str_to_int("10.21.0.0")), ==, -1);
    munit_assert_int(ipv4_subnet_registry_check(reg, ipv4_str_to_int("32.64.127.255")), ==, -1);
    munit_assert_int(ipv4_subnet_registry_check(reg, ipv4_str_to_int("32.64.144.0")), ==, -1);
}