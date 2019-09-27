#include "stdafx.h"
#include "DriverIterator.h"


DriverIterator::DriverIterator( Drive* pointer ) {this->pointer = pointer;}

DriverIterator& DriverIterator::operator=( Drive* pointer ) {
	this->pointer = pointer;
	return ( *this );
}
bool DriverIterator::operator==( DriverIterator& pointer ) const { return this->pointer == pointer.getConst(); }
bool DriverIterator::operator!=( DriverIterator& pointer ) const { return this->pointer != pointer.getConst(); }
DriverIterator& DriverIterator::operator++() {
	indexer++;
	TCHAR letter = 'A';
	TCHAR buffer[ 3 ];
	for( ; indexer < 27; indexer++ ) {
		if ( drives_letter & ( 1 << indexer ) ) {
			buffer[ 0 ] = letter + indexer;
			buffer[ 1 ] = L':';
			buffer[ 2 ] = L'\0';
			if ( GetDriveType( buffer ) == DRIVE_FIXED ) {
				DriveFixed* df = new DriveFixed();
				df->setName( buffer );
				tstring path( buffer );
				path.append( TEXT( "\\" ) );
				df->setPath( path );
				pointer = df;
				return *this;
			}
			else if ( GetDriveType( buffer ) == DRIVE_REMOVABLE ) {
				DriveRemovable* dr = new DriveRemovable();
				dr->setName( buffer );
				tstring path( buffer );
				path.append( TEXT( "\\" ) );
				dr->setPath( path );
				pointer = dr;
				return *this;
			}
			else if ( GetDriveType( buffer ) == DRIVE_CDROM ) {
				DriveCDRom* cd = new DriveCDRom();
				cd->setName( buffer );
				tstring path( buffer );
				path.append( TEXT( "\\" ) );
				cd->setPath( path );
				pointer = cd;
				return *this;
			}
			else if ( GetDriveType( buffer ) == DRIVE_UNKNOWN ) {
				DriveUnknown* du = new DriveUnknown();
				du->setName( buffer );
				tstring path( buffer );
				path.append( TEXT( "\\" ) );
				du->setPath( path );
				pointer = du;
				return *this;
			}
		}
	}
	pointer = nullptr;
	return *this;
}
DriverIterator& DriverIterator::operator++( int ) {return ++( *this );}
Drive* DriverIterator::operator*() const noexcept {return get();}
Drive* DriverIterator::operator->() const noexcept {return get();}
DriverIterator::~DriverIterator() {}
Drive* DriverIterator::get() const {return pointer;}
const Drive* DriverIterator::getConst() const {return pointer;}
