#include "stdafx.h"
#include "LocalIterator.h"


LocalIterator::LocalIterator( Local* pointer ) {this->pointer = pointer;}
LocalIterator& LocalIterator::operator=( Local* pointer ) {
	this->pointer = pointer;
	return ( *this );
}
LocalIterator::operator bool() const {if ( pointer ) return true; else return false;}
bool LocalIterator::operator==( LocalIterator& pointer ) const {return this->pointer == pointer.getConst();}
bool LocalIterator::operator!=( LocalIterator& pointer ) const {return this->pointer != pointer.getConst();}
LocalIterator& LocalIterator::operator++() {
	( *dri )++;
	pointer = nullptr;
	for(; *dri != dh->end(); (*dri)++ ) {
		if ( dri->file_name.compare( TEXT( "." ) ) == 0
			 || dri->file_name.compare( TEXT( ".." ) ) == 0
			 || dri->file_name.compare( TEXT( "$RECYCLE.BIN" ) ) == 0
			 || dri->file_name.compare( TEXT( "$Recycle.Bin" ) ) == 0
			 || dri->file_name.compare( TEXT( "bootmgr" ) ) == 0
			 || dri->file_name.compare( TEXT( "BOOTNXT" ) ) == 0
			 || dri->file_name.compare( TEXT( "hiberfil.sys" ) ) == 0
			 || dri->file_name.compare( TEXT( "pagefile.sys" ) ) == 0
			 || dri->file_name.compare( TEXT( "Recovery" ) ) == 0
			 || dri->file_name.compare( TEXT( "swapfile.sys" ) ) == 0
			 || dri->file_name.compare( TEXT( "$SysReset" ) ) == 0
			 ) continue;
		if ( dri->file_attributes & FILE_ATTRIBUTE_DIRECTORY ) {
			DirectoryLocal* dir = new DirectoryLocal();
			dir->setName( dri->file_name );
			tstring path = parent->getPath()->c_str();
			path.append( dri->file_name );
			path.append( TEXT( "\\" ) );
			dir->setPath( path.c_str() );
			pointer = dir;
			return *this;
		}
		else {
			FileLocal* fc = new FileLocal();
			fc->setName( dri->file_name );
			tstring path = parent->getPath()->c_str();
			path.append( dri->file_name );
			fc->setPath( path.c_str() );
			pointer = fc;
			return *this;
		}
	}
	return *this;
}
LocalIterator& LocalIterator::operator++( int ){return ++( *this );}
Local* LocalIterator::operator*() const noexcept {return get();}
LocalIterator::~LocalIterator() {}
Local* LocalIterator::get() const {return pointer;}
const Local* LocalIterator::getConst() const {return pointer;}