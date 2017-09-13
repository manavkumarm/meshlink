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

static char *lxcpath = "/home/manavkumarm/.local/share/lxc";

/* Create all required test containers */
void create_containers(char *node_names[], int num_nodes) {
    int i;
    char container_name[100];
    struct lxc_container *first_container;

    for (i = 0; i < num_nodes; i++) {
        assert(snprintf(container_name, 100, "run_%s", node_names[i]) >= 0);
        fprintf(stderr, "Creating Container '%s'\n", container_name);
        if (i == 0) {
            assert(first_container = lxc_container_new(container_name, NULL));
            assert(!first_container->is_defined(first_container));
            assert(first_container->createl(first_container, "download", NULL, NULL,
                LXC_CREATE_QUIET, "-d", "ubuntu", "-r", "trusty", "-a", "i386", NULL));
            fprintf(stderr, "Snapshotting Container '%s'\n", container_name);
            assert(first_container->snapshot(first_container, NULL) != -1);
        } else
            assert(first_container->snapshot_restore(first_container, "snap0", container_name));
    }
}

/* Return the handle to an existing container after finding it by node name */
struct lxc_container *find_container(char *node_name) {
    struct lxc_container **test_containers;
    char **container_names;
    int num_containers, i;

    num_containers = list_all_containers(lxcpath, &container_names, &test_containers);
    assert(num_containers != -1);

    for (i = 0; i < num_containers; i++)
        if (strstr(container_names[i], node_name))
            return test_containers[i];

    return NULL;
}
