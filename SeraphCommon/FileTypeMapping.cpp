#include "stdafx.h"
#include "FileTypeMapping.h"


FileTypeMapping::FileTypeMapping() {
}

bool FileTypeMapping::exists() {
	DWORD result = GetFileAttributes( getPath()->c_str() );
	return result != INVALID_FILE_ATTRIBUTES && !( result & FILE_ATTRIBUTE_DIRECTORY );
}

void FileTypeMapping::read() {
	if ( exists() ) {
		maschinen::file::FileHandler fh( getPath()->c_str(), GENERIC_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL );
		if ( !fh.isValid() ) {
			DWORD error_code;
			TCHAR error[ MAX_LOADSTRING ];
			if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) )
				throw SeraphException( string::file::name::ERROR_LOG_NAME.c_str(), error, error_code );
			else throw SeraphException();
		}
		std::string temp = "";
		try {
			char buffer_mb[ MAX_LOADSTRING ];
			for ( auto i = fh.beginRead( buffer_mb, MAX_LOADSTRING ); i != fh.endRead(); i++ ) {
				buffer_mb[ i.number_of_byte_read ] = '\0';
				temp.append( buffer_mb );
			}
		} catch ( maschinen::exception::Exception& e ) {
			TCHAR buffer_error[ MAX_LOADSTRING ];
			e.getMessage( buffer_error, MAX_LOADSTRING );
			throw SeraphException( string::file::name::ERROR_LOG_NAME.c_str(), buffer_error, e.getErrorCode() );
		}

		std::vector<std::string> str_exp;
		strSplit( temp, &str_exp, "\r\n" );
		{
			char joiner[ MAX_LOADSTRING ] = "";
			char previous = 0;
			char out[ 2 ];
			for ( auto i = str_exp.begin(); i != str_exp.end(); i++ ) {
				Expression exp;
				for ( auto j = i->begin(); j != i->end(); j++ ) {
					if ( *j == '=' ) {
						exp.variable = joiner;
						strcpy_s( joiner, "" );
					}
					else {
						out[ 0 ] = *j;
						out[ 1 ] = '\0';
						strcat_s( joiner, out );
					}
				}
				exp.value = joiner;
				strcpy_s( joiner, "" );
				FILE_TYPES ft;
				ft.format.assign( strToWstr( exp.variable )->c_str() );
				ft.name.assign( strToWstr( exp.value )->c_str() );
				types.push_back( ft );
			}
		}
	}
}

std::unique_ptr<tstring> FileTypeMapping::getType( LPCTSTR file_name ) {
	std::vector<tstring> names;
	tstrSplit( file_name, &names, TEXT( "." ) );
	if( names.size() == 0  ) return std::make_unique<tstring>( TEXT( "File" ) );
	if( names.size() > 1 ) {
		for( auto i = types.cbegin(); i != types.cend(); ++i ) {
			bool same = true;
			auto k = i->format.begin();
			for ( auto j = names.back().begin(); j != names.back().end() && k != i->format.end() && same; ++j, ++k ) {
				if ( *j == *k ) same = false;
			}
			if( same ) return std::make_unique<tstring>( i->name.c_str() );
		}
	}
	OutputDebugStringW( TEXT( "\n" ) );
	return std::make_unique<tstring>( TEXT( "File" ) );
}

std::unique_ptr<tstring> FileTypeMapping::getPath() {
	std::unique_ptr<tstring> path = std::make_unique<tstring>();
	path->append( getDefaultAppDataPath( true )->c_str() );
	path->append( string::file::name::FILE_TYPE_MAPPING.c_str() );
	return path;
}

FileTypeMapping::~FileTypeMapping() {
}
