/*
    common_handlers.c -- Implementation of common callback handling and signal handling
                            functions for black box tests
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
#include <signal.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "test_step.h"
#include "common_handlers.h"

black_box_state_t *state_ptr = NULL;

bool meta_conn_status[10];
bool node_reachable_status[10];

void mesh_close_signal_handler(int a) {
    execute_close();

    exit(EXIT_SUCCESS);
}

void mesh_stop_start_signal_handler(int a) {
    /* Stop the Mesh if it is running, otherwise start it again */
    (mesh_started) ? execute_stop() : execute_start();

    return;
}

void setup_signals(void) {
    signal(SIGTERM, mesh_close_signal_handler);
    signal(SIGINT, mesh_stop_start_signal_handler);

    return;
}

void meshlink_callback_node_status(meshlink_handle_t *mesh, meshlink_node_t *node,
                                        bool reachable) {
    int i;

    fprintf(stderr, "Node %s became %s\n", node->name, (reachable) ? "reachable" : "unreachable");

    if (state_ptr)
        for (i = 0; i < state_ptr->num_nodes; i++)
            if (strcmp(node->name, state_ptr->node_names[i]) == 0)
                node_reachable_status[i] = reachable;

    return;
}

void meshlink_callback_logger(meshlink_handle_t *mesh, meshlink_log_level_t level,
                                      const char *text) {
    int i;
    char connection_match_msg[100];

    fprintf(stderr, "meshlink>> %s\n", text);

    if (state_ptr && (strstr(text, "Connection") || strstr(text, "connection")))
        for (i = 0; i < state_ptr->num_nodes; i++) {
            assert(snprintf(connection_match_msg, sizeof(connection_match_msg),
                "Connection with %s", state_ptr->node_names[i]) >= 0);
            if (strstr(text, connection_match_msg) && strstr(text, "activated")) {
                meta_conn_status[i] = true;
                continue;
            }

            assert(snprintf(connection_match_msg, sizeof(connection_match_msg),
                "Connection closed by %s", state_ptr->node_names[i]) >= 0);
            if (strstr(text, connection_match_msg)) {
                meta_conn_status[i] = false;
                continue;
            }

            assert(snprintf(connection_match_msg, sizeof(connection_match_msg),
                "Closing connection with %s", state_ptr->node_names[i]) >= 0);
            if (strstr(text, connection_match_msg)) {
                meta_conn_status[i] = false;
                continue;
            }
        }

    return;
}
