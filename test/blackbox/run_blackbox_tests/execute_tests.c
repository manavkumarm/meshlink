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
#include "execute_tests.h"
#include "../common/common_handlers.h"
#include "../common/containers.h"

int setup_test(void **state) {
    printf("Setting up Containers\n");
    state_ptr = (black_box_state_t *)(*state);
    setup_containers(state);

    return EXIT_SUCCESS;
}

void execute_test(test_step_func_t step_func, void **state) {
    black_box_state_t *st_ptr = (black_box_state_t *) state;

    printf("Running Test\n");
    st_ptr->test_result = step_func();

    if (!st_ptr->test_result)
        fail();

    return;
}
