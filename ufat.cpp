// Copyright 2025 The OnePointer Authors.
//

#include "ufat.hpp"

extern "C" {
#include "ufat_init.h"
}

namespace UFAT {


FileDevice::FileDevice( ufat_device& dev, const char* filepath, const bool readonly_flag )
    :   ufat_file_device( *ufat_init_file_device(filepath, readonly_flag, 4096) )
    ,   dev( dev )
    ,   fp( filepath )
    ,   readonly( readonly_flag )
{}


DeviceBuffer::DeviceBuffer( ufat_device& dev )
    :   ufat_buffer_device( *ufat_init_buffer_device( 0, false, 4096 ) )
{
    this->dev = &dev;
    this->buf = ufat_init_buffer_t( 0 );
}

DeviceBuffer::DeviceBuffer( FileDevice& fdev )
    :   ufat_buffer_device( *ufat_init_buffer_device( sizeof(fdev.dev.devbuf)/sizeof(unsigned char), fdev.read_only, 4096 ))
{
    this->dev = &fdev.dev;
    this->buf = ufat_init_buffer_t( 0 );
    this->buf->buf = fdev.dev.devbuf;
}


DirEntry::DirEntry()
    :   ufat_dirent( *ufat_init_direntry( 0, 0, "", "", UFAT_ATTR_USER
                                        , 0, 0, ufat_datenow(), ufat_timenow()
        ) )
{}

DirEntry::DirEntry( ufat_dirent& dirent )
    :   ufat_dirent( dirent )
{}



File::File( DirEntry& dirent, ufat& uf )
    :   ufat_file( *ufat_init_file( &dirent, 0, &uf ) )
    ,   dirent( dirent )
    ,   uf( uf )
{}


File::File( ufat_dirent& dirent, ufat& uf )
    :   ufat_file( *ufat_init_file( &dirent, 0, &uf ) )
    ,   dirent( *new DirEntry(dirent) )
    ,   uf( uf )
{}


File& File::open( ufat& uf, DirEntry& dirent ) {
    File* file = new File( dirent, uf );
    file->file_open();
    return *file;
}


bool File::file_open() {
    return ufat_open_file( &this->uf, this, &this->dirent ) == 0;
}

bool File::file_advance( ufat_size_t nbytes ) {
    return ufat_file_advance( this, nbytes ) == 0;
}

char* File::file_read( ufat_size_t max_size ) {
    char* buf = new char[max_size];
    if ( ufat_file_read( this, buf, max_size ) != 0 ) return "";
    return buf;
}


unsigned char* File::file_read_unsigned( ufat_size_t max_size ) {
    char* file_content = this->file_read( max_size );
    return reinterpret_cast<unsigned char*>( file_content );
}


bool File::file_write( const char* buf, ufat_size_t buflen ) {
    return ufat_file_write( this, buf, buflen ) == 0;
}

bool File::file_truncate() {
    return ufat_file_truncate( this ) == 0;
}


Directory::Directory( ufat_dirent& dirent, ufat& uf )
    :   ufat_directory( *ufat_init_directory(dirent.dirent_block
                            , dirent.dirent_pos, &uf ) )
    ,   dirent( *new DirEntry(dirent) )
{}

Directory::Directory( ufat_directory& directory )
    :   ufat_directory( directory )
    ,   dirent( *new DirEntry(*ufat_init_direntry(
                                directory.cur_block, directory.cur_pos
                             , "", "", UFAT_ATTR_DIRECTORY, directory.start
                             , 0, 0, 0
                            ))
               )
{}

bool Directory::dirdelete( DirEntry& dirent ) {
    return ufat_dir_delete( this->uf, &dirent ) == 0;
}

DirEntry* Directory::dircreate( const char* name ) {
    struct ufat_dirent* udirent = (struct ufat_dirent*) malloc(sizeof(struct ufat_dirent));
    if ( ufat_dir_create( this, udirent, name ) != 0 )
        return nullptr;
    DirEntry* dirent = new DirEntry( *udirent );
    return dirent;
}

DirEntry* Directory::mkfile( const char* name ) {
    struct ufat_dirent* udirent = (struct ufat_dirent*) malloc(sizeof(struct ufat_dirent));
    if ( ufat_dir_mkfile( this, udirent, name ) != 0 )
        return nullptr;
    DirEntry* dirent = new DirEntry( *udirent );
    return dirent;
}

DirEntry* Directory::find( const char* name ) {
    struct ufat_dirent* udirent = (struct ufat_dirent*) malloc(sizeof(struct ufat_dirent));
    if ( ufat_dir_find( this, name, udirent ) != 0 )
        return nullptr;
    DirEntry* dirent = new DirEntry( *udirent );
    return dirent;
}

DirEntry* Directory::findpath( const char* path ) {
    struct ufat_dirent* udirent = (struct ufat_dirent*) malloc(sizeof(struct ufat_dirent));
    const char** path_out = nullptr;
    if ( ufat_dir_find_path( this, path, udirent, path_out ) != 0 )
        return nullptr;
    DirEntry* dirent = new DirEntry( *udirent );
    return dirent;
}


FileSystem::FileSystem( ufat_device& dev, ufat_size_t devsize, bool read, bool write )
    :   ufat( *ufat_init_ufat( read, write, UFAT_TYPE_FAT32, dev.log2_block_size, devsize / dev.log2_block_size, devsize ) )
    ,   fdev( *new FileDevice( dev, "", !write && read ) )
{}


FileSystem& FileSystem::open( DeviceBuffer& devbuf ) {
    FileSystem* fs = new FileSystem( *devbuf.dev, devbuf.buf->bufsize );
    fs->fsopen();
    return *fs;
}

int FileSystem::fsopen() {
    return ufat_open( this, &fdev.dev );
}

Directory& FileSystem::fsopenroot() {
    struct ufat_directory* udir = (struct ufat_directory*) malloc(sizeof(struct ufat_directory));
    Directory* dir = new Directory( *udir );
    ufat_open_root( this, dir );
    return *dir;
}

Directory& FileSystem::fsopensubdir( DirEntry& dirent ) {
    Directory* dir = new Directory( dirent, *this );
    ufat_open_subdir( this, dir, &dirent );
    return *dir;
}

int FileSystem::fssync() {
    return ufat_sync( this );
}

void FileSystem::fsclose() {
    ufat_close( this );
}

ufat_block_t FileSystem::mkfs( ufat_device& dev, ufat_block_t nblk ) {
    if ( ufat_mkfs( &dev, nblk ) == 0 ) return nblk;
    return 0;
}


} // namespace UFAT