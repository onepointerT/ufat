cmake_minimum_required( VERSION 3.22 )

project( ufat C )


set( PROJECT_NAME ufat )
set( PROJECT_DESCRIPTION "Small flexible Low-memory feature-complete VFAT implementation" )
set( PROJECT_VERSION "0.9.0" )

set( CMAKE_C_STANDARD 23 )

set( CMAKE_C_COMPILE_FEATURES "${CMAKE_C_COMPILE_FEATURES} -O1 -Wall -Wextra -Wshadow -Wpedantic -ggdb" )

if( UNIX )
    set( -fpermissive )
else()

endif( UNIX )

include_directories( ${PROJECT_SOURCE_DIR} )
set( UFAT_SRC ${PROJECT_SOURCE_DIR}/ufat_dir.c
            ${PROJECT_SOURCE_DIR}/ufat_ent.c
            ${PROJECT_SOURCE_DIR}/ufat_file.c
            ${PROJECT_SOURCE_DIR}/ufat_internal.c
            ${PROJECT_SOURCE_DIR}/ufat_mkfs.c
            ${PROJECT_SOURCE_DIR}/ufat.c
)

add_library( ufat ${UFAT_SRC} )
target_link_libraries( ufat )
