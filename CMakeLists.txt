# uFAT -- small flexible VFAT implementation
# Copyright (C) 2012 TracMap Holdings Ltd
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
# 
# 1. Redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer.
# 
# 2. Redistributions in binary form must reproduce the above copyright
# notice, this list of conditions and the following disclaimer in the
# documentation and/or other materials provided with the distribution.
# 
# 3. Neither the name of the copyright holder nor the names of its
# contributors may be used to endorse or promote products derived from
# this software without specific prior written permission.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
# IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
# TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
# PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
# TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
# PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
# LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

cmake_minimum_required( VERSION 3.22 )

project( ufat C CXX )


set( PROJECT_NAME ufat )
set( PROJECT_DESCRIPTION "Small flexible Low-memory feature-complete VFAT implementation" )
set( PROJECT_VERSION "0.9.1" )

set( CMAKE_C_STANDARD 23 )
set( CMAKE_CXX_STANDARD 23 )

set( CMAKE_C_COMPILE_FEATURES "${CMAKE_C_COMPILE_FEATURES} -O1 -Wall -Wextra -Wshadow -Wpedantic -ggdb" )

if( UNIX )
    set( -fpermissive )
else()

endif( UNIX )

include_directories( ${PROJECT_SOURCE_DIR} )
set( UFAT_SRC ${PROJECT_SOURCE_DIR}/ufat_dir.c
            ${PROJECT_SOURCE_DIR}/ufat_ent.c
            ${PROJECT_SOURCE_DIR}/ufat_file.c
            ${PROJECT_SOURCE_DIR}/ufat_init.c
            ${PROJECT_SOURCE_DIR}/ufat_internal.c
            ${PROJECT_SOURCE_DIR}/ufat_mkfs.c
            ${PROJECT_SOURCE_DIR}/ufat.c
            ${PROJECT_SOURCE_DIR}/ufat.cpp
)

add_library( ufat ${UFAT_SRC} )
target_link_libraries( ufat )
