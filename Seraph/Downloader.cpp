#include "stdafx.h"
#include "Downloader.h"


Downloader::Downloader(const DWORD buffer_size) : BUFFER_SIZE(buffer_size) {}
void Downloader::onPreExecute() {flag_pause = false;}

LPVOID Downloader::doInBackground( LPVOID input ) {
	DownloaderResult* rs = new DownloaderResult();
	std::vector<MetaDataDownloadBaseFile*> files;
	meta_data->filesToVector( files );
	ULONGLONG total_read = meta_data->getByteWritten();
	DWORD number_of_byte_read, number_of_byte_write;
	time_t current_time_for_speed = 0, before_time_for_speed = 0;
	try {
		for ( auto i = files.begin(); i != files.end(); i++ ) {
			if ( MetaDataDownloadDirectory* dir = dynamic_cast< MetaDataDownloadDirectory* >( *i ) ) {
				tstring path( meta_data->getTargetPath()->c_str() ); path.append( TEXT( "\\" ) );
				path.append( dir->getPath()->c_str() );
				{
					tstring status( TEXT( "Creating Directory " ) );
					status.append( path.c_str() );
					( ( FileProcessWindow* ) parent )->setStatus( status.c_str() );
				}
				DWORD attributes = GetFileAttributes( path.c_str() );
				if ( attributes == INVALID_FILE_ATTRIBUTES ) {
					if ( CreateDirectoryW( path.c_str(), NULL ) == 0 ) {
						TCHAR buffer[ MAX_LOADSTRING ];
						DWORD error_code;
						if ( getLastErrorAsString( buffer, MAX_LOADSTRING, error_code ) )
							throw SeraphException( string::file::name::ERROR_LOG_NAME.c_str(), buffer, error_code );
						else throw SeraphException( string::file::name::ERROR_LOG_NAME.c_str() );
					}
				}
			}
			else if ( MetaDataDownloadFile* file = dynamic_cast< MetaDataDownloadFile* >( *i ) ) {
				{
					tstring status( TEXT( "Downloading " ) );
					status.append( file->getPath()->c_str() );
					( ( FileProcessWindow* ) parent )->setStatus( status.c_str() );
				}
				tstring path( meta_data->getTargetPath()->c_str() ); path.append( TEXT( "\\" ) );
				path.append( file->getPath()->c_str() );
				maschinen::wininet::HttpParams params;
				getDefaultHttpParam( params, &uc->getPhpSessionId() );
				params.add( DOWNLOAD_ID, meta_data->getId()->c_str(), params.METHOD_POST );
				params.add( FILE_ID, to_tstring( file->getId() ).c_str(), params.METHOD_POST );
				{
					maschinen::file::FileHandler checker( path.c_str(), FILE_GENERIC_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL );
					if ( checker.isValid() ) {
						if ( checker.getSize() > meta_data->getByteWritten() ) {
							if ( DeleteFileW( path.c_str() ) ) {
								TCHAR error[ MAX_LOADSTRING ];
								DWORD error_code;
								if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) )
									throw SeraphException( string::file::name::ERROR_LOG_NAME.c_str(), error, error_code );
								else throw SeraphException( string::file::name::ERROR_LOG_NAME.c_str() );
							}
						}
						params.add( internet::variable::FILE_DOWNLOAD_START.c_str(), to_tstring( checker.getSize() ).c_str(), params.METHOD_POST );
					}
				}
				//params.add( internet::variable::FILE_DOWNLOAD_END.c_str(), to_tstring( byte_end ).c_str(), params.METHOD_POST );
				/*
				tidak digunakan, digunakan, jika download difragment atau dipecah-pecah
				*/
				maschinen::wininet::HttpRequest comm;
				if ( sendRequest( setting->getServerName()->c_str(), setting->getUrl()->c_str(), &comm, internet::value::request::DOWNLOAD_FILE_SEEN_BY_ALL, akey->getAppKey()->c_str(), &params ) != HTTP_STATUS_OK ) {
					tstring result;
					char buffer_mb[ MAX_LOADSTRING ];
					DWORD number_of_byte_read = 0;
					for ( auto i = comm.beginRead( buffer_mb, MAX_LOADSTRING - 1, &number_of_byte_read ); i != comm.endRead(); i++ ) {
						buffer_mb[ number_of_byte_read ] = 0;
						result.append( strToWstr(buffer_mb)->c_str() );
					}
					rs->status = rs->STATUS_ERROR;
					rs->message = result.c_str();
					return rs;
				}
				maschinen::file::FileHandler fh;
				if ( GetFileAttributes( path.c_str() ) == INVALID_FILE_ATTRIBUTES )
					fh.init( path.c_str(), FILE_GENERIC_WRITE, 0, CREATE_NEW, FILE_ATTRIBUTE_NORMAL );
				else fh.init( path.c_str(), FILE_APPEND_DATA, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL );
				if ( !fh.isValid() ) {
					TCHAR error[ MAX_LOADSTRING ];
					DWORD error_code;
					if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) )
						throw SeraphException( string::file::name::ERROR_LOG_NAME.c_str(), error, error_code );
					else throw SeraphException( string::file::name::ERROR_LOG_NAME.c_str() );
				}
				std::unique_ptr<BYTE[]> buffer_mb = std::make_unique<BYTE[]>( BUFFER_SIZE );
				ULONGLONG current_downloaded_progress = 0, before_downloaded_progress = 0, total_downloaded_speed = 0;
				_time64( &current_time_for_speed ); before_time_for_speed = current_time_for_speed;
				for ( auto read = comm.beginRead( buffer_mb.get(), BUFFER_SIZE, &number_of_byte_read ); read != comm.endRead(); read++ ) {
					total_read += number_of_byte_read;
					if ( number_of_byte_read > 0 ) 
						fh.write( buffer_mb.get(), number_of_byte_read, &number_of_byte_write );
					//begin progress download
					current_downloaded_progress = fileSizeToProperSize( total_read );
					if ( current_downloaded_progress != before_downloaded_progress )
						( ( FileProcessWindow* ) parent )->setDataDownloaded( total_read );
					before_downloaded_progress = current_downloaded_progress;
					//begin speed download
					_time64( &current_time_for_speed );
					if ( current_time_for_speed - before_time_for_speed >= 1 ) {
						total_downloaded_speed += number_of_byte_read;
						( ( FileProcessWindow* ) parent )->setDownloadSpeed( total_downloaded_speed, current_time_for_speed - before_time_for_speed );
						( ( FileProcessWindow* ) parent )->setRemainingTime( total_read, total_downloaded_speed / ( current_time_for_speed - before_time_for_speed ) );
						before_time_for_speed = current_time_for_speed;
						total_downloaded_speed = 0;
					}
					else total_downloaded_speed += number_of_byte_read;
					if ( flag_pause ) {
						i = files.end() - 1;
						break;
					}
				}
				( ( FileProcessWindow* ) parent )->setDownloadSpeed( 0, 1 );
				( ( FileProcessWindow* ) parent )->setRemainingTime( total_read, 0 );
				if ( total_read < meta_data->getSize() ) rs->status = rs->STATUS_PAUSE;
				else {
					rs->status = rs->STATUS_FINISH;
				}
			}
		}
	} catch ( SeraphException& e ) {
		rs->status = rs->STATUS_ERROR;
		rs->message = e.getMessage()->c_str();
	} catch ( maschinen::exception::Exception& e ) {
		tstring error( e.getMessage()->c_str() );
		error.append( TEXT( "  ; Error Code : " ) );
		error.append( to_tstring( e.getErrorCode() ).c_str() );
		SeraphException se;
		se.write( string::file::name::ERROR_LOG_NAME.c_str(), error.c_str() );
		rs->status = rs->STATUS_ERROR;
		rs->message = error.c_str();
	} catch ( std::exception& e ) {
		SeraphException se;
		se.write( string::file::name::ERROR_LOG_NAME.c_str(), strToWstr( e.what() )->c_str() );
		rs->status = rs->STATUS_ERROR;
		rs->message = strToWstr( e.what() )->c_str();
	}
	return rs;
}

void Downloader::onPostExecute( LPVOID result ) {
	std::unique_ptr<DownloaderResult> rs(( DownloaderResult* ) result);
	if ( rs->status == rs->STATUS_FINISH ) {
		MessageBox( parent->getHandler(), TEXT( "Files have been downloaded" ), L"Error Downloading file", MB_ICONERROR | MB_OK );
		( ( FileProcessWindow* ) parent )->finishMode();
		( ( FileProcessWindow* ) parent )->hide();
	}
	else if ( rs->status == rs->STATUS_PAUSE ) ( ( FileProcessWindow* ) parent )->pauseMode();
	else if ( rs->status == rs->STATUS_ERROR )  {
		MessageBox( parent->getHandler(), rs->message.c_str(), L"Error Downloading file", MB_ICONERROR | MB_OK );
		( ( FileProcessWindow* ) parent )->pauseMode();
	}
}

void Downloader::setMetaData( MetaDataDownload* meta_data ) {
	this->meta_data = meta_data;
}

void Downloader::pause() {
	flag_pause = true;
}

Downloader::~Downloader() {
}
