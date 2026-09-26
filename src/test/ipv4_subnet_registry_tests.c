#include "ipv4_subnet_registry_tests.h"
#include "../ipv4_subnet_registry.h"
#include "../../lib/munit/munit.h"

#include <stddef.h>

uint32_t ipv4_str_to_int(const char *str);

struct Ipv4SubnetCheck {
    const char *ip;
    int expected_mask;
};

static void assert_ipv4_subnet_registry_checks(
    struct Ipv4SubnetRegistry *reg,
    const struct Ipv4SubnetCheck checks[],
    size_t check_count
) {
    for (size_t i = 0; i < check_count; i++) {
        munit_assert_char(ipv4_subnet_registry_check(reg, ipv4_str_to_int(checks[i].ip)), ==, checks[i].expected_mask);
    }
}

void ipv4_subnet_registry_test() {
    // Add a subnet and check
    struct Ipv4SubnetRegistry *reg = ipv4_subnet_registry_init();
    ipv4_subnet_registry_add(reg, ipv4_str_to_int("10.20.0.0"), 16);
    ipv4_subnet_registry_add(reg, ipv4_str_to_int("32.64.128.0"), 20);

    for (size_t i = ipv4_str_to_int("10.20.0.0"); i < ipv4_str_to_int("10.21.0.0"); i++) {
        munit_assert_int(ipv4_subnet_registry_check(reg, i), ==, 16);
    }
    for (size_t i = ipv4_str_to_int("32.64.128.0"); i < ipv4_str_to_int("32.64.144.0"); i++) {
        munit_assert_int(ipv4_subnet_registry_check(reg, i), ==, 20);
    }

    munit_assert_int(ipv4_subnet_registry_check(reg, ipv4_str_to_int("10.19.255.255")), ==, -1);
    munit_assert_int(ipv4_subnet_registry_check(reg, ipv4_str_to_int("10.21.0.0")), ==, -1);
    munit_assert_int(ipv4_subnet_registry_check(reg, ipv4_str_to_int("32.64.127.255")), ==, -1);
    munit_assert_int(ipv4_subnet_registry_check(reg, ipv4_str_to_int("32.64.144.0")), ==, -1);

    ipv4_subnet_registry_add(reg, ipv4_str_to_int("0.0.0.0"), 0);

    for (size_t i = ipv4_str_to_int("10.20.0.0"); i < ipv4_str_to_int("10.21.0.0"); i++) {
        munit_assert_int(ipv4_subnet_registry_check(reg, i), ==, 16);
    }
    for (size_t i = ipv4_str_to_int("32.64.128.0"); i < ipv4_str_to_int("32.64.144.0"); i++) {
        munit_assert_int(ipv4_subnet_registry_check(reg, i), ==, 20);
    }

    munit_assert_int(ipv4_subnet_registry_check(reg, ipv4_str_to_int("10.19.255.255")), ==, 0);
    munit_assert_int(ipv4_subnet_registry_check(reg, ipv4_str_to_int("10.21.0.0")), ==, 0);
    munit_assert_int(ipv4_subnet_registry_check(reg, ipv4_str_to_int("32.64.127.255")), ==, 0);
    munit_assert_int(ipv4_subnet_registry_check(reg, ipv4_str_to_int("32.64.144.0")), ==, 0);

    ipv4_subnet_registry_del(reg, ipv4_str_to_int("10.20.0.0"), 16);
    for (size_t i = ipv4_str_to_int("10.20.0.0"); i < ipv4_str_to_int("10.21.0.0"); i++) {
        munit_assert_int(ipv4_subnet_registry_check(reg, i), ==, 0);
    }
    for (size_t i = ipv4_str_to_int("32.64.128.0"); i < ipv4_str_to_int("32.64.144.0"); i++) {
        munit_assert_int(ipv4_subnet_registry_check(reg, i), ==, 20);
    }

    ipv4_subnet_registry_del(reg, ipv4_str_to_int("0.0.0.0"), 0);
    for (size_t i = ipv4_str_to_int("10.20.0.0"); i < ipv4_str_to_int("10.21.0.0"); i++) {
        munit_assert_int(ipv4_subnet_registry_check(reg, i), ==, -1);
    }
    for (size_t i = ipv4_str_to_int("32.64.128.0"); i < ipv4_str_to_int("32.64.144.0"); i++) {
        munit_assert_int(ipv4_subnet_registry_check(reg, i), ==, 20);
    }
    munit_assert_int(ipv4_subnet_registry_check(reg, ipv4_str_to_int("10.19.255.255")), ==, -1);
    munit_assert_int(ipv4_subnet_registry_check(reg, ipv4_str_to_int("10.21.0.0")), ==, -1);
    munit_assert_int(ipv4_subnet_registry_check(reg, ipv4_str_to_int("32.64.127.255")), ==, -1);
    munit_assert_int(ipv4_subnet_registry_check(reg, ipv4_str_to_int("32.64.144.0")), ==, -1);

    ipv4_subnet_registry_del(reg, ipv4_str_to_int("32.64.128.0"), 20);
    for (size_t i = ipv4_str_to_int("10.20.0.0"); i < ipv4_str_to_int("10.21.0.0"); i++) {
        munit_assert_int(ipv4_subnet_registry_check(reg, i), ==, -1);
    }
    for (size_t i = ipv4_str_to_int("32.64.128.0"); i < ipv4_str_to_int("32.64.144.0"); i++) {
        munit_assert_int(ipv4_subnet_registry_check(reg, i), ==, -1);
    }
    munit_assert_int(ipv4_subnet_registry_check(reg, ipv4_str_to_int("10.19.255.255")), ==, -1);
    munit_assert_int(ipv4_subnet_registry_check(reg, ipv4_str_to_int("10.21.0.0")), ==, -1);
    munit_assert_int(ipv4_subnet_registry_check(reg, ipv4_str_to_int("32.64.127.255")), ==, -1);
    munit_assert_int(ipv4_subnet_registry_check(reg, ipv4_str_to_int("32.64.144.0")), ==, -1);
}

void ipv4_subnet_registry_many_subnets_test() {
    struct Ipv4SubnetRegistry *reg = ipv4_subnet_registry_init();
    const struct {
        const char *base;
        uint8_t mask;
    } subnets[] = {
        {"10.20.1.192", 26},
        {"172.20.32.0", 20},
        {"192.168.0.0", 16},
        {"10.20.1.128", 25},
        {"198.51.100.0", 24},
        {"10.20.2.0", 23},
        {"172.20.0.0", 16},
        {"10.20.1.0", 24},
        {"192.168.1.0", 24},
        {"10.20.0.0", 16},
        {"192.168.1.64", 26},
        {"203.0.113.0", 24},
        {"172.16.0.0", 12},
        {"10.0.0.0", 8}
    };

    for (size_t i = 0; i < sizeof(subnets) / sizeof(subnets[0]); i++) {
        ipv4_subnet_registry_add(
            reg,
            ipv4_str_to_int(subnets[i].base),
            subnets[i].mask
        );
    }

    const struct Ipv4SubnetCheck initial_checks[] = {
        {"10.20.1.10", 24},
        {"10.20.1.127", 24},
        {"10.20.1.128", 25},
        {"10.20.1.191", 25},
        {"10.20.1.192", 26},
        {"10.20.1.255", 26},
        {"10.20.2.0", 23},
        {"10.20.3.255", 23},
        {"10.20.4.0", 16},
        {"10.99.255.255", 8},
        {"9.255.255.255", -1},
        {"172.19.255.255", 12},
        {"172.20.0.1", 16},
        {"172.20.32.0", 20},
        {"172.20.47.255", 20},
        {"172.20.48.0", 16},
        {"172.32.0.0", -1},
        {"192.168.1.63", 24},
        {"192.168.1.64", 26},
        {"192.168.1.127", 26},
        {"192.168.1.128", 24},
        {"192.169.0.0", -1},
        {"198.51.100.0", 24},
        {"198.51.101.0", -1},
        {"203.0.113.255", 24},
        {"203.0.114.0", -1}
    };
    assert_ipv4_subnet_registry_checks(
        reg,
        initial_checks,
        sizeof(initial_checks) / sizeof(initial_checks[0])
    );

    ipv4_subnet_registry_del(reg, ipv4_str_to_int("10.20.1.192"), 26);
    const struct Ipv4SubnetCheck after_deleting_10_26_checks[] = {
        {"10.20.1.192", 25},
        {"10.20.1.255", 25},
        {"10.20.1.127", 24},
        {"192.168.1.64", 26}
    };
    assert_ipv4_subnet_registry_checks(
        reg,
        after_deleting_10_26_checks,
        sizeof(after_deleting_10_26_checks) / sizeof(after_deleting_10_26_checks[0])
    );

    ipv4_subnet_registry_del(reg, ipv4_str_to_int("10.20.1.128"), 25);
    const struct Ipv4SubnetCheck after_deleting_10_25_checks[] = {
        {"10.20.1.128", 24},
        {"10.20.1.192", 24},
        {"10.20.1.255", 24},
        {"192.168.1.64", 26}
    };
    assert_ipv4_subnet_registry_checks(
        reg,
        after_deleting_10_25_checks,
        sizeof(after_deleting_10_25_checks) / sizeof(after_deleting_10_25_checks[0])
    );

    ipv4_subnet_registry_del(reg, ipv4_str_to_int("10.20.1.0"), 24);
    const struct Ipv4SubnetCheck after_deleting_10_24_checks[] = {
        {"10.20.1.0", 16},
        {"10.20.1.192", 16},
        {"10.20.2.0", 23},
        {"10.20.4.0", 16}
    };
    assert_ipv4_subnet_registry_checks(
        reg,
        after_deleting_10_24_checks,
        sizeof(after_deleting_10_24_checks) / sizeof(after_deleting_10_24_checks[0])
    );

    ipv4_subnet_registry_del(reg, ipv4_str_to_int("172.20.0.0"), 16);
    const struct Ipv4SubnetCheck after_deleting_172_16_checks[] = {
        {"172.20.1.1", 12},
        {"172.20.32.0", 20},
        {"172.20.47.255", 20},
        {"172.20.48.0", 12}
    };
    assert_ipv4_subnet_registry_checks(
        reg,
        after_deleting_172_16_checks,
        sizeof(after_deleting_172_16_checks) / sizeof(after_deleting_172_16_checks[0])
    );

    ipv4_subnet_registry_del(reg, ipv4_str_to_int("172.16.0.0"), 12);
    const struct Ipv4SubnetCheck after_deleting_172_12_checks[] = {
        {"172.20.1.1", -1},
        {"172.20.32.0", 20},
        {"172.20.47.255", 20},
        {"172.20.48.0", -1},
        {"10.20.1.1", 16},
        {"192.168.1.64", 26}
    };
    assert_ipv4_subnet_registry_checks(
        reg,
        after_deleting_172_12_checks,
        sizeof(after_deleting_172_12_checks) / sizeof(after_deleting_172_12_checks[0])
    );

    ipv4_subnet_registry_destory(reg);
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
