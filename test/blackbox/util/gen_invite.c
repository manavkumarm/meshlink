/*
    gen_invite.c -- Black Box Test Utility to generate a meshlink invite
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
#include "../../../src/meshlink.h"
#include "../common/test_step.h"

#define CMD_LINE_ARG_NODENAME   1
#define CMD_LINE_ARG_INVITEE    2

int main(int argc, char *argv[]) {
    char *invite = NULL;
    meshlink_handle_t *mesh_handle = NULL;

    /* Start mesh, generate an invite and print out the invite */
    mesh_handle = execute_open(argv[CMD_LINE_ARG_NODENAME], "1");
    execute_start();
    invite = meshlink_invite(mesh_handle, argv[CMD_LINE_ARG_INVITEE]);
    fprintf(stderr, "meshlink_invite status: %s\n", meshlink_strerror(meshlink_errno));
    assert(invite);
    execute_close();
    printf("%s\n", invite);

    return EXIT_SUCCESS;
}
