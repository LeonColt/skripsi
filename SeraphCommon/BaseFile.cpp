#include "stdafx.h"
#include "BaseFile.h"

BaseFile::BaseFile() {}
void BaseFile::setName( LPCTSTR name ) { this->name.assign( name ); }
void BaseFile::setName( tstring& name ) { this->name.assign( name.c_str() ); }
void BaseFile::setName( tstring* name ) { this->name.assign( name->c_str() ); }
std::unique_ptr<tstring> BaseFile::getName() const noexcept { return std::make_unique<tstring>( name.c_str() ); }
void BaseFile::setPath( LPCTSTR path ) {this->path.assign( path );}
void BaseFile::setPath( tstring& path ){this->path.assign( path.c_str() );}
void BaseFile::setPath( tstring* path ){this->path.assign( path->c_str() );}
std::unique_ptr<tstring> BaseFile::getPath() const noexcept {return std::make_unique<tstring>( path.c_str() );}
bool BaseFile::operator==( BaseFile& base_file ) const noexcept {
	return path.compare( base_file.getPath()->c_str() ) == 0;
}
bool BaseFile::operator==( BaseFile* base_file ) const {
	return path.compare( base_file->getPath()->c_str() ) == 0;
}
bool BaseFile::operator!=( BaseFile& base_file ) const noexcept{
	return path.compare( base_file.getPath()->c_str() ) != 0;
}
bool BaseFile::operator!=( BaseFile* base_file ) const{
	return path.compare( base_file->getPath()->c_str() ) != 0;
}
bool BaseFile::compare( BaseFile& base_file ) const noexcept {
	return path.compare( base_file.getPath()->c_str() ) == 0;
}
bool BaseFile::compare( BaseFile* base_file ) const {
	return path.compare( base_file->getPath()->c_str() ) == 0;
}
void BaseFile::operator=( BaseFile& base_file ) noexcept {
	this->name.assign( base_file.name.c_str() );
	this->path.assign( base_file.path.c_str() );
}
void BaseFile::operator=( BaseFile* base_file ) noexcept {
	this->name.assign( base_file->name.c_str() );
	this->path.assign( base_file->path.c_str() );
}
void BaseFile::assign( BaseFile& base_file ) noexcept{
	this->name.assign( base_file.name.c_str() );
	this->path.assign( base_file.path.c_str() );
}
void BaseFile::assign( BaseFile* base_file ) noexcept{
	this->name.assign( base_file->name.c_str() );
	this->path.assign( base_file->path.c_str() );
}
BaseFile::~BaseFile() {}