#include "stdafx.h"
#include "FileUniversalMethode.h"


SERAPHCOMMON_API std::unique_ptr<Local> loadLocalByPath( LPCTSTR path ) noexcept {
	if ( PathFileExists( path ) == FALSE ) return nullptr;
	std::unique_ptr<Local> local;
	std::vector<tstring> paths;
	size_t size = tstrSplit( path, &paths, TEXT( "\\" ) );

	if( PathIsDirectory(path) == FILE_ATTRIBUTE_DIRECTORY ) {
		if ( size == 1 ) {
			tstring drive_path = paths.begin()->c_str();
			drive_path.append( TEXT( "\\" ) );
			UINT drive_type = GetDriveType( drive_path.c_str() );
			if ( drive_type == DRIVE_FIXED ) {
				local = std::make_unique<DriveFixed>();
				( ( DriveFixed* ) local.get() )->setPath( path );
				( ( DriveFixed* ) local.get() )->setName( paths.back().c_str() );
				return local;
			}
			else if ( drive_type == DRIVE_REMOVABLE ) {
				local = std::make_unique<DriveRemovable>();
				( ( DriveRemovable* ) local.get() )->setPath( path );
				( ( DriveRemovable* ) local.get() )->setName( paths.back().c_str() );
				return local;
			}
		}
		else {
			local = std::make_unique<DirectoryLocal>();
			( ( DirectoryLocal* ) local.get() )->setPath( path );
			( ( DirectoryLocal* ) local.get() )->setName( paths.back().c_str() );
			return local;
		}
	}
	else {
		local = std::make_unique<FileLocal>();
		( ( FileLocal* ) local.get() )->setPath( path );
		( ( FileLocal* ) local.get() )->setName( paths[ paths.size() - 1 ].c_str() );
		return local;
	}
	return nullptr;
}
SERAPHCOMMON_API std::unique_ptr<Local> loadLocalByPath( tstring& path ) noexcept {
	return loadLocalByPath( path.c_str() );
}
SERAPHCOMMON_API std::unique_ptr<Local> loadLocalByPath( tstring* path ) noexcept {
	return loadLocalByPath( path->c_str() );
}
