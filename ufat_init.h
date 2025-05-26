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

#include <stdbool.h>
#include <string.h>

#include "ufat.h"

struct ufat_buffer_t* ufat_init_buffer_t( const unsigned long long bufsize );

struct ufat_device* ufat_init_device( const unsigned int log2_block_size );
struct ufat_file_device* ufat_init_file_device( const char* filepath, const unsigned int read_only
                                        , const unsigned int log2_block_size );
struct ufat_buffer_device* ufat_init_buffer_device( const unsigned long long bufsize
                                        , const unsigned int read_only, const unsigned int log2_block_size );


struct ufat_cache_desc* ufat_init_cache_desc( const ufat_block_t index, const int flags );
struct ufat_stat* ufat_init_stat( const bool read, const bool write );
struct ufat_bpb* ufat_init_bpb( const ufat_fat_type_t fat_type, const unsigned int log2_blocks_per_cluster
                            , const ufat_cluster_t num_clusters, const ufat_block_t fat_size );

struct ufat* ufat_init_ufat( const bool read, const bool write, const ufat_fat_type_t fat_type
                        , const unsigned int log2_blocks_per_cluster, const ufat_cluster_t num_clusters
                        , const ufat_block_t fat_size );

struct ufat_dirent* ufat_init_direntry( const ufat_block_t dirent_block, const unsigned int dirent_pos
                                , const char* dirent_short_name, const char* dirent_short_ext
                                , const ufat_attr_t dirent_attributes, const ufat_cluster_t first_cluster
                                , const ufat_size_t file_size, const ufat_date_t create_date
                                , const ufat_time_t create_time );

struct ufat_directory* ufat_init_directory( const ufat_block_t current_block, const ufat_block_t current_pos
                                    , struct ufat* uf );

struct ufat_file* ufat_init_file( struct ufat_dirent* direntry, const ufat_cluster_t prev_cluster
                            , struct ufat* uf );