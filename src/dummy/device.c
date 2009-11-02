/*
    device.c -- Dummy device
    Copyright (C) 2009 Guus Sliepen <guus@tinc-vpn.org>

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

#include "logger.h"
#include "net.h"

int device_fd = -1;
char *device = "dummy";
char *iface = "dummy";
static char *device_info = "dummy device";

static int device_total_in = 0;
static int device_total_out = 0;

bool setup_device(void) {
	logger(LOG_INFO, "%s (%s) is a %s", device, iface, device_info);
	return true;
}

void close_device(void) {
}

bool read_packet(vpn_packet_t *packet) {
	return false;
}

bool write_packet(vpn_packet_t *packet) {
	device_total_out += packet->len;
	return true;
}

void dump_device_stats(void) {
	logger(LOG_DEBUG, "Statistics for %s %s:", device_info, device);
	logger(LOG_DEBUG, " total bytes in:  %10d", device_total_in);
	logger(LOG_DEBUG, " total bytes out: %10d", device_total_out);
}
