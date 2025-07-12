// Copyright 2025 The OnePointer Authors.
//

#pragma once

#include <string>

extern "C" {
#include "ufat.h"

int	ufat_dev_read(const struct ufat_device *dev,
		ufat_block_t start, ufat_block_t count,
		unsigned char *buffer);
int	ufat_dev_write(const struct ufat_device *dev,
		ufat_block_t start, ufat_block_t count,
		const unsigned char *buffer);    
}

namespace UFAT {
            


class FileDevice
    :   public ufat_file_device
{
public:
    ufat_device& dev;
    const std::string fp;
    const bool readonly;

    FileDevice( ufat_device& dev, const char* filepath = "", const bool readonly_flag = false );
};


class DeviceBuffer
    :   public ufat_buffer_device
{
public:
    DeviceBuffer( ufat_device& dev );
    DeviceBuffer( FileDevice& fdev );
};


class DirEntry
    :   public ufat_dirent
{
public:
    DirEntry();
    DirEntry( ufat_dirent& dirent );
};


class File
    :   public ufat_file
{
public:
    DirEntry& dirent;
    ufat& uf;

    File( DirEntry& dirent, ufat& uf );
    File( ufat_dirent& dirent, ufat& uf );

    static File& open( ufat& uf, DirEntry& dirent );

    bool file_open();
    bool file_advance( ufat_size_t nbytes );
    char* file_read( ufat_size_t max_size );
    unsigned char* file_read_unsigned( ufat_size_t max_size );
    bool file_write( const char* buf, ufat_size_t buflen );
    bool file_truncate();
};


class Directory
    :   public ufat_directory
{
public:
    DirEntry& dirent;

    Directory( ufat_dirent& dirent, ufat& uf );
    Directory( ufat_directory& directory );

    bool dirdelete( DirEntry& dirent );
    DirEntry* dircreate( const char* name );
    DirEntry* mkfile( const char* name );

    DirEntry* find( const char* name );
    DirEntry* findpath( const char* path );
};


class FileSystem
    :   public ufat
{
public:
    FileDevice& fdev;

    FileSystem( ufat_device& dev, ufat_size_t devsize, bool read = true, bool write = true );

    static FileSystem& open( DeviceBuffer& devbuf );

    int fsopen();
    Directory& fsopenroot();
    Directory& fsopensubdir( DirEntry& dirent );
    int fssync();
    void fsclose();

    static ufat_block_t mkfs( ufat_device& dev, ufat_block_t nblk );
};


} // namespace UFAT