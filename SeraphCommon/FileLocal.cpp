#include "stdafx.h"
#include "FileLocal.h"


FileLocal::FileLocal() : SHARE_DEFAULT(0), 
SHARE_DELETE(FILE_SHARE_DELETE), 
SHARE_READ(FILE_SHARE_READ), 
SHARE_WRITE(FILE_SHARE_WRITE),
DISPOSITION_NEW(CREATE_NEW),
DISPOSITION_ALWAYS(CREATE_ALWAYS),
DISPOSITION_OPEN_ALWAYS(OPEN_ALWAYS),
DISPOSITION_OPEN_EXISTING(OPEN_EXISTING),
DISPOSITION_TRUNCATE_EXISTING(TRUNCATE_EXISTING){}

bool FileLocal::exists() {
	DWORD result = GetFileAttributes( getPath()->c_str() );
	return result != INVALID_FILE_ATTRIBUTES && !(result & FILE_ATTRIBUTE_DIRECTORY);
}
std::unique_ptr<tstring> FileLocal::getOwner() {
	return std::make_unique<tstring>( L"Leon Colt" );
	maschinen::file::FileHandler fh( getPath()->c_str(), GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL );
	if ( !fh.isValid() ) {
		TCHAR error[ MAX_LOADSTRING ];
		DWORD error_code;
		if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) )
			throw SeraphException( error, error_code );
		else throw SeraphException();
	}
	return fh.getOwner();
}

std::unique_ptr<tstring> FileLocal::getType() {
	FileTypeMapping mapper;
	mapper.read();
	return mapper.getType( getName()->c_str() );
}

ULONGLONG FileLocal::getSize() const {
	try {
		maschinen::file::FileHandler fh;
		return ( ULONGLONG ) fh.getSize( getPath()->c_str() );
	} catch(maschinen::exception::Exception& e ) {
		throw e;
	} catch(std::exception& e ) {
		throw e;
	}
}

std::unique_ptr<FILETIME> FileLocal::getCreatedTime() const {
	try {
		maschinen::file::FileHandler fh;
		return fh.getCreatedTime( getPath()->c_str() );
	} catch ( maschinen::exception::Exception& e ) {
		throw e;
	} catch ( std::exception& e ) {
		throw e;
	}
}

std::unique_ptr<FILETIME> FileLocal::getLastAccessTime() const {
	try {
		maschinen::file::FileHandler fh;
		return fh.getLastAccessTime( getPath()->c_str() );
	} catch ( maschinen::exception::Exception& e ) {
		throw e;
	} catch ( std::exception& e ) {
		throw e;
	}
}
std::unique_ptr<FILETIME> FileLocal::getLastWriteTime() const{
	try {
		maschinen::file::FileHandler fh;
		return fh.getLastWriteTime( getPath()->c_str() );
	} catch ( maschinen::exception::Exception& e ) {
		throw e;
	} catch ( std::exception& e ) {
		throw e;
	}
}

bool FileLocal::operator==( FileLocal& file ) {return getPath()->compare( file.getPath()->c_str() ) == 0;}
bool FileLocal::operator==( FileLocal* file ) {return getPath()->compare( file->getPath()->c_str() ) == 0;}
bool FileLocal::equal( FileLocal& file ) {return getPath()->compare( file.getPath()->c_str() ) == 0;}
bool FileLocal::equal( FileLocal* file ) {return getPath()->compare( file->getPath()->c_str() ) == 0;}

FileLocal::~FileLocal() {
}
