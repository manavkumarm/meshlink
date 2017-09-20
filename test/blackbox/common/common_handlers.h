/*
    common_handlers.h -- Declarations of common callback handlers and signal handlers for
                            black box test cases
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

#ifndef COMMON_HANDLERS_H
#define COMMON_HANDLERS_H

#include "common_types.h"

#define PRINT_TEST_CASE_HEADER()        if (state_ptr) \
                                            fprintf(stderr, "[ %s ]\n", state_ptr->test_case_name)
#define PRINT_TEST_CASE_MSG(...)        if (state_ptr) \
                                            do { \
                                                fprintf(stderr, "[ %s ] ", \
                                                    state_ptr->test_case_name); \
                                                fprintf(stderr, __VA_ARGS__); \
                                            } while(0)

extern bool meta_conn_status[];
extern bool node_reachable_status[];

void mesh_close_signal_handler(int a);
void mesh_stop_start_signal_handler(int a);
void setup_signals(void);
void meshlink_callback_node_status(meshlink_handle_t *mesh, meshlink_node_t *node,
                                        bool reachable);
void meshlink_callback_logger(meshlink_handle_t *mesh, meshlink_log_level_t level,
                                      const char *text);

#endif // COMMON_HANDLERS_H
