/*
    mesh_simulate_node.h -- Global Declarations
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

#ifndef MESH_SIMULATE_NODE_H
#define MESH_SIMULATE_NODE_H

#include "../../../src/meshlink.h"

/* Meshlink Mesh Handle */
extern meshlink_handle_t *mesh_handle;

/* Flag to indicate if Mesh is running */
extern bool mesh_started;

#endif // MESHLINK_INTERNAL_H
