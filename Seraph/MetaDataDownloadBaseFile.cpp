#include "stdafx.h"
#include "MetaDataDownloadBaseFile.h"


MetaDataDownloadBaseFile::MetaDataDownloadBaseFile() {
}

void MetaDataDownloadBaseFile::setId( QWORD id ) noexcept { this->id = id; }
QWORD MetaDataDownloadBaseFile::getId() const noexcept { return id; }
void MetaDataDownloadBaseFile::setName( tstring& name ) noexcept {this->name.assign( name.c_str() );}
void MetaDataDownloadBaseFile::setName( tstring* name ) noexcept {this->name.assign( name->c_str() );}
void MetaDataDownloadBaseFile::setName( LPCTSTR name ) noexcept { this->name.assign( name ); }
std::unique_ptr<tstring> MetaDataDownloadBaseFile::getName() const noexcept { return std::make_unique<tstring>( name.c_str() ); }
void MetaDataDownloadBaseFile::setSize( ULONGLONG size ) noexcept { this->size = size; }
ULONGLONG MetaDataDownloadBaseFile::getSize() const noexcept { return size; }
void MetaDataDownloadBaseFile::setOriginal( bool original ) noexcept { this->original = original; if ( isOriginal() ) parent = nullptr; }
bool MetaDataDownloadBaseFile::isOriginal() const noexcept {return original;}
void MetaDataDownloadBaseFile::setParent( MetaDataDownloadBaseFile* parent ) noexcept 
{
	if ( isOriginal() ) return;
	this->parent = parent; 
}
MetaDataDownloadBaseFile* MetaDataDownloadBaseFile::getParent() const noexcept { return parent; }

MetaDataDownloadBaseFile::~MetaDataDownloadBaseFile() {
}
