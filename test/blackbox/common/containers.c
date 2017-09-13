/*
    containers.c -- Container Management API
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

#include <assert.h>
#include <string.h>
#include "containers.h"
#include "../run_blackbox_tests/run_blackbox_tests.h"

static char *lxc_path = "/home/manavkumarm/.local/share/lxc";

/* Create all required test containers */
void create_containers(char *node_names[], int num_nodes) {
    int i;
    char container_name[100];
    struct lxc_container *first_container;
    int create_status, snapshot_status, snap_restore_status;

    fprintf(stderr, "Creating Containers\n");
    for (i = 0; i < num_nodes; i++) {
        assert(snprintf(container_name, 100, "run_%s", node_names[i]) >= 0);
        if (i == 0) {
            assert(first_container = lxc_container_new(container_name, NULL));
            assert(!first_container->is_defined(first_container));
            create_status = first_container->createl(first_container, "download", NULL, NULL,
                LXC_CREATE_QUIET, "-d", "ubuntu", "-r", "trusty", "-a", "i386", NULL);
            fprintf(stderr, "Container '%s' create status: %d - %s\n", container_name,
                first_container->error_num, first_container->error_string);
            assert(create_status);
            snapshot_status = first_container->snapshot(first_container, NULL);
            fprintf(stderr, "Container '%s' snapshot status: %d - %s\n", container_name,
                first_container->error_num, first_container->error_string);
            assert(snapshot_status != -1);
        } else {
            snap_restore_status = first_container->snapshot_restore(first_container, "snap0",
                container_name);
            fprintf(stderr, "Snapshot restore to Container '%s' status: %d - %s\n", container_name,
                first_container->error_num, first_container->error_string);
            assert(snap_restore_status);
        }
    }
}

/* Return the handle to an existing container after finding it by node name */
struct lxc_container *find_container(char *node_name) {
    struct lxc_container **test_containers;
    char **container_names;
    int num_containers, i;

    num_containers = list_all_containers(lxc_path, &container_names, &test_containers);
    assert(num_containers != -1);

    for (i = 0; i < num_containers; i++)
        if (strstr(container_names[i], node_name))
            return test_containers[i];

    return NULL;
}

/* Setup Containers required for a test */
void setup_containers(void **state) {
    black_box_state_t *test_state = (black_box_state_t *)(*state);
    int i;
    char rename_command[200], build_command[200];
    struct lxc_container *test_container;
    int rename_status, build_status;

    for (i = 0; i < test_state->num_nodes; i++) {
        /* Locate the Container */
        assert(test_container = find_container(test_state->node_names[i]));

        /* Rename the Container to make it specific to this test case */
        assert(snprintf(rename_command, 200, "%s/" LXC_UTIL_REL_PATH "/" LXC_RENAME_SCRIPT
            " %s %s run_%s_%s", meshlink_root_path, lxc_path, test_container->name,
            test_state->test_case_name, test_state->node_names[i]));
        fprintf(stderr, "Container '%s' rename status: ", test_container->name);
        rename_status = system(rename_command);
        fprintf(stderr, "%d\n", rename_status);
        assert(rename_status == 0);

        /* Find the Container again and start the Container */
        assert(test_container = find_container(test_state->node_names[i]));
        assert(test_container->start(test_container, 0, NULL));
        /* Build the Container by copying required files into it */
        assert(snprintf(build_command, 200, "%s/" LXC_UTIL_REL_PATH "/" LXC_BUILD_SCRIPT
            " %s %s %s +x >/dev/null", meshlink_root_path, test_state->test_case_name,
            test_state->node_names[i], meshlink_root_path));
        build_status = system(build_command);
        fprintf(stderr, "Container '%s' Build Status: %d\n", test_container->name, build_status);
        assert(build_status == 0);
    }

    return;
}

/* Destroy all Containers with names beginning with test_ */
void destroy_containers(void) {
    struct lxc_container **test_containers;
    char **container_names;
    int num_containers, i;

    num_containers = list_all_containers(lxc_path, &container_names, &test_containers);
    assert(num_containers != -1);

    fprintf(stderr, "Destroying Containers if any\n");

    for (i = 0; i < num_containers; i++) {
        if (strstr(container_names[i], "run_")) {
            test_containers[i]->shutdown(test_containers[i], 5);
            test_containers[i]->destroy(test_containers[i]);
            test_containers[i]->destroy_with_snapshots(test_containers[i]);
        }
    }

    return;
}

