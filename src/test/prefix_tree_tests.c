#include "prefix_tree_tests.h"
#include "../prefix_tree.h"
#include "../../lib/munit/munit.h"

#include <stdio.h>
#include <stddef.h>

uint32_t ipv4_str_to_int(const char *str);

struct Ipv4Subnet {
    const char *base;
    uint8_t mask;
};

struct Ipv4SubnetCheck {
    const char *ip;
    int expected_mask;
};

static void assert_checks(
    const struct Ipv4SubnetCheck checks[],
    size_t check_count
) {
    for (size_t i = 0; i < check_count; i++) {
        munit_assert_char(check(ipv4_str_to_int(checks[i].ip)), ==, checks[i].expected_mask);
    }
}

void prefix_tree_test() {
    // Add a subnet and check
    add(ipv4_str_to_int("10.20.0.0"), 16);
    add(ipv4_str_to_int("32.64.128.0"), 20);

    for (size_t i = ipv4_str_to_int("10.20.0.0"); i < ipv4_str_to_int("10.21.0.0"); i++) {
        munit_assert_int(check(i), ==, 16);
    }
    for (size_t i = ipv4_str_to_int("32.64.128.0"); i < ipv4_str_to_int("32.64.144.0"); i++) {
        munit_assert_int(check(i), ==, 20);
    }

    munit_assert_int(check(ipv4_str_to_int("10.19.255.255")), ==, -1);
    munit_assert_int(check(ipv4_str_to_int("10.21.0.0")), ==, -1);
    munit_assert_int(check(ipv4_str_to_int("32.64.127.255")), ==, -1);
    munit_assert_int(check(ipv4_str_to_int("32.64.144.0")), ==, -1);

    add(ipv4_str_to_int("0.0.0.0"), 0);

    for (size_t i = ipv4_str_to_int("10.20.0.0"); i < ipv4_str_to_int("10.21.0.0"); i++) {
        munit_assert_int(check(i), ==, 16);
    }
    for (size_t i = ipv4_str_to_int("32.64.128.0"); i < ipv4_str_to_int("32.64.144.0"); i++) {
        munit_assert_int(check(i), ==, 20);
    }

    munit_assert_int(check(ipv4_str_to_int("10.19.255.255")), ==, 0);
    munit_assert_int(check(ipv4_str_to_int("10.21.0.0")), ==, 0);
    munit_assert_int(check(ipv4_str_to_int("32.64.127.255")), ==, 0);
    munit_assert_int(check(ipv4_str_to_int("32.64.144.0")), ==, 0);

    del(ipv4_str_to_int("10.20.0.0"), 16);
    for (size_t i = ipv4_str_to_int("10.20.0.0"); i < ipv4_str_to_int("10.21.0.0"); i++) {
        munit_assert_int(check(i), ==, 0);
    }
    for (size_t i = ipv4_str_to_int("32.64.128.0"); i < ipv4_str_to_int("32.64.144.0"); i++) {
        munit_assert_int(check(i), ==, 20);
    }

    del(ipv4_str_to_int("0.0.0.0"), 0);
    for (size_t i = ipv4_str_to_int("10.20.0.0"); i < ipv4_str_to_int("10.21.0.0"); i++) {
        munit_assert_int(check(i), ==, -1);
    }
    for (size_t i = ipv4_str_to_int("32.64.128.0"); i < ipv4_str_to_int("32.64.144.0"); i++) {
        munit_assert_int(check(i), ==, 20);
    }
    munit_assert_int(check(ipv4_str_to_int("10.19.255.255")), ==, -1);
    munit_assert_int(check(ipv4_str_to_int("10.21.0.0")), ==, -1);
    munit_assert_int(check(ipv4_str_to_int("32.64.127.255")), ==, -1);
    munit_assert_int(check(ipv4_str_to_int("32.64.144.0")), ==, -1);

    del(ipv4_str_to_int("32.64.128.0"), 20);
    for (size_t i = ipv4_str_to_int("10.20.0.0"); i < ipv4_str_to_int("10.21.0.0"); i++) {
        munit_assert_int(check(i), ==, -1);
    }
    for (size_t i = ipv4_str_to_int("32.64.128.0"); i < ipv4_str_to_int("32.64.144.0"); i++) {
        munit_assert_int(check(i), ==, -1);
    }
    munit_assert_int(check(ipv4_str_to_int("10.19.255.255")), ==, -1);
    munit_assert_int(check(ipv4_str_to_int("10.21.0.0")), ==, -1);
    munit_assert_int(check(ipv4_str_to_int("32.64.127.255")), ==, -1);
    munit_assert_int(check(ipv4_str_to_int("32.64.144.0")), ==, -1);
}

void prefix_tree_complex_test() {
    const struct Ipv4Subnet subnets[] = {
        {"10.0.0.0", 8},
        {"10.20.0.0", 16},
        {"10.20.2.0", 23},
        {"10.20.1.0", 24},
        {"10.20.1.128", 25},
        {"10.20.1.192", 26},
        {"172.16.0.0", 12},
        {"172.20.0.0", 16},
        {"172.20.32.0", 20},
        {"192.168.0.0", 16},
        {"192.168.1.0", 24},
        {"192.168.1.64", 26},
        {"198.51.100.0", 24},
        {"203.0.113.0", 24}
    };

    for (size_t i = 0; i < sizeof(subnets) / sizeof(subnets[0]); i++) {
        add(ipv4_str_to_int(subnets[i].base), subnets[i].mask);
    }

    const struct Ipv4SubnetCheck first_checks[] = {
        {"10.20.1.0", 24},
        {"10.20.1.10", 24},
        {"10.20.1.127", 24},
        {"10.20.1.128", 25},
        {"10.20.1.191", 25},
        {"10.20.1.192", 26},
        {"10.20.1.255", 26},
        {"10.20.2.0", 23},
        {"10.20.3.255", 23},
        {"10.20.4.0", 16},
        {"10.21.0.0", 8},
        {"10.99.255.255", 8},
        {"9.255.255.255", -1},
        {"172.19.255.255", 12},
        {"172.20.0.0", 16},
        {"172.20.0.1", 16},
        {"172.20.32.0", 20},
        {"172.20.47.255", 20},
        {"172.20.48.0", 16},
        {"172.32.0.0", -1},
        {"172.31.255.255", 12},
        {"192.168.1.63", 24},
        {"192.168.1.64", 26},
        {"192.168.1.127", 26},
        {"192.168.1.128", 24},
        {"192.169.0.0", -1},
        {"198.51.100.0", 24},
        {"198.51.101.0", -1},
        {"203.0.112.255", -1},
        {"203.0.113.0", 24},
        {"203.0.113.255", 24},
        {"203.0.114.0", -1}
    };
    assert_checks(first_checks, sizeof(first_checks) / sizeof(first_checks[0]));

    del(ipv4_str_to_int("10.20.1.192"), 26);
    const struct Ipv4SubnetCheck second_checks[] = {
        {"10.20.1.0", 24},
        {"10.20.1.10", 24},
        {"10.20.1.127", 24},
        {"10.20.1.128", 25},
        {"10.20.1.191", 25},
        {"10.20.1.192", 25}, // CHANGES
        {"10.20.1.255", 25}, // CHNAGES
        {"10.20.2.0", 23},
        {"10.20.3.255", 23},
        {"10.20.4.0", 16},
        {"10.21.0.0", 8},
        {"10.99.255.255", 8},
        {"9.255.255.255", -1},
        {"172.19.255.255", 12},
        {"172.20.0.0", 16},
        {"172.20.0.1", 16},
        {"172.20.32.0", 20},
        {"172.20.47.255", 20},
        {"172.20.48.0", 16},
        {"172.32.0.0", -1},
        {"172.31.255.255", 12},
        {"192.168.1.63", 24},
        {"192.168.1.64", 26},
        {"192.168.1.127", 26},
        {"192.168.1.128", 24},
        {"192.169.0.0", -1},
        {"198.51.100.0", 24},
        {"198.51.101.0", -1},
        {"203.0.112.255", -1},
        {"203.0.113.0", 24},
        {"203.0.113.255", 24},
        {"203.0.114.0", -1}
    };

    assert_checks(second_checks, sizeof(second_checks) / sizeof(second_checks[0]));

    del(ipv4_str_to_int("10.20.1.128"), 25);
    const struct Ipv4SubnetCheck third_checks[] = {
        {"10.20.1.0", 24},
        {"10.20.1.10", 24},
        {"10.20.1.127", 24},
        {"10.20.1.128", 24}, // CHANGES
        {"10.20.1.191", 24}, // CHANGES
        {"10.20.1.192", 24}, // CHANGES
        {"10.20.1.255", 24}, // CHNAGES
        {"10.20.2.0", 23},
        {"10.20.3.255", 23},
        {"10.20.4.0", 16},
        {"10.21.0.0", 8},
        {"10.99.255.255", 8},
        {"9.255.255.255", -1},
        {"172.19.255.255", 12},
        {"172.20.0.0", 16},
        {"172.20.0.1", 16},
        {"172.20.32.0", 20},
        {"172.20.47.255", 20},
        {"172.20.48.0", 16},
        {"172.32.0.0", -1},
        {"172.31.255.255", 12},
        {"192.168.1.63", 24},
        {"192.168.1.64", 26},
        {"192.168.1.127", 26},
        {"192.168.1.128", 24},
        {"192.169.0.0", -1},
        {"198.51.100.0", 24},
        {"198.51.101.0", -1},
        {"203.0.112.255", -1},
        {"203.0.113.0", 24},
        {"203.0.113.255", 24},
        {"203.0.114.0", -1}
    };

    assert_checks(third_checks, sizeof(third_checks) / sizeof(third_checks[0]));

    del(ipv4_str_to_int("10.20.1.0"), 24);
    const struct Ipv4SubnetCheck fourth_checks[] = {
        {"10.20.1.10", 16},  // CHANGES
        {"10.20.1.127", 16}, // CHANGES
        {"10.20.1.128", 16}, // CHANGES
        {"10.20.1.191", 16}, // CHANGES
        {"10.20.1.192", 16}, // CHANGES
        {"10.20.1.255", 16},
        {"10.20.2.0", 23},
        {"10.20.3.255", 23},
        {"10.20.4.0", 16},
        {"10.21.0.0", 8},
        {"10.99.255.255", 8},
        {"9.255.255.255", -1},
        {"172.19.255.255", 12},
        {"172.20.0.0", 16},
        {"172.20.0.1", 16},
        {"172.20.32.0", 20},
        {"172.20.47.255", 20},
        {"172.20.48.0", 16},
        {"172.32.0.0", -1},
        {"172.31.255.255", 12},
        {"192.168.1.63", 24},
        {"192.168.1.64", 26},
        {"192.168.1.127", 26},
        {"192.168.1.128", 24},
        {"192.169.0.0", -1},
        {"198.51.100.0", 24},
        {"198.51.101.0", -1},
        {"203.0.112.255", -1},
        {"203.0.113.0", 24},
        {"203.0.113.255", 24},
        {"203.0.114.0", -1}
    };
    assert_checks(fourth_checks, sizeof(fourth_checks) / sizeof(fourth_checks[0]));

    del(ipv4_str_to_int("172.20.0.0"), 16);
    const struct Ipv4SubnetCheck fifth_checks[] = {
        {"10.20.1.10", 16},
        {"10.20.1.127", 16},
        {"10.20.1.128", 16},
        {"10.20.1.191", 16},
        {"10.20.1.192", 16},
        {"10.20.1.255", 16},
        {"10.20.2.0", 23},
        {"10.20.3.255", 23},
        {"10.20.4.0", 16},
        {"10.21.0.0", 8},
        {"10.99.255.255", 8},
        {"9.255.255.255", -1},
        {"172.19.255.255", 12},
        {"172.20.0.0", 12}, // CHANGES
        {"172.20.0.1", 12}, // CHANGES
        {"172.20.32.0", 20},
        {"172.20.47.255", 20},
        {"172.20.48.0", 12}, // CHANGES
        {"172.32.0.0", -1},
        {"172.31.255.255", 12},
        {"192.168.1.63", 24},
        {"192.168.1.64", 26},
        {"192.168.1.127", 26},
        {"192.168.1.128", 24},
        {"192.169.0.0", -1},
        {"198.51.100.0", 24},
        {"198.51.101.0", -1},
        {"203.0.112.255", -1},
        {"203.0.113.0", 24},
        {"203.0.113.255", 24},
        {"203.0.114.0", -1}
    };
    assert_checks(fifth_checks, sizeof(fifth_checks) / sizeof(fifth_checks[0]));

    del(ipv4_str_to_int("172.16.0.0"), 12);
    const struct Ipv4SubnetCheck sixth_checks[] = {
        {"10.20.1.10", 16},
        {"10.20.1.127", 16},
        {"10.20.1.128", 16},
        {"10.20.1.191", 16},
        {"10.20.1.192", 16},
        {"10.20.1.255", 16},
        {"10.20.2.0", 23},
        {"10.20.3.255", 23},
        {"10.20.4.0", 16},
        {"10.21.0.0", 8},
        {"10.99.255.255", 8},
        {"9.255.255.255", -1},
        {"172.19.255.255", -1}, // CHANGES
        {"172.20.0.0", -1}, // CHANGES
        {"172.20.0.1", -1}, // CHANGES
        {"172.20.32.0", 20},
        {"172.20.47.255", 20},
        {"172.20.48.0", -1}, // CHANGES
        {"172.32.0.0", -1},
        {"172.31.255.255", -1}, // CHANGES
        {"192.168.1.63", 24},
        {"192.168.1.64", 26},
        {"192.168.1.127", 26},
        {"192.168.1.128", 24},
        {"192.169.0.0", -1},
        {"198.51.100.0", 24},
        {"198.51.101.0", -1},
        {"203.0.112.255", -1},
        {"203.0.113.0", 24},
        {"203.0.113.255", 24},
        {"203.0.114.0", -1}

    };
    assert_checks(sixth_checks, sizeof(sixth_checks) / sizeof(sixth_checks[0]));
}

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
