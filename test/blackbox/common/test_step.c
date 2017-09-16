/*
    test_step.c -- Handlers for executing test steps during node simulation
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

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "../../../src/meshlink.h"
#include "test_step.h"
#include "common_handlers.h"
#include "../common/node_sim.h"
#include "../run_blackbox_tests/run_blackbox_tests.h"

/* Modify this to change the logging level of Meshlink */
#define TEST_MESHLINK_LOG_LEVEL MESHLINK_DEBUG

meshlink_handle_t *mesh_handle = NULL;
bool mesh_started = false;

meshlink_handle_t *execute_open(char *node_name, char *dev_class) {
    /* Set up logging for Meshlink */
    meshlink_set_log_cb(NULL, TEST_MESHLINK_LOG_LEVEL, meshlink_callback_logger);

    /* Delete the meshlink confbase folder if it already exists */
    assert(system("rm -rf testconf") == 0);

    /* Create meshlink instance */
    mesh_handle = meshlink_open("testconf", node_name, "node_sim", atoi(dev_class));
    fprintf(stderr, "meshlink_open status: %s\n", meshlink_strerror(meshlink_errno));
    assert(mesh_handle);

    /* Set up logging for Meshlink with the newly acquired Mesh Handle */
    meshlink_set_log_cb(mesh_handle, TEST_MESHLINK_LOG_LEVEL, meshlink_callback_logger);

    return mesh_handle;
}

char *execute_invite(char *invitee) {
    char *invite_url = meshlink_invite(mesh_handle, invitee);

    fprintf(stderr, "meshlink_invite status: %s\n", meshlink_strerror(meshlink_errno));
    assert(invite_url);

    return invite_url;
}

void execute_join(char *invite_url) {
    bool join_status = meshlink_join(mesh_handle, invite_url);

    fprintf(stderr, "meshlink_join status: %s\n", meshlink_strerror(meshlink_errno));
    assert(join_status);

    return;
}

void execute_start(void) {
    bool start_init_status = meshlink_start(mesh_handle);

    fprintf(stderr, "meshlink_start status: %s\n", meshlink_strerror(meshlink_errno));
    assert(start_init_status);
    mesh_started = true;

    return;
}

void execute_stop(void) {
    meshlink_stop(mesh_handle);
    mesh_started = false;

    return;
}

void execute_close(void) {
    meshlink_close(mesh_handle);

    exit(EXIT_SUCCESS);
}

