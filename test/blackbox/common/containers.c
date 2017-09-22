/*
    containers.h -- Container Management API
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
#include <unistd.h>
#include <pthread.h>
#include "containers.h"
#include "common_handlers.h"

static char *lxc_path = "/home/manavkumarm/.local/share/lxc";

/* Create all required test containers */
void create_containers(char *node_names[], int num_nodes) {
    int i;
    char container_name[100];
    int create_status, snapshot_status, snap_restore_status;
    struct lxc_container *first_container;

    for(i = 0; i < num_nodes; i++) {
        assert(snprintf(container_name, 100, "run_%s", node_names[i]) >= 0);
        if(i == 0) {
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

/* Return the handle to an existing container after finding it by container name */
struct lxc_container *find_container(char *name) {
    struct lxc_container **test_containers;
    char **container_names;
    int num_containers, i;

    fprintf(stderr, "In find_container(), name = %s\n", name);
    num_containers = list_all_containers(lxc_path, &container_names, &test_containers);
    assert(num_containers != -1);

    for(i = 0; i < num_containers; i++) {
        fprintf(stderr, "Found Container %s\n", container_names[i]);
        if(strcmp(container_names[i], name) == 0)
            return test_containers[i];
    }

    return NULL;
}

/* Rename a Container */
int rename_container(char *old_name, char *new_name) {
    char rename_command[200];
    int rename_status;
    struct lxc_container *old_container;

    /* Stop the old container if its still running */
    assert(old_container = find_container(old_name));
    old_container->shutdown(old_container, 5);
    /* Call stop() in case shutdown() fails - one of these two will always succeed */
    old_container->stop(old_container);
    /* Rename the Container */
    assert(snprintf(rename_command, 200, "%s/" LXC_UTIL_REL_PATH "/" LXC_RENAME_SCRIPT
        " %s %s %s", meshlink_root_path, lxc_path, old_name, new_name) >= 0);
    rename_status = system(rename_command);

    return rename_status;
}

/* Setup Containers required for a test
    This function should always be invoked in a CMocka context
    after setting the state of the test case to an instance of black_box_state_t */
void setup_containers(void **state) {
    black_box_state_t *test_state = (black_box_state_t *)(*state);
    int i, confbase_del_status;
    char build_command[200];
    struct lxc_container *test_container;
    char container_find_name[100];
    char container_new_name[100];
    int rename_status, build_status;
    char *ip;
    size_t ip_len;
    bool ip_found;
    FILE *lxcls_fp;
    char lxcls_command[200];

    PRINT_TEST_CASE_HEADER();
    for(i = 0; i < test_state->num_nodes; i++) {
        /* Locate the Container */
        assert(snprintf(container_find_name, 100, "run_%s", test_state->node_names[i]) >= 0);
        assert(test_container = find_container(container_find_name));

        /* Stop the Container if it's running */
        test_container->shutdown(test_container, 5);
        /* Call stop() in case shutdown() fails
            One of these two calls will always succeed */
        test_container->stop(test_container);
        /* Rename the Container to make it specific to this test case,
            if a Container with the target name already exists, skip this step */
        assert(snprintf(container_new_name, 100, "%s_%s", test_state->test_case_name,
            test_state->node_names[i]) >= 0);
        fprintf(stderr, "Rename to test container %s\n", container_new_name);
        struct lxc_container *new_container = find_container(container_new_name);
        fprintf(stderr, "find_container result: Found %s\n",
            (new_container) ? new_container->name : "no container");
        if (!new_container) {
            PRINT_TEST_CASE_MSG("Container '%s' rename status: ", test_container->name);
            rename_status = rename_container(test_container->name, container_new_name);
            fprintf(stderr, "%d\n", rename_status);
            assert(rename_status == 0);
        }

        /* Find the Container again and start the Container */
        assert(test_container = find_container(container_new_name));
        assert(test_container->start(test_container, 0, NULL));
        /* Wait for the Container to acquire an IP Address */
        assert(snprintf(lxcls_command, 200, "lxc-ls -f | grep %s | tr -s ' ' | cut -d ' ' -f 5",
            test_container->name) >= 0);
        PRINT_TEST_CASE_MSG("Waiting for Container '%s' to acquire IP", test_container->name);
        assert(ip = malloc(20));
        ip_len = sizeof(ip);
        ip_found = false;
        while (!ip_found) {
            assert(lxcls_fp = popen(lxcls_command, "r"));
            assert(getline((char **)&ip, &ip_len, lxcls_fp) != -1);
            ip[strlen(ip) - 1] = '\0';
            ip_found = (strcmp(ip, "-") != 0);
            assert(pclose(lxcls_fp) != -1);
            fprintf(stderr, ".");
            sleep(1);
        }
        fprintf(stderr, "\n");

        /* Build the Container by copying required files into it */
        assert(snprintf(build_command, 200, "%s/" LXC_UTIL_REL_PATH "/" LXC_BUILD_SCRIPT
            " %s %s %s +x >/dev/null", meshlink_root_path, test_state->test_case_name,
            test_state->node_names[i], meshlink_root_path) >= 0);
        build_status = system(build_command);
        PRINT_TEST_CASE_MSG("Container '%s' build Status: %d\n", test_container->name,
            build_status);
        assert(build_status == 0);
    }

    /* Delete any existing NUT confbase folder - every test case starts on a clean state */
    confbase_del_status = system("rm -rf testconf");
    PRINT_TEST_CASE_MSG("Confbase Folder Delete Status: %d\n", confbase_del_status);
    assert(confbase_del_status == 0);

    free(ip);
    return;
}

/* Destroy all Containers with names beginning with run_ */
void destroy_containers(void) {
    struct lxc_container **test_containers;
    char **container_names;
    int num_containers, i;

    num_containers = list_all_containers(lxc_path, &container_names, &test_containers);
    assert(num_containers != -1);

    for(i = 0; i < num_containers; i++) {
        if(strstr(container_names[i], "run_")) {
            fprintf(stderr, "Destroying Container '%s'\n", container_names[i]);
            test_containers[i]->shutdown(test_containers[i], 5);
            /* Call stop() in case shutdown() fails
                One of these two calls will always succeed */
            test_containers[i]->stop(test_containers[i]);
            test_containers[i]->destroy(test_containers[i]);
            /* call destroy_with_snapshots() in case destroy() fails
                one of these two calls will always succeed */
            test_containers[i]->destroy_with_snapshots(test_containers[i]);
        }
    }

    return;
}

/* Run 'cmd' inside the Container created for 'node' and return the first line of the output */
char *run_in_container(char *cmd, char *node, bool daemonize) {
    char attach_command[400];
    char *attach_argv[4];
    char container_find_name[100];
    struct lxc_container *container;
    FILE *attach_fp;
    char *output = NULL;
    size_t output_len;
    int i;

    assert(snprintf(container_find_name, 100, "%s_%s", state_ptr->test_case_name, node) >= 0);
    assert(container = find_container(container_find_name));
    if (daemonize) {
        for (i = 0; i < 3; i++)
            assert(attach_argv[i] = malloc(200));
        assert(snprintf(attach_argv[0], 200, "%s/" LXC_UTIL_REL_PATH "/" LXC_RUN_SCRIPT,
            meshlink_root_path) >= 0);
        strncpy(attach_argv[1], cmd, 200);
        strncpy(attach_argv[2], container->name, 200);
        attach_argv[3] = NULL;
        /* Create a child process and run the command in that child process */
        if (fork() == 0)
            assert(execv(attach_argv[0], attach_argv) != -1);   // Run exec() in the child process
        for (i = 0; i < 3; i++)
            free(attach_argv[i]);
    } else {
        assert(snprintf(attach_command, 200, "%s/" LXC_UTIL_REL_PATH "/" LXC_RUN_SCRIPT
            " \"%s\" %s", meshlink_root_path, cmd, container->name) >= 0);
        assert(attach_fp = popen(attach_command, "r"));
        /* If the command has an output, strip out any trailing carriage returns or newlines and
            return it, otherwise return NULL */
        assert(output = malloc(100));
        output_len = sizeof(output);
        if (getline(&output, &output_len, attach_fp) != -1) {
            i = strlen(output) - 1;
            while (output[i] == '\n' || output[i] == '\r')
                i--;
            output[i + 1] = '\0';
        }
        else {
            free(output);
            output = NULL;
        }
        assert(pclose(attach_fp) != -1);
    }

    return output;
}

/* Run the gen_invite command inside the 'inviter' container to generate an invite
    for 'invitee', and return the generated invite which is output on the terminal */
char *invite_in_container(char *inviter, char *invitee) {
    char invite_command[200];
    char *invite_url;

    assert(snprintf(invite_command, 200,
        "LD_LIBRARY_PATH=/home/ubuntu/test/.libs /home/ubuntu/test/gen_invite %s %s "
        "2> gen_invite.log", inviter, invitee) >= 0);
    assert(invite_url = run_in_container(invite_command, inviter, false));
    PRINT_TEST_CASE_MSG("Invite Generated from '%s' to '%s': %s\n", inviter,
        invitee, invite_url);

    return invite_url;
}

/* Run the node_sim_<nodename> program inside the 'node''s container */
void node_sim_in_container(char *node, char *device_class, char *invite_url) {
    char node_sim_command[200];

    assert(snprintf(node_sim_command, 200,
        "LD_LIBRARY_PATH=/home/ubuntu/test/.libs /home/ubuntu/test/node_sim_%s %s %s %s "
        "1>&2 2> node_sim_%s.log", node, node, device_class,
        (invite_url) ? invite_url : "", node) >= 0);
    run_in_container(node_sim_command, node, true);
    PRINT_TEST_CASE_MSG("node_sim_%s started in Container\n", node);

    return;
}

/* Run the node_step.sh script inside the 'node''s container to send the 'sig' signal to the
    node_sim program in the container */
void node_step_in_container(char *node, char *sig) {
    char node_step_command[200];

    assert(snprintf(node_step_command, 200,
        "/home/ubuntu/test/node_step.sh lt-node_sim_%s %s 1>&2 2> node_step.log",
        node, sig) >= 0);
    run_in_container(node_step_command, node, false);
    PRINT_TEST_CASE_MSG("Signal %s sent to node_sim_%s\n", sig, node);

    return;
}
