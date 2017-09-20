/*
    test_cases.c -- Execution of specific meshlink black box test cases
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

#include "execute_tests.h"
#include "test_cases.h"
#include "../common/containers.h"
#include "../common/test_step.h"
#include "../common/common_handlers.h"

//static void utest_sig_handler(int sig);
//static sigjmp_buf stack;

/* static void utest_sig_handler(int sig) {
  longjmp(stack, 1);

  return;
} */

void test_case_meta_conn_01(void **state) {
    execute_test(test_steps_meta_conn_01, state);
    return;
}

bool test_steps_meta_conn_01(void) {
    char *invite_peer, *invite_nut;
    bool result = false;
    int i;

    invite_peer = invite_in_container("relay", "peer");
    invite_nut = invite_in_container("relay", NUT_NODE_NAME);
    node_sim_in_container("relay", "1", NULL);
    node_sim_in_container("peer", "1", invite_peer);
    execute_open(NUT_NODE_NAME, "1");
    execute_join(invite_nut);
    execute_start();
    PRINT_TEST_CASE_MSG("Waiting for peer to be connected\n");
    while (!meta_conn_status[1])
        sleep(1);
    node_step_in_container("peer", "SIGINT");
    PRINT_TEST_CASE_MSG("Waiting for peer to become unreachable\n");
    while (node_reachable_status[1])
        sleep(1);
    node_step_in_container("peer", "SIGINT");
    PRINT_TEST_CASE_MSG("Waiting 60 sec for peer to be re-connected\n");
    for (i = 0; i < 60; i++) {
        if (meta_conn_status[1]) {
            result = true;
            break;
        }
        sleep(1);
    }

    node_step_in_container("peer", "SIGTERM");
    node_step_in_container("relay", "SIGTERM");
    execute_close();
    free(invite_peer);
    free(invite_nut);

    return result;
}





