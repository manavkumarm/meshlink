/*
    execute_tests.c -- Utility functions for black box test execution
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
#include "../common/common_handlers.h"
#include "../common/containers.h"
#include "../common/test_step.h"

int setup_test(void **state) {
    printf("Setting up Containers\n");
    state_ptr = (black_box_state_t *)(*state);
    setup_containers(state);

    return EXIT_SUCCESS;
}

void execute_test(test_step_func_t step_func, void **state) {
    black_box_state_t *st_ptr = (black_box_state_t *) (*state);

    printf("Running Test\n");
    st_ptr->test_result = step_func();

    if (!st_ptr->test_result)
        fail();

    return;
}

int teardown_test(void **state) {
    black_box_state_t *st_ptr = (black_box_state_t *) (*state);
    char container_old_name[100], container_new_name[100];
    int i, rename_status;

    if (st_ptr->test_result) {
        PRINT_TEST_CASE_MSG("Test successful! Shutting down nodes.\n");
        for (i = 0; i < st_ptr->num_nodes; i++) {
            /* Shut down node */
            node_step_in_container(st_ptr->node_names[i], "SIGTERM");
            /* Rename Container to run_<node-name> - this allows it to be re-used for the
                next test, otherwise it will be ignored assuming that it has been saved
                for debugging */
            assert(snprintf(container_old_name, 100, "%s_%s", st_ptr->test_case_name,
                st_ptr->node_names[i]) >= 0);
            assert(snprintf(container_new_name, 100, "run_%s", st_ptr->node_names[i]) >= 0);
            rename_status = rename_container(container_old_name, container_new_name);
            PRINT_TEST_CASE_MSG("Container '%s' rename status: %d\n", container_old_name,
                rename_status);
            assert(rename_status == 0);
        }
    }

    PRINT_TEST_CASE_MSG("Terminating NUT.\n");
    execute_close();
    state_ptr = NULL;

    return EXIT_SUCCESS;
}
