#include "stdafx.h"
#include "DriveLoader.h"


DriveLoader::DriveLoader() {
}

DriverIterator DriveLoader::begin() {
	DWORD drives_letter = GetLogicalDrives();
	iterator iter;
	iter.drives_letter = drives_letter;
	iter.indexer = 0;
	TCHAR letter = 'A';
	TCHAR buffer[ 3 ];
	for( iter.indexer = 0; iter.indexer < 27; iter.indexer++ ) {
		if ( drives_letter & ( 1 << iter.indexer ) ) {
			buffer[ 0 ] = letter + iter.indexer;
			buffer[ 1 ] = L':';
			buffer[ 2 ] = L'\0';
			if ( GetDriveType( buffer ) == DRIVE_FIXED ) {
				DriveFixed* df = new DriveFixed();
				df->setName( buffer );
				tstring path( buffer );
				path.append( TEXT( "\\" ) );
				df->setPath( path );
				iter.pointer = df;
				return iter;
			}
			else if ( GetDriveType( buffer ) == DRIVE_REMOVABLE ) {
				DriveRemovable* dr = new DriveRemovable();
				dr->setName( buffer );
				tstring path( buffer );
				path.append( TEXT( "\\" ) );
				dr->setPath( path );
				iter.pointer = dr;
				return iter;
			}
			else if ( GetDriveType( buffer ) == DRIVE_CDROM ) {
				DriveCDRom* cd = new DriveCDRom();
				cd->setName( buffer );
				tstring path( buffer );
				path.append( TEXT( "\\" ) );
				cd->setPath( path );
				iter.pointer = cd;
				return iter;
			}
			else if ( GetDriveType( buffer ) == DRIVE_UNKNOWN ) {
				DriveUnknown* du = new DriveUnknown();
				du->setName( buffer );
				tstring path( buffer );
				path.append( TEXT( "\\" ) );
				du->setPath( path );
				iter.pointer = du;
				return iter;
			}
		}
	}
	iter.pointer = nullptr;
	return iter;
}
DriverIterator DriveLoader::end(){
	return iterator( nullptr );
}

DriveLoader::~DriveLoader() {
}
