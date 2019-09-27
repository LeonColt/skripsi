#include "stdafx.h"
#include "FileDownloadWindowDownloader.h"


FileDownloadWindowDownloader::FileDownloadWindowDownloader()
{
}

void FileDownloadWindowDownloader::onPreExecute()
{
}

LPVOID FileDownloadWindowDownloader::doInBackground(LPVOID input)
{
	wcscpy_s(error, L"");

	maschinen::wininet::HttpParams params;
	getDefaultHttpParam(&params, internet::value::request::DOWNLOAD_FILE_SEEN_BY_ALL.c_str(), akey, &uc->getPhpSessionId());

	params.add( internet::variable::VAR_REQUEST_DOWNLOAD_ID_DOWNLOAD.c_str(), download_id.c_str(), params.METHOD_POST);

	maschinen::wininet::HttpRequest comm;

	try {
		if ( sendRequest( &comm, &params ) != HTTP_STATUS_OK ) {
			tstring result;
			char buffer_mb[ MAX_LOADSTRING ];
			TCHAR buffer[ MAX_LOADSTRING ];
			DWORD number_of_byte_read = 0;
			size_t size;
			for ( auto i = comm.beginRead( buffer_mb, MAX_LOADSTRING, &number_of_byte_read ); i != comm.endRead(); i++ ) {
				buffer_mb[ number_of_byte_read ] = 0;
				size = strlen( buffer_mb );
				mbstowcs_s( &size, buffer, buffer_mb, MAX_LOADSTRING );
				result.append( buffer );
			}
			MessageBox( NULL, result.c_str(), L"Error", MB_OK | MB_ICONERROR );
		}
		TCHAR rpath[ MAX_PATH ];
		wcscpy_s( rpath, path.c_str() );
		wcscat_s( rpath, L"\\" );
		wcscat_s( rpath, file_meta_name.c_str() );
		OutputDebugStringW( L"path : " );
		OutputDebugStringW( rpath );
		OutputDebugStringW( L"\n" );
		//wcscpy_s( rpath, L"B:\\abcd.datenen" );
		maschinen::FileHandler fh( rpath, FILE_GENERIC_WRITE, 0, CREATE_NEW, FILE_ATTRIBUTE_NORMAL );
		if ( !fh.isValid() ) {
			TCHAR error[ MAX_LOADSTRING ];
			DWORD error_code;
			if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) ) {
				throw maschinen::Exception( error, error_code );
			}
			else throw maschinen::Exception();
		}
		char buffer_mb[ MAX_LOADSTRING ];
		DWORD number_of_byte_read = 0;
		DWORD number_of_byte_write = 0;
		DWORD total_read = 0;
		for ( auto i = comm.beginRead( buffer_mb, MAX_LOADSTRING, &number_of_byte_read ); i != comm.endRead(); i++ ) {
			total_read += number_of_byte_read;
			if ( number_of_byte_read == 0 ) break;
			if ( WriteFile( fh.get(), buffer_mb, number_of_byte_read, &number_of_byte_write, NULL ) == FALSE ) {
				break;
			}
		}
	} catch ( maschinen::Exception& e ) {
		TCHAR buffer[ MAX_LOADSTRING ];
		e.getMessage( buffer, MAX_LOADSTRING );

		TCHAR buffer_error_code[MAX_LOADSTRING];
		_ultow_s( e.getErrorCode(), buffer_error_code, 10 );
		wcscat_s( buffer, TEXT( "  ; Error Code : " ) );
		wcscat_s( buffer, buffer_error_code );
		SeraphException se;
		se.write( string::file::name::ERROR_LOG_NAME.c_str(), buffer );

		MessageBox( parent->getHandler(), buffer, TEXT( "Error Downloading File" ), MB_OK | MB_ICONERROR );
	} catch ( std::exception& e ) {
		TCHAR buffer[ MAX_LOADSTRING ];
		{
			size_t size = strlen( e.what() );
			mbstowcs_s( &size, buffer, e.what(), MAX_LOADSTRING );
		}
		SeraphException se;
		se.write( string::file::name::ERROR_LOG_NAME.c_str(), buffer );

		MessageBox( parent->getHandler(), buffer, TEXT( "Error Downloading File" ), MB_OK | MB_ICONERROR );
	}
	return nullptr;
}

void FileDownloadWindowDownloader::onPostExecute(LPVOID result){}
void FileDownloadWindowDownloader::setByteStart( LONGLONG byte_start ) {this->byte_start = byte_start;}
void FileDownloadWindowDownloader::setByteEnd( LONGLONG byte_end ) {this->byte_end = byte_end; }
void FileDownloadWindowDownloader::setPath(LPCTSTR path){this->path.assign( path );}
void FileDownloadWindowDownloader::setDownloadId(LPCTSTR download_id){this->download_id.assign( download_id );}
void FileDownloadWindowDownloader::setFileSize( LONGLONG file_size){this->file_size = file_size;}
void FileDownloadWindowDownloader::setFileMetaName( LPCTSTR file_meta_name ) {this->file_meta_name.assign( file_meta_name );}
FileDownloadWindowDownloader::~FileDownloadWindowDownloader(){}
