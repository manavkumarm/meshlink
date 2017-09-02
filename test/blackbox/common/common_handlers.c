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
#include "node_sim.h"
#include "test_step.h"

void mesh_close_signal_handler(int a) {
    execute_close();
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

void meshlink_callback_logger(meshlink_handle_t *mesh, meshlink_log_level_t level,
                                      const char *text) {
    fprintf(stderr, "meshlink>> %s\n", text);

    return;
}
