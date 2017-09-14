/*
    containers.h -- Declarations for Container Management API
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

#ifndef CONTAINERS_H
#define CONTAINERS_H

#include <lxc/lxccontainer.h>

void create_containers(char *node_names[], int num_nodes);
struct lxc_container *find_container(char *container_name);
void setup_containers(void **state);
void destroy_containers(void);
char *run_in_container(char *cmd, char *node);
char *invite_in_container(char *inviter, char *invitee);

#endif // CONTAINERS_H
