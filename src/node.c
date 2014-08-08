/*
    node.c -- node tree management
    Copyright (C) 2014 Guus Sliepen <guus@meshlink.io>,

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

#include "system.h"

#include "hash.h"
#include "logger.h"
#include "meshlink_internal.h"
#include "net.h"
#include "netutl.h"
#include "node.h"
#include "splay_tree.h"
#include "utils.h"
#include "xalloc.h"

static int node_compare(const node_t *a, const node_t *b) {
	return strcmp(a->name, b->name);
}

void init_nodes(meshlink_handle_t *mesh) {
	pthread_mutex_lock(&(mesh->nodes_mutex));
	mesh->nodes = splay_alloc_tree((splay_compare_t) node_compare, (splay_action_t) free_node);
	mesh->node_udp_cache = hash_alloc(0x100, sizeof(sockaddr_t));
	pthread_mutex_unlock(&(mesh->nodes_mutex));
}

void exit_nodes(meshlink_handle_t *mesh) {
	pthread_mutex_lock(&(mesh->nodes_mutex));
	if(mesh->node_udp_cache)
		hash_free(mesh->node_udp_cache);
	if(mesh->nodes)
		splay_delete_tree(mesh->nodes);
	mesh->node_udp_cache = NULL;
	mesh->nodes = NULL;
	pthread_mutex_unlock(&(mesh->nodes_mutex));
}

node_t *new_node(void) {
	node_t *n = xzalloc(sizeof *n);

	if(replaywin) n->late = xzalloc(replaywin);
	n->edge_tree = new_edge_tree();
	n->mtu = MTU;
	n->maxmtu = MTU;

	return n;
}

void free_node(node_t *n) {
	if(n->edge_tree)
		free_edge_tree(n->edge_tree);

	sockaddrfree(&n->address);

	ecdsa_free(n->ecdsa);
	sptps_stop(&n->sptps);

	if(n->mtutimeout.cb)
		abort();

	if(n->hostname)
		free(n->hostname);

	if(n->name)
		free(n->name);

	if(n->late)
		free(n->late);

	free(n);
}

void node_add(meshlink_handle_t *mesh, node_t *n) {
	pthread_mutex_lock(&(mesh->nodes_mutex));
	n->mesh = mesh;
	splay_insert(mesh->nodes, n);
	pthread_mutex_unlock(&(mesh->nodes_mutex));
}

void node_del(meshlink_handle_t *mesh, node_t *n) {
	pthread_mutex_lock(&(mesh->nodes_mutex));
	timeout_del(&mesh->loop, &n->mtutimeout);

	for splay_each(edge_t, e, n->edge_tree)
		edge_del(mesh, e);

	splay_delete(mesh->nodes, n);
	pthread_mutex_unlock(&(mesh->nodes_mutex));
}

node_t *lookup_node(meshlink_handle_t *mesh, const char *name) {
	node_t n = {NULL};
	node_t* result;

	n.name = name;

	pthread_mutex_lock(&(mesh->nodes_mutex));
	result = splay_search(mesh->nodes, &n);
	pthread_mutex_unlock(&(mesh->nodes_mutex));

	return result;
}

node_t *lookup_node_udp(meshlink_handle_t *mesh, const sockaddr_t *sa) {
	return hash_search(mesh->node_udp_cache, sa);
}

void update_node_udp(meshlink_handle_t *mesh, node_t *n, const sockaddr_t *sa) {
	if(n == mesh->self) {
		logger(DEBUG_ALWAYS, LOG_WARNING, "Trying to update UDP address of mesh->self!");
		return;
	}

	hash_insert(mesh->node_udp_cache, &n->address, NULL);

	if(sa) {
		n->address = *sa;
		n->sock = 0;
		for(int i = 0; i < mesh->listen_sockets; i++) {
			if(mesh->listen_socket[i].sa.sa.sa_family == sa->sa.sa_family) {
				n->sock = i;
				break;
			}
		}
		hash_insert(mesh->node_udp_cache, sa, n);
		free(n->hostname);
		n->hostname = sockaddr2hostname(&n->address);
		logger(DEBUG_PROTOCOL, LOG_DEBUG, "UDP address of %s set to %s", n->name, n->hostname);
	}
}
