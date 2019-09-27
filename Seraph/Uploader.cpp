#include "stdafx.h"
#include "Uploader.h"


Uploader::Uploader(const DWORD buffer_size) : BUFFER_SIZE(buffer_size) {
}
void Uploader::onPreExecute() {flag_pause = false;}
LPVOID Uploader::doInBackground( LPVOID input ) {
	UploaderResult* rs = new UploaderResult();
	try {
		QWORD total_uploaded = ( ( FileUploadProcessWindow* ) parent )->getTotalUploaded();
		int counter = 0;
		for ( auto i = meta_data->begin(); i != meta_data->end() && !flag_pause; ++i ) {
			if ( i->isDirectory() ) continue;
			std::unique_ptr<LARGE_INTEGER> file_uploaded_size = getFileSizeUploaded( *i );
			if ( file_uploaded_size->QuadPart >= i->getSize() ) continue;
			maschinen::wininet::HttpParams params;
			getDefaultHttpParam( &params, &uc->getPhpSessionId() );
			tstring s_status( TEXT( "Uploading " ) );
			s_status.append( i->getName()->c_str() );
			( ( FileUploadProcessWindow* ) parent )->setStatus( s_status );
			//check if file exists
			if ( GetFileAttributes( i->getPath()->c_str() ) == INVALID_FILE_ATTRIBUTES ) {
				rs->status = rs->STATUS_ERROR;
				rs->message = TEXT( "File does not exist" );
				return rs;
			}
			maschinen::wininet::HttpRequest comm;
			comm.open( L"", comm.ACCESS_TYPE_PRE_CONFIG );
			comm.connect( setting->getServerName()->c_str() );
			comm.openRequest( setting->getUrl()->c_str(), comm.VERSION_HTTP_11, NULL, NULL,
							  INTERNET_FLAG_PRAGMA_NOCACHE |
							  INTERNET_FLAG_NO_CACHE_WRITE |
							  INTERNET_FLAG_NO_COOKIES |
							  INTERNET_FLAG_NO_UI |
							  INTERNET_FLAG_NO_AUTO_REDIRECT |
							  INTERNET_FLAG_RELOAD, &params );
			comm.setRequestTimeOut( comm.REQUEST_TIME_OUT_INFINITE );
			comm.setReceiveTimeOut( 950000 );
			tstring boundary = L"Bound8439ifsjfjsiejifsfe38578348357";
			//tstring boundary = L"Bound_";
			//maschinen::crypt::Crypt crypt;
			//boundary.append( crypt.randomAlphaNumeric( 25 )->c_str() );

			std::string boundary_mb( wstrToStr( boundary )->c_str() );
			std::string head = "--";
			head.append( boundary_mb );
			head.append( "\r\nContent-Type: application/octet-stream" );
			head.append( "\r\nContent-Disposition: form-data; filename=\"" );
			head.append( wstrToStr( i->getName()->c_str() )->c_str() );
			head.append( "\"; name=\"" );
			head.append( THE_FILE );
			head.append( "\"\r\n\r\n" );

			std::string tail = "\r\n--";
			tail.append( boundary_mb );
			tail.append( "--\r\n" );

			maschinen::file::FileHandler fh( i->getPath()->c_str(), GENERIC_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL );
			if ( file_uploaded_size->QuadPart > 0 ) fh.setPointer( *file_uploaded_size.get() );
			{
				std::unique_ptr<LARGE_INTEGER> file_size = fh.getSizeEx();
				std::unique_ptr<BYTE[]> buffer = std::make_unique<BYTE[]>( BUFFER_SIZE );
				const DWORD MAX_CHUNKED_SIZE = ( DWORD ) setting->getMaxFileSizeUploadChunked();
				bool flag_new_chunk = true;
				DWORD chunk_sent_counter = 0;
				DWORD number_of_written;
				DWORD chunked_size = 0;
				DWORD total_speed = 0;
				time_t current_time_for_speed = 0, before_time_for_speed = 0;
				_time64( &current_time_for_speed ); before_time_for_speed = current_time_for_speed;
				for ( auto j = fh.beginRead( buffer.get(), BUFFER_SIZE ); j != fh.endRead(); j++ ) {
					if ( chunk_sent_counter == 0 ) {
						chunked_size = ( DWORD ) ( file_size.get()->QuadPart - ( LONGLONG ) file_uploaded_size->QuadPart );
						if ( chunked_size > MAX_CHUNKED_SIZE ) chunked_size = MAX_CHUNKED_SIZE;

						std::string temp;
						appendSendingData( temp, boundary_mb, chunked_size, *i );

						maschinen::wininet::HttpHeaders headers;
						tstring content_type_val = TEXT( "multipart/form-data;" );
						content_type_val.append( TEXT( "boundary=" ) );
						content_type_val.append( boundary );
						headers.add( TEXT( "Content-Type" ), content_type_val.c_str() );
						if ( akey != nullptr ) headers.add( internet::variable::VAR_APP_KEY, akey->getAppKey()->c_str() );
						headers.add( internet::variable::VAR_REQUEST, internet::value::request::upload::EXECUTE );
						headers.add( internet::variable::VAR_REQUEST_TYPE, internet::value::REQUEST_TYPE );
						headers.add( internet::variable::VAR_APP_TYPE, internet::value::APP_TYPE );
						headers.add( TEXT( "Content-Length:" ), to_tstring( ( DWORD ) ( ( temp.length() ) +
							( head.length() ) + chunked_size + ( tail.length() ) ) ).c_str() );

						comm.beginRequest( headers, NULL, NULL, NULL, ( DWORD ) ( ( temp.length() ) +
							( head.length() ) + chunked_size + ( tail.length() ) ) );
						comm.write( ( LPVOID ) temp.c_str(), ( DWORD ) temp.length(), &number_of_written );
						comm.write( ( LPVOID ) head.c_str(), ( DWORD ) ( head.length() ), &number_of_written );
					}

					comm.write( buffer.get(), j.number_of_byte_read, &number_of_written );
					total_uploaded += ( QWORD ) number_of_written;
					file_uploaded_size->QuadPart += ( LONGLONG ) number_of_written;
					chunk_sent_counter += number_of_written;
					_time64( &current_time_for_speed );
					if ( current_time_for_speed - before_time_for_speed >= 1 ) {
						( ( FileUploadProcessWindow* ) parent )->setTotalUploaded( total_uploaded );
						total_speed += number_of_written;
						( ( FileUploadProcessWindow* ) parent )->setSpeed( total_speed, current_time_for_speed - before_time_for_speed );
						( ( FileUploadProcessWindow* ) parent )->setRemainingTime( total_uploaded, total_speed / ( current_time_for_speed - before_time_for_speed ) );
						before_time_for_speed = current_time_for_speed;
						total_speed = 0;
					}
					else total_speed += number_of_written;

					if ( chunk_sent_counter >= chunked_size ) {
						comm.write( ( LPVOID ) tail.c_str(), ( DWORD ) ( tail.length() ), &number_of_written );
						comm.endRequest();
						chunk_sent_counter = 0;

						if ( comm.getStatusCode() != HTTP_STATUS_OK ) {
							tstring result;
							char buffer_mb[ MAX_LOADSTRING ];
							DWORD number_of_byte_read = 0;
							size_t size;
							for ( auto j = comm.beginRead( buffer_mb, MAX_LOADSTRING - 1, &number_of_byte_read ); j != comm.endRead(); j++ ) {
								buffer_mb[ number_of_byte_read ] = 0;
								size = strlen( buffer_mb );
								result.append( strToWstr( buffer_mb )->c_str() );
							}
							rs->status = rs->STATUS_ERROR;
							rs->message = result.c_str();
							return rs;
						}
						else {
							tstring result = L"";
							char buffer_mb[ MAX_LOADSTRING ];
							DWORD number_of_byte_read = 0;
							for ( auto j = comm.beginRead( buffer_mb, MAX_LOADSTRING - 1, &number_of_byte_read ); j != comm.endRead(); j++ ) {
								buffer_mb[ number_of_byte_read ] = 0;
								result.append( strToWstr( buffer_mb )->c_str() );
							}
							Poco::JSON::Object::Ptr json = deserialization( buffer_mb );
							if ( !json->has( VAR_JSON_STATUS ) ) throw new SeraphException( string::file::name::ERROR_LOG_NAME.c_str(), "Upload File Status Error", 0 );
							if ( json->get( VAR_JSON_STATUS ).extract<int>() == 0 ) {
								if ( json->has( VAR_JSON_MESSAGE ) ) {
									rs->status = rs->STATUS_ERROR;
									rs->message = strToWstr( json->get( VAR_JSON_MESSAGE ).extract<std::string>().c_str() )->c_str();
									return rs;
								}
								else {
									rs->status = rs->STATUS_ERROR;
									rs->message = TEXT( "Upload Error" );
								}
							}
						}

					}
					if ( flag_pause ) break;
				}
			}
			if ( flag_pause ) break;
		}
		if ( total_uploaded < meta_data->getSize() ) rs->status = rs->STATUS_PAUSE;
		else {
			tstring status( TEXT( "Finishing Upload " ) );
			status.append( meta_data->getId()->c_str() );
			( ( FileUploadProcessWindow* ) parent )->setStatus( status );
			for ( ;; ) {
				if ( flag_pause ) break;
				if ( reportFileUploadFinish() ) {
					MessageBox( parent->getHandler(), TEXT( "Files have been uploaded" ), TEXT( "File Upload" ), MB_OK | MB_ICONINFORMATION );
					rs->status = rs->STATUS_FINISH;
					return rs;
				}
			}
			rs->status = rs->STATUS_PAUSE;
		}
	}  catch (SeraphException& e ) {
		rs->status = rs->STATUS_ERROR;
		rs->message = e.getMessage()->c_str();
	} catch ( maschinen::exception::Exception& e ) {
		TCHAR buffer[ MAX_LOADSTRING ];
		e.getMessage( buffer, MAX_LOADSTRING );

		TCHAR buffer_error_code[ MAX_LOADSTRING ];
		_ultow_s( e.getErrorCode(), buffer_error_code, 10 );
		wcscat_s( buffer, TEXT( "  ; Error Code : " ) );
		wcscat_s( buffer, buffer_error_code );
		SeraphException se;
		se.write( string::file::name::ERROR_LOG_NAME.c_str(), buffer );

		rs->status = rs->STATUS_ERROR;
		rs->message = buffer;
		return rs;
	} catch ( std::exception& e ) {
		SeraphException se;
		se.write( string::file::name::ERROR_LOG_NAME.c_str(), strToWstr( e.what() )->c_str() );
		rs->status = rs->STATUS_ERROR;
		rs->message = strToWstr( e.what() )->c_str();
		return rs;
	}
	return rs;
}

void Uploader::onPostExecute( LPVOID result ) {
	std::unique_ptr<UploaderResult> ur( ( UploaderResult* ) result );
	if ( ur->status == ur->STATUS_FINISH ) ( ( FileUploadProcessWindow* ) parent )->finishMode();
	else if ( ur->status == ur->STATUS_PAUSE ) ( ( FileUploadProcessWindow* ) parent )->pauseMode();
	else if ( ur->status == ur->STATUS_ERROR ) {
		( ( FileUploadProcessWindow* ) parent )->pauseMode();
		MessageBox( NULL, ur->message.c_str(), L"Error", MB_OK | MB_ICONERROR );
	}
	( ( FileUploadProcessWindow* ) parent )->enableButtonStartPause();
}

void Uploader::setMetaData( MetaDataUpload* meta_data ) {this->meta_data = meta_data;}

void Uploader::pause() {
	flag_pause = true;
}

Uploader::~Uploader() {
}

bool Uploader::reportFileUploadFinish() {
	maschinen::wininet::HttpParams params;
	getDefaultHttpParam( params, &uc->getPhpSessionId() );
	params.add( internet::value::request::upload_file_finish::VAR_ID.c_str(), meta_data->getId()->c_str(), params.METHOD_POST );
	if ( uc->isCredential() ) params.add( FileSize.LOGIN_ID, uc->getLoginId()->c_str(), params.METHOD_POST );
	ATRequestAndParamCustomInternetJson atr;
	atr.request = &internet::value::request::upload::END;
	atr.params = &params;
	std::unique_ptr<tstring> result( ( ( tstring* ) CustomInternetJSONAsyncTask::doInBackground( &atr ) ) );
	try {
		Poco::JSON::Object::Ptr json = deserialization( result->c_str() );
		if ( !json->has( VAR_JSON_STATUS ) ) throw SeraphException( string::file::name::ERROR_LOG_NAME.c_str(), TEXT( "Invalid json " ), 0 );
		if ( json->get( VAR_JSON_STATUS ).extract<int>() == 1 ) {
			meta_data->deleteCache();
			meta_data->deleteMetaDataFile();
			( ( FileUploadProcessWindow* ) parent )->getListering()->updateListView();
			return true;
		}
		else {
			if ( json->has( VAR_JSON_MESSAGE ) )
				MessageBox( NULL, strToWstr( json->get( VAR_JSON_MESSAGE ).extract<std::string>().c_str() )->c_str(), TEXT("Upload Error"), MB_OK | MB_ICONERROR );
			else MessageBox( NULL, strToWstr( json->get( VAR_JSON_MESSAGE ).extract<std::string>().c_str() )->c_str(), TEXT( "Upload Error" ), MB_OK | MB_ICONERROR );
		}
	} catch ( SeraphException& e ) {
		throw e;

	} catch ( maschinen::exception::Exception& e ) {
		throw e;
	} catch ( std::exception& e ) {
		throw e;
	}
	return false;
}

std::unique_ptr<LARGE_INTEGER> Uploader::getFileSizeUploaded( const MetaDataUploadDetail& mdud ) {
	tstring s_status( TEXT( "retrive " ) ); s_status.append( mdud.getName()->c_str() );
	s_status.append( TEXT( " uploaded size" ) );
	( ( FileUploadProcessWindow* ) parent )->setStatus( s_status );
	std::unique_ptr<LARGE_INTEGER> lr = std::make_unique<LARGE_INTEGER>();
	lr->QuadPart = 0;
	maschinen::wininet::HttpParams params;
	getDefaultHttpParam( params, &uc->getPhpSessionId() );
	params.add( FileSize.ID, meta_data->getId()->c_str(), params.METHOD_POST );
	params.add( FileSize.FILE_ID, to_tstring(mdud.getId()).c_str(), params.METHOD_POST );
	if ( uc->isCredential() ) params.add( FileSize.LOGIN_ID, uc->getLoginId()->c_str(), params.METHOD_POST );
	ATRequestAndParamCustomInternetJson atr;
	atr.request = &internet::value::request::upload::GET_SIZE_PER_FILE_ID;
	atr.params = &params;
	std::unique_ptr<tstring> result( ( (tstring*) CustomInternetJSONAsyncTask::doInBackground( &atr ) ) );
	try {
		Poco::JSON::Object::Ptr json = deserialization( result->c_str() );
		if ( !json->has( VAR_JSON_STATUS ) ) throw SeraphException( string::file::name::ERROR_LOG_NAME.c_str(), TEXT( "Invalid json " ), 0 );
		if ( json->get( VAR_JSON_STATUS ).extract<int>() == 1 ) {
			if ( json->has( VAR_JSON_RESULT ) ) 
				lr->QuadPart = _strtoi64( json->get( VAR_JSON_RESULT ).extract<std::string>().c_str(), nullptr, 10 );
			else throw SeraphException( string::file::name::ERROR_LOG_NAME.c_str(), TEXT( "Invalid json " ), 0 );
		}
		else {
			if ( json->has( VAR_JSON_MESSAGE ) ) throw SeraphException( string::file::name::ERROR_LOG_NAME.c_str(), strToWstr( json->get( VAR_JSON_MESSAGE ).extract<std::string>() )->c_str(), 0);
			else throw SeraphException( string::file::name::ERROR_LOG_NAME.c_str(), TEXT("An Error Occured"), 0 );
		}
	} catch ( SeraphException& e ) {
		throw e;
	} catch ( maschinen::exception::Exception& e ) {
		throw e;
	} catch ( std::exception& e ) {
		throw e;
	}
	( ( FileUploadProcessWindow* ) parent )->setStatusDefault();
	return lr;
}

void Uploader::appendSendingData( std::string& data, std::string& boundary, QWORD size, const MetaDataUploadDetail& mdud ) {
	data.assign( "" );
	//begin id
	data.append( "--" );
	data.append( boundary );
	data.append( "\r\nContent-Type: text/plain" );
	data.append( "\r\nContent-Disposition: form-data; name=\"" );
	data.append( ID );
	data.append( "\"\r\n\r\n" );
	data.append( wstrToStr( meta_data->getId()->c_str() )->c_str() );
	data.append( "\r\n" );
	//end id

	//begin file_id
	data.append( "--" );
	data.append( boundary );
	data.append( "\r\nContent-Type: text/plain" );
	data.append( "\r\nContent-Disposition: form-data; name=\"" );
	data.append( FILE_ID );
	data.append( "\"\r\n\r\n" );
	data.append( std::to_string( mdud.getId() ) );
	data.append( "\r\n" );
	//end file_id

	//begin size
	data.append( "--" );
	data.append( boundary );
	data.append( "\r\nContent-Type: text/plain" );
	data.append( "\r\nContent-Disposition: form-data; name=\"" );
	data.append( FILE_SIZE );
	data.append( "\"\r\n\r\n" );
	data.append( std::to_string( size ) );
	data.append( "\r\n" );
	//end size

	if ( uc->isCredential() ) {
		//begin login id
		data.append( "--" );
		data.append( boundary );
		data.append( "\r\nContent-Type: text/plain" );
		data.append( "\r\nContent-Disposition: form-data; name=\"" );
		data.append( LOGIN_ID );
		data.append( "\"\r\n\r\n" );
		data.append( wstrToStr( uc->getLoginId()->c_str() )->c_str() );
		data.append( "\r\n" );
		//end login id
	}
}