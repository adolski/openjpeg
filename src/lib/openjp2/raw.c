/*
 * The copyright in this software is being made available under the 2-clauses
 * BSD License, included below. This software may be subject to other third
 * party and contributor rights, including patent rights, and no such rights
 * are granted under this license.
 *
 * Copyright (c) 2002-2014, Universite catholique de Louvain (UCL), Belgium
 * Copyright (c) 2002-2014, Professor Benoit Macq
 * Copyright (c) 2003-2007, Francois-Olivier Devaux
 * Copyright (c) 2003-2014, Antonin Descampe
 * Copyright (c) 2005, Herve Drolon, FreeImage Team
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS `AS IS'
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "opj_includes.h"

/*
==========================================================
   local functions
==========================================================
*/


/*
==========================================================
   RAW encoding interface
==========================================================
*/

opj_raw_t* opj_raw_create(void)
{
    opj_raw_t *raw = (opj_raw_t*)opj_malloc(sizeof(opj_raw_t));
    return raw;
}

void opj_raw_destroy(opj_raw_t *raw)
{
    if(raw) {
        opj_free(raw);
    }
}

OPJ_UINT32 opj_raw_numbytes(opj_raw_t *raw)
{
    const ptrdiff_t diff = raw->bp - raw->start;
    assert( diff <= (ptrdiff_t)0xffffffff && diff >= 0 ); /* UINT32_MAX */
    return (OPJ_UINT32)diff;
}

void opj_raw_init_dec(opj_raw_t *raw, OPJ_BYTE *bp, OPJ_UINT32 len)
{
    raw->start = bp;
    raw->lenmax = len;
    raw->len = 0;
    raw->c = 0;
    raw->ct = 0;
}

OPJ_UINT32 opj_raw_decode(opj_raw_t *raw)
{
    OPJ_UINT32 d;
    if (raw->ct == 0) {
        raw->ct = 8;
        if (raw->len == raw->lenmax) {
            raw->c = 0xff;
        } else {
            if (raw->c == 0xff) {
                raw->ct = 7;
            }
            raw->c = *(raw->start + raw->len);
            raw->len++;
        }
    }
    raw->ct--;
    d = (raw->c >> raw->ct) & 0x01;

    return d;
}

