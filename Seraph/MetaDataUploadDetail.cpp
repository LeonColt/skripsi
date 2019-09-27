#include "stdafx.h"
#include "MetaDataUploadDetail.h"


MetaDataUploadDetail::MetaDataUploadDetail() {
	as_directory = false;
}

void MetaDataUploadDetail::setId( QWORD id ) noexcept { this->id = id; }
void MetaDataUploadDetail::setName( LPCTSTR name ) noexcept { this->name.assign( name ); }
void MetaDataUploadDetail::setName( tstring& name ) noexcept { this->name.assign( name.c_str() ); }
void MetaDataUploadDetail::setName( tstring* name ) noexcept { if ( name ) this->name.assign( name->c_str() ); }
void MetaDataUploadDetail::setPath( LPCTSTR path ) noexcept { this->path.assign( path ); }
void MetaDataUploadDetail::setPath( tstring& path ) noexcept { this->path.assign( path.c_str() ); }
void MetaDataUploadDetail::setPath( tstring* path ) noexcept {if(path) this->path.assign( path->c_str() );}
void MetaDataUploadDetail::asDirectory( bool as_directory ) noexcept { this->as_directory = as_directory; }

QWORD MetaDataUploadDetail::getId() const noexcept {return id;}
std::unique_ptr<tstring> MetaDataUploadDetail::getPath() const noexcept { return std::make_unique<tstring>( path.c_str() ); }
std::unique_ptr<tstring> MetaDataUploadDetail::getName() const noexcept { return std::make_unique<tstring>( name.c_str() ); }

QWORD MetaDataUploadDetail::getSize() const {
	maschinen::file::FileHandler fh;
	return ( QWORD ) fh.getSize( path.c_str() );
}

bool MetaDataUploadDetail::isDirectory() const noexcept {
	return as_directory;
}

MetaDataUploadDetail::~MetaDataUploadDetail() {
}
