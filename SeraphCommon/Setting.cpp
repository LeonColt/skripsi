#include "stdafx.h"
#include "Setting.h"


Setting::Setting() {
	log_time_type = 2; log_url = log_post = log_result = true;
	show_window_download_after_create_new_download = true;
	show_window_upload_after_create_new_upload = true;
	jumlah_cell_per_panjang = DEFAULT_JUMLAH_CELL_PER_PANJANG;
	jumlah_cell_per_lebar = DEFAULT_JUMLAH_CELL_PER_LEBAR;
	max_size_uploaded_file_chunked = 5242880;
	server_name.assign( TEXT( "leoncolt.xyz" ) );
	url.assign( TEXT( "seraph/" ) );
	cloud_root_name.assign( TEXT( "Cloud:" ) );
	cloud_bin_root_name.assign( TEXT( "CloudRecycleBin:" ) );
}

bool Setting::exists() {
	DWORD result = GetFileAttributes( getPath()->c_str() );
	return result != INVALID_FILE_ATTRIBUTES && !( result & FILE_ATTRIBUTE_DIRECTORY );
}

void Setting::create() {
	if ( !exists() ) {
		maschinen::file::FileHandler fh( getPath()->c_str(), GENERIC_ALL, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL );
		if ( !fh.isValid() ) {
			DWORD error_code;
			TCHAR error[ MAX_LOADSTRING ];
			if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) )
				throw SeraphException( string::file::name::ERROR_LOG_NAME.c_str(), error, error_code );
			else throw SeraphException();
		}
	}
}

void Setting::read() {
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
		std::vector<Expression> exps;
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
				exps.push_back( exp );
			}
		}

		for ( auto i = exps.begin(); i != exps.end(); i++ ) {
			if ( strcmp( i->variable.c_str(), "log_time_type" ) == 0 ) log_time_type = atoi( i->value.c_str() );
			else if ( strcmp( i->variable.c_str(), "log_url" ) == 0 ) {
				if ( strcmp( i->value.c_str(), "false" ) == 0 ) log_url = false;
			}
			else if ( strcmp( i->variable.c_str(), "log_post" ) == 0 ) {
				if ( strcmp( i->value.c_str(), "false" ) == 0 ) log_post = false;
			}
			else if ( strcmp( i->variable.c_str(), "log_result" ) == 0 ) {
				if ( strcmp( i->value.c_str(), "false" ) == 0 ) log_result = false;
			}
			else if ( strcmp( i->variable.c_str(), "show_window_download_after_create_new_download" ) == 0 ) {
				if ( strcmp( i->value.c_str(), "false" ) == 0 ) show_window_download_after_create_new_download = false;
			}
			else if ( strcmp( i->variable.c_str(), "show_window_upload_after_create_new_upload" ) == 0 ) {
				if ( strcmp( i->value.c_str(), "false" ) == 0 ) show_window_upload_after_create_new_upload = false;
			}
			else if ( i->variable.compare( "max_file_size_uploaded_file_chunked(in bytes)" ) == 0 ) {
				max_size_uploaded_file_chunked = _strtoui64( i->value.c_str(), nullptr, 10 );
				if ( max_size_uploaded_file_chunked == 0 ) max_size_uploaded_file_chunked = 5242880;
			}
			else if ( i->variable.compare( "server_name" ) == 0 )
				server_name.assign( strToWstr( i->value.c_str() )->c_str() );
			else if ( i->variable.compare( "url" ) == 0 )
				url.assign( strToWstr( i->value.c_str() )->c_str() );
			else if ( i->variable.compare( "cloud_root_name" ) == 0 )
				cloud_root_name.assign( strToWstr( i->value.c_str() )->c_str() );
			else if ( i->variable.compare( "cloud_bin_root_name" ) == 0 )
				cloud_bin_root_name.assign( strToWstr( i->value.c_str() )->c_str() );
		}
	}
}

void Setting::write() {
	maschinen::file::FileHandler fh( getPath()->c_str(), GENERIC_WRITE, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL );
	if ( !fh.isValid() ) {
		DWORD error_code;
		TCHAR error[ MAX_LOADSTRING ];
		char error_mb[ MAX_LOADSTRING ];
		if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) ) {
			size_t size = wcslen( error );
			wcstombs_s( &size, error_mb, error, MAX_LOADSTRING );
			throw std::exception( error_mb, ( int ) error_code );
		}
		else throw std::exception( "an unknown error occured" );
	}
	std::vector<Expression> exp;
	Expression s_log_time_type;
	s_log_time_type.variable = "log_time_type";
	s_log_time_type.value.assign( std::to_string( log_time_type ).c_str() );
	exp.push_back( s_log_time_type );

	Expression s_log_url;
	s_log_url.variable = "log_url";
	s_log_url.value.assign( log_url ? "true" : "false" );
	exp.push_back( s_log_url );

	Expression s_log_post;
	s_log_post.variable = "log_post";
	s_log_post.value = ( log_post ? "true" : "false" );
	exp.push_back( s_log_post );

	Expression s_log_result;
	s_log_result.variable = "log_result";
	s_log_result.value = log_result ? "true" : "false";
	exp.push_back( s_log_result );

	Expression s_show_window_download_after_create_new_download;
	s_show_window_download_after_create_new_download.variable = "show_window_download_after_create_new_download";
	s_show_window_download_after_create_new_download.value = show_window_download_after_create_new_download ? "true" : "false";
	exp.push_back( s_show_window_download_after_create_new_download );

	Expression s_show_window_upload_after_create_new_upload;
	s_show_window_upload_after_create_new_upload.variable = "show_window_upload_after_create_new_upload";
	s_show_window_upload_after_create_new_upload.value = show_window_upload_after_create_new_upload ? "true" : "false";
	exp.push_back( s_show_window_upload_after_create_new_upload );

	Expression ex_max_file_size_uploaded_file_chunked;
	ex_max_file_size_uploaded_file_chunked.variable = "max_file_size_uploaded_file_chunked(in bytes)";
	ex_max_file_size_uploaded_file_chunked.value = std::to_string( max_size_uploaded_file_chunked );
	exp.push_back( ex_max_file_size_uploaded_file_chunked );

	Expression ex_server_name;
	ex_server_name.variable = "server_name";
	ex_server_name.value.assign( wstrToStr(server_name.c_str())->c_str() );
	exp.push_back( ex_server_name );

	Expression ex_url;
	ex_url.variable = "url";
	ex_url.value.assign( wstrToStr( url.c_str() )->c_str() );
	exp.push_back( ex_url );

	Expression ex_crn;
	ex_crn.variable = "cloud_root_name";
	ex_crn.value.assign( wstrToStr( cloud_root_name.c_str() )->c_str() );
	exp.push_back( ex_crn );

	Expression ex_crbn;
	ex_crbn.variable = "cloud_bin_root_name";
	ex_crbn.value.assign( wstrToStr( cloud_bin_root_name.c_str() )->c_str() );
	exp.push_back( ex_crbn );

	std::string temp = "";
	for ( auto i = exp.begin(); i != exp.end(); i++ ) {
		temp.append( i->variable );
		temp.append( "=" );
		temp.append( i->value );
		temp.append( "\r\n" );
	}

	DWORD number_of_written = 0;

	if ( WriteFile( fh.get(), temp.c_str(), ( DWORD ) strlen( temp.c_str() ), &number_of_written, NULL ) == FALSE ) {
		DWORD error_code;
		TCHAR error[ MAX_LOADSTRING ];
		char error_mb[ MAX_LOADSTRING ];
		if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) ) {
			size_t size = wcslen( error );
			wcstombs_s( &size, error_mb, error, MAX_LOADSTRING );
			throw std::exception( error_mb, ( int ) error_code );
		}
		else throw std::exception( "an unknown error occured" );
	}
}

std::unique_ptr<tstring> Setting::getPath() {
	std::unique_ptr<tstring> path = std::make_unique<tstring>();
	path->append( getDefaultAppDataPath( true )->c_str() );
	path->append( string::file::name::SETTING.c_str() );
	return path;
}

Setting::~Setting() {
}

void Setting::setServerName( LPCTSTR server_name ) {this->server_name = server_name;}
void Setting::setServerName( tstring& server_name ) {this->server_name.assign( server_name.c_str() );}
void Setting::setServerName( tstring* server_name ) {this->server_name.assign( server_name->c_str() );}
void Setting::setUrl( LPCTSTR url ) { this->url = url; }
void Setting::setUrl( tstring& url ) { this->url.assign( url.c_str() ); }
void Setting::setUrl( tstring* url ) { this->url.assign( url->c_str() ); }
void Setting::setCloudRootName( LPCTSTR cloud_root_name ) {this->cloud_root_name.assign( cloud_root_name );}
void Setting::setCloudRootName( tstring& cloud_root_name ){this->cloud_root_name.assign( cloud_root_name.c_str() );}
void Setting::setCloudRootName( tstring* cloud_root_name ){this->cloud_root_name.assign( cloud_root_name->c_str() );}
void Setting::setCloudRecycleBinRootName( LPCTSTR cloud_bin_root_name ){ this->cloud_bin_root_name.assign( cloud_bin_root_name ); }
void Setting::setCloudRecycleBinRootName( tstring& cloud_bin_root_name ){ this->cloud_bin_root_name.assign( cloud_bin_root_name.c_str() ); }
void Setting::setCloudRecycleBinRootName( tstring* cloud_bin_root_name ){ this->cloud_bin_root_name.assign( cloud_bin_root_name->c_str() ); }
std::unique_ptr<tstring> Setting::getServerName() {return std::make_unique<tstring>( server_name.c_str() );}
std::unique_ptr<tstring> Setting::getUrl() {return std::make_unique<tstring>( url.c_str() );}
std::unique_ptr<tstring> Setting::getCloudRootName() { return std::make_unique<tstring>( cloud_root_name.c_str() ); }
std::unique_ptr<tstring> Setting::getCloudRecycleBinRootName() { return std::make_unique<tstring>( cloud_bin_root_name.c_str() ); }