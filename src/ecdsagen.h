/*
    ecdsagen.h -- ECDSA key generation and export
    Copyright (C) 2014 Guus Sliepen <guus@meshlink.io>

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

#ifndef __MESHLINK_ECDSAGEN_H__
#define __MESHLINK_ECDSAGEN_H__

#include "ecdsa.h"

extern ecdsa_t *ecdsa_generate(void) __attribute__ ((__malloc__));
extern bool ecdsa_write_pem_public_key(ecdsa_t *ecdsa, FILE *fp) __attribute__ ((__warn_unused_result__));
extern bool ecdsa_write_pem_private_key(ecdsa_t *ecdsa, FILE *fp) __attribute__ ((__warn_unused_result__));

#endif
