/* uFAT -- small flexible VFAT implementation
Copyright (C) 2012 TracMap Holdings Ltd

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

1. Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
contributors may be used to endorse or promote products derived from
this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "ufat_init.h"

#include <stdlib.h>


struct ufat_buffer_t* ufat_init_buffer_t( const unsigned long long bufsize ) {

    struct ufat_buffer_t* ufat_buf = malloc(sizeof(struct ufat_buffer_t));
    ufat_buf->bufmalloc = &ufat_bufmalloc;
    ufat_buf->bufallocate = &ufat_bufallocate;
    ufat_buf->read = &ufat_bufread;
    ufat_buf->write = &ufat_bufwrite;
    if ( ufat_bufmalloc( ufat_buf->buf, bufsize ) == 0 ) ufat_buf->bufsize = bufsize;

    return ufat_buf;

}


struct ufat_device* ufat_init_device( const unsigned int log2_block_size ) {

    struct ufat_device* ufat_dev = malloc(sizeof(struct ufat_device));
    ufat_dev->log2_block_size = log2_block_size;
    ufat_dev->read = &ufat_device_read;
    ufat_dev->write = &ufat_device_write;

    return ufat_dev;

}

struct ufat_file_device* ufat_init_file_device( const char* filepath, const unsigned int read_only, const unsigned int log2_block_size ) {

    struct ufat_device* ufat_dev = ufat_init_device( log2_block_size );
    ufat_dev->read = &ufat_filedevice_read;
    ufat_dev->write = &ufat_filedevice_write;

    struct ufat_file_device* ufat_fdev = malloc(sizeof(struct ufat_file_device));
    ufat_fdev->dev = ufat_dev;
    ufat_fdev->fd = fopen( filepath, read_only == 0 ? "r" : "rw" );
    ufat_fdev->read_only = read_only;

    return ufat_fdev;

}

struct ufat_buffer_device* ufat_init_buffer_device( const unsigned long long bufsize, const unsigned int read_only, const unsigned int log2_block_size ) {

    struct ufat_device* ufat_dev = ufat_init_device( log2_block_size );
    ufat_dev->read = &ufat_bufferdevice_read;
    ufat_dev->write = &ufat_bufferdevice_write;

    struct ufat_buffer_t* buf = ufat_init_buffer_t( bufsize );

    struct ufat_buffer_device* ufat_buf = malloc(sizeof(struct ufat_buffer_device));
    ufat_buf->buf = buf;
    ufat_buf->dev = ufat_dev;
    ufat_buf->read_only = read_only;

    return ufat_buf;

}

