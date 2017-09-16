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
#include <stdio.h>
#include <assert.h>
#include "../common/test_step.h"
#include "../common/containers.h"
#include "run_blackbox_tests.h"

char *meshlink_root_path = "/home/manavkumarm/meshlink";

int black_box_group0_setup(void **state) {
    char *nodes[] = { "peer", "relay" };

    destroy_containers();
    create_containers(nodes, 2);

    return 0;
}

int black_box_group0_teardown(void **state) {
    destroy_containers();

    return 0;
}

int main(int argc, char *argv[]) {
    char *invite_peer; //*invite_nut;

    black_box_group0_setup(NULL);

    char *test_1_nodes[] = { "relay", "peer" };
    black_box_state_t test_case_1_state = {
        /* test_case_name = */ "test_case_meta_conn_01",
        /* node_names = */ test_1_nodes,
        /* num_nodes = */ 2,
        /* test_result (defaulted to) = */ false
    };
    black_box_state_t *test_1_state_ptr = &test_case_1_state;
    setup_containers((void **)&test_1_state_ptr);
    invite_peer = invite_in_container("relay", "peer");
    //invite_nut = invite_in_container("relay", "nut");
    node_sim_in_container("relay", "1", NULL);
    node_sim_in_container("peer", "1", invite_peer);
    //execute_open(NUT_NODE_NAME, "1");
    //execute_join(invite_nut);
    //execute_start();

    //black_box_group0_teardown(NULL);

    while (1) sleep(1);

    /*const struct CMUnitTest blackbox_tests[] = {
        cmocka_unit_test(utest_create_list_01)
    };

    return cmocka_run_group_tests(group3Tests, utest_remove_from_list_group3_setup,
        utest_remove_from_list_group3_teardown);*/
    return EXIT_SUCCESS;
}
