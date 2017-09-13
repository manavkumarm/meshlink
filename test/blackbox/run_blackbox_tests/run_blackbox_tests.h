/*
    run_blackbox_tests.h -- Common Declarations for Black Box Test Driver Program
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

#ifndef RUN_BLACKBOX_TESTS_H
#define RUN_BLACKBOX_TESTS_H

typedef struct black_box_state {
    char *test_case_name;
    char **node_names;
    int num_nodes;
    bool test_result;
} black_box_state_t;

#define LXC_UTIL_REL_PATH "test/blackbox/util"
#define LXC_RENAME_SCRIPT "lxc_rename.sh"
#define LXC_RUN_SCRIPT "lxc_run.sh"
#define LXC_BUILD_SCRIPT "build_container.sh"

extern char *meshlink_root_path;

#endif // RUN_BLACKBOX_TESTS_H
