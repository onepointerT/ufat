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





struct ufat_cache_desc* ufat_init_cache_desc( const ufat_block_t index, const int flags ) {

    struct ufat_cache_desc* ufat_cd = malloc(sizeof(struct ufat_cache_desc));
    ufat_cd->flags = flags;
    ufat_cd->index = index;
    ufat_cd->seq = 0;

    return ufat_cd;

}

struct ufat_stat* ufat_init_stat( const bool read, const bool write ) {

    struct ufat_stat* ufat_st = malloc(sizeof(struct ufat_stat));
    ufat_st->read = read;
    ufat_st->write = write;
    ufat_st->read_blocks = 0;
    ufat_st->write_blocks = 0;
    ufat_st->cache_hit = 0;
    ufat_st->cache_miss = 0;
    ufat_st->cache_write = 0;
    ufat_st->cache_flush = 0;

    return ufat_st;

}

struct ufat_bpb* ufat_init_bpb( const ufat_fat_type_t fat_type, const unsigned int log2_blocks_per_cluster
                            , const ufat_cluster_t num_clusters, const ufat_block_t fat_size ) {

    struct ufat_bpb* ufat_b = malloc(sizeof(struct ufat_bpb));
    ufat_b->type = fat_type;
    ufat_b->log2_blocks_per_cluster = log2_blocks_per_cluster;
    ufat_b->fat_start = 0;
    ufat_b->fat_size = fat_size;
    ufat_b->fat_count = 1;
    ufat_b->cluster_start = 0;
    ufat_b->num_clusters = num_clusters;
    ufat_b->root_start = 0;
    ufat_b->root_size = fat_size;
    ufat_b->root_cluster = 1;

    return ufat_b;
}


struct ufat* ufat_init_ufat( const bool read, const bool write, const ufat_fat_type_t fat_type
                        , const unsigned int log2_blocks_per_cluster, const ufat_cluster_t num_clusters
                        , const ufat_block_t fat_size ) {

    struct ufat* uf = malloc(sizeof(struct ufat));
    uf->dev = ufat_init_device( log2_blocks_per_cluster );
    uf->stat = *(ufat_init_stat( read, write ));
    uf->bpb = *(ufat_init_bpb( fat_type, log2_blocks_per_cluster, num_clusters, fat_size ));
    uf->next_seq = 0;
    uf->cache_size = UFAT_CACHE_BYTES;
    uf->alloc_ptr = 0;
    
    return uf;

}


struct ufat_dirent* ufat_init_direntry( const ufat_block_t dirent_block, const unsigned int dirent_pos
                                , const char* dirent_short_name, const char* dirent_short_ext
                                , const ufat_attr_t dirent_attributes, const ufat_cluster_t first_cluster
                                , const ufat_size_t file_size, const ufat_date_t create_date
                                , const ufat_time_t create_time ) {

    struct ufat_dirent* ufat_de = malloc(sizeof(struct ufat_dirent));
    ufat_de->dirent_block = dirent_block;
    ufat_de->dirent_pos = dirent_pos;
    ufat_de->lfn_block = dirent_block;
    ufat_de->lfn_pos = dirent_pos;
    strncpy( ufat_de->short_name, dirent_short_name, 9 );
    strncpy( ufat_de->short_ext, dirent_short_ext, 4 );
    ufat_de->attributes = dirent_attributes;
    ufat_de->create_date = create_date;
    ufat_de->create_time = create_time;
    ufat_de->modify_date = 0;
    ufat_de->modify_time = 0;
    ufat_de->access_date = 0;
    ufat_de->first_cluster = first_cluster;
    ufat_de->file_size = file_size;

    return ufat_de;
}


struct ufat_directory* ufat_init_directory( const ufat_block_t current_block, const ufat_block_t current_pos
                                    , struct ufat* uf ) {

    struct ufat_directory* ufat_dir = malloc(sizeof(struct ufat_directory));
    ufat_dir->uf = uf;
    ufat_dir->cur_pos = current_pos;
    ufat_dir->cur_block = current_block;
    ufat_dir->start = current_block;

    return ufat_dir;

}


struct ufat_file* ufat_init_file( struct ufat_dirent* direntry, const ufat_cluster_t prev_cluster
                            , struct ufat* uf ) {

    struct ufat_file* ufat_fl = malloc(sizeof(struct ufat_file));
    ufat_fl->uf = uf;
    ufat_fl->dirent_block = direntry->dirent_block;
    ufat_fl->dirent_pos = direntry->dirent_pos;
    ufat_fl->start = direntry->first_cluster;
    ufat_fl->file_size = direntry->file_size;
    ufat_fl->prev_cluster = prev_cluster;
    ufat_fl->cur_cluster = direntry->first_cluster;
    ufat_fl->cur_pos = direntry->dirent_pos;

    return ufat_fl;

}


