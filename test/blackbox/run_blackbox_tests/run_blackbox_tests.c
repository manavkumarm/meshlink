/*
    run_blackbox_tests.c -- Implementation of Black Box Test Execution for meshlink
    Copyright (C) 2017  Guus Sliepen <guus@meshlink.io>
                        Manav Kumar Mehta <manavkumarm@yahoo.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/
#include <stdlib.h>
#include <stdarg.h>
#include <setjmp.h>
#include <cmocka.h>
#include <assert.h>
#include "execute_tests.h"
#include "test_cases.h"
#include "../common/containers.h"
#include "../common/common_handlers.h"

#define CMD_LINE_ARG_MESHLINK_ROOT_PATH 1
#define CMD_LINE_ARG_LXC_PATH 2
#define CMD_LINE_ARG_LXC_BRIDGE_NAME 3

char *meshlink_root_path = NULL;

/* State structure for Meta-connections Test Case #1 */
static char *test_meta_conn_1_nodes[] = { "relay", "peer" };
static black_box_state_t test_meta_conn_1_state = {
    /* test_case_name = */ "test_case_meta_conn_01",
    /* node_names = */ test_meta_conn_1_nodes,
    /* num_nodes = */ 2,
    /* test_result (defaulted to) = */ false
};

/* State structure for Meta-connections Test Case #2 */
static char *test_meta_conn_2_nodes[] = { "relay", "peer" };
static black_box_state_t test_meta_conn_2_state = {
    /* test_case_name = */ "test_case_meta_conn_02",
    /* node_names = */ test_meta_conn_2_nodes,
    /* num_nodes = */ 2,
    /* test_result (defaulted to) = */ false
};

int black_box_group0_setup(void **state) {
    char *nodes[] = { "peer", "relay" };
    int num_nodes = sizeof(nodes) / sizeof(nodes[0]);

    printf("Creating Containers\n");
    destroy_containers();
    create_containers(nodes, num_nodes);

    return 0;
}

int black_box_group0_teardown(void **state) {
    printf("Destroying Containers\n");
    destroy_containers();

    return 0;
}

int main(int argc, char *argv[]) {
    const struct CMUnitTest blackbox_group0_tests[] = {
        cmocka_unit_test_prestate_setup_teardown(test_case_meta_conn_01, setup_test, teardown_test,
            (void *)&test_meta_conn_1_state),
        cmocka_unit_test_prestate_setup_teardown(test_case_meta_conn_02, setup_test, teardown_test,
            (void *)&test_meta_conn_2_state)
    };
    int num_tests = sizeof(blackbox_group0_tests) / sizeof(blackbox_group0_tests[0]);
    int failed_tests;

    /* Set configuration */
    assert(argc >= (CMD_LINE_ARG_LXC_BRIDGE_NAME + 1));
    meshlink_root_path = argv[CMD_LINE_ARG_MESHLINK_ROOT_PATH];
    lxc_path = argv[CMD_LINE_ARG_LXC_PATH];
    lxc_bridge = argv[CMD_LINE_ARG_LXC_BRIDGE_NAME];

    failed_tests = cmocka_run_group_tests(blackbox_group0_tests, black_box_group0_setup,
        black_box_group0_teardown);

    printf("[ PASSED ] %d test(s).\n", num_tests - failed_tests);
    printf("[ FAILED ] %d test(s).\n", failed_tests);

    return failed_tests;
}
