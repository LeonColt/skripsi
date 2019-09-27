// CSCommunication.cpp : Defines the exported functions for the DLL application.
//
#include "stdafx.h"
#include "HttpRequest.h"

// This is the constructor of a class that has been exported.
// see CSCommunication.h for the class definition
maschinen::wininet::HttpRequest::HttpRequest() : ACCESS_TYPE_DIRECT( INTERNET_OPEN_TYPE_DIRECT ),
ACCESS_TYPE_PRE_CONFIG( INTERNET_OPEN_TYPE_PRECONFIG ),
ACESS_TYPE_PRE_CONFIG_WITH_NO_AUTO_PROXY( INTERNET_OPEN_TYPE_PRECONFIG_WITH_NO_AUTOPROXY ),
VERSION_HTTP_10( TEXT( "HTTP/1.0" ) ),
VERSION_HTTP_11( TEXT( "HTTP/1.1" ) ),
REQUEST_TIME_OUT_INFINITE( 0xFFFFFFFF ){status = 0;}

maschinen::wininet::HttpRequestReadingIterator& maschinen::wininet::HttpRequest::beginRead(_Out_ LPVOID buffer, _In_ DWORD buffer_size, _Inout_ LPDWORD number_of_byte_read)
{
	if (ri.get() != nullptr) ri.reset();
	if (reading_finish.get() != nullptr) reading_finish.reset();
	ri = std::make_unique<maschinen::wininet::HttpRequestReadingIterator>();
	reading_finish = std::make_unique<maschinen::wininet::HttpRequestReadingIterator>();
	ri->hr = nullptr;
	reading_finish->hr = nullptr;
	if (InternetReadFile(hrequest, buffer, buffer_size, number_of_byte_read))
	{
		if (number_of_byte_read != 0)
		{
			ri->buffer = buffer;
			ri->buffer_size = buffer_size;
			ri->number_of_byte_read = number_of_byte_read;
			ri->hr = this;
		}
	}
	return *ri.get();
}

maschinen::wininet::HttpRequestReadingIterator& maschinen::wininet::HttpRequest::endRead() const { return *reading_finish.get(); }

void maschinen::wininet::HttpRequest::write( LPVOID buffer, DWORD buffer_size, LPDWORD number_of_byte_written ) const {
	if ( InternetWriteFile( hrequest, buffer, buffer_size, number_of_byte_written ) == FALSE ) {
		TCHAR error[ MAX_LOADSTRING ];
		DWORD error_code = 0;
		if ( getLastErrorAsString( error, error_code ) )
			throw maschinen::exception::Exception( error, error_code );
		else throw maschinen::exception::Exception();
	}
}

//begin SendRequest
INT maschinen::wininet::HttpRequest::sendRequest( LPCTSTR headers )
{
	//begin send request 
	{
#ifdef _UNICODE
		std::string temp( wstrToStr( sending_data.c_str() )->c_str() );
		if ( !HttpSendRequest( hrequest,
							   headers,
							   ( DWORD ) wcslen( headers ),
							   //L"Content-Type: application/x-www-form-urlencoded",
							   //( DWORD ) wcslen( L"Content-Type: application/x-www-form-urlencoded" ),
							   (LPVOID)temp.c_str(),
							   (DWORD)temp.length() ) )
#else
		if ( !HttpSendRequest( hrequest,
							   headers,
							   ( DWORD ) wcslen( headers ),
							   //L"Content-Type: application/x-www-form-urlencoded",
							   //( DWORD ) wcslen( L"Content-Type: application/x-www-form-urlencoded" ),
							   sending_data.c_str(),
							   ( DWORD ) sending_data.length() ) )
#endif // _UNICODE
		{
			DWORD error_code;
			TCHAR error[ MAX_LOADSTRING ];
			if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) ) {
				if ( error_code != ERROR_INTERNET_OPERATION_CANCELLED )
					throw maschinen::exception::Exception( error, error_code );
				else return 0;
			}
			else throw maschinen::exception::Exception();
		}
		GetLocalTime( &local_time );
		GetSystemTime( &system_time );
	}
	//end send request

	//begin query status code
	{
		TCHAR buffer[MAX_LOADSTRING];
		DWORD size_buffer = MAX_LOADSTRING;
		if ( HttpQueryInfo( hrequest, HTTP_QUERY_STATUS_CODE, buffer, &size_buffer, 0 ) )
			return _wtoi( buffer );
	}
	//end query status code
	return 0;
}

INT maschinen::wininet::HttpRequest::sendRequest( tstring& headers ) { return sendRequest( headers.c_str() ); }
INT maschinen::wininet::HttpRequest::sendRequest( tstring* headers ) { return sendRequest( headers->c_str() ); }
INT maschinen::wininet::HttpRequest::sendRequest( maschinen::wininet::HttpHeaders& headers ) { return sendRequest(headers.serialize()->c_str()); }

void maschinen::wininet::HttpRequest::beginRequest( INTERNET_BUFFERS& internet_buffers ) const{
	if ( HttpSendRequestEx( hrequest, &internet_buffers, NULL, 0, NULL ) == FALSE ) {
		TCHAR error[ MAX_LOADSTRING ];
		DWORD error_code = 0;
		if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) ) {
			throw maschinen::exception::Exception( error, error_code );
		}
		else throw maschinen::exception::Exception();
	}
}
void maschinen::wininet::HttpRequest::beginRequest( INTERNET_BUFFERS* internet_buffers ) const {
	if ( HttpSendRequestEx( hrequest, internet_buffers, NULL, 0, NULL ) == FALSE ) {
		TCHAR error[ MAX_LOADSTRING ];
		DWORD error_code = 0;
		if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) ) {
			throw maschinen::exception::Exception( error, error_code );
		}
		else throw maschinen::exception::Exception();
	}
}

void maschinen::wininet::HttpRequest::beginRequest( LPCTSTR header, DWORD header_length_total, LPVOID buffer, DWORD buffer_size, DWORD buffer_size_total ) const {
	HttpAddRequestHeaders( hrequest, header, -1, HTTP_ADDREQ_FLAG_REPLACE | HTTP_ADDREQ_FLAG_ADD );
	INTERNET_BUFFERS ibuffer;
	ZeroMemory( &ibuffer, sizeof( INTERNET_BUFFERS ) );
	ibuffer.dwStructSize = sizeof( INTERNET_BUFFERS );
	/*
	ibuffer.lpcszHeader = header;
	ibuffer.dwHeadersLength = ( DWORD ) tcslen( header );
	if ( header_length_total != NULL ) ibuffer.dwHeadersTotal = header_length_total;
	if ( buffer != NULL && buffer != nullptr ) {
		ibuffer.lpvBuffer = buffer;
		ibuffer.dwBufferLength = buffer_size;
	}
	*/
	if ( buffer_size_total != NULL ) ibuffer.dwBufferTotal = buffer_size_total;
	if ( HttpSendRequestEx( hrequest, &ibuffer, NULL, 0, NULL ) == FALSE ) {
		TCHAR error[ MAX_LOADSTRING ];
		DWORD error_code = 0;
		if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) ){
			OutputDebugStringW( error );
			throw maschinen::exception::Exception( error, error_code );
		}
		else throw maschinen::exception::Exception();
	}
}

void maschinen::wininet::HttpRequest::beginRequest( tstring& header, DWORD header_length_total, LPVOID buffer, DWORD buffer_size, DWORD buffer_size_total ) const {
	try {
		beginRequest( header.c_str(), header_length_total, buffer, buffer_size, buffer_size_total );
	} catch ( maschinen::exception::Exception& e ) {
		throw e;
	}
}

void maschinen::wininet::HttpRequest::beginRequest( tstring* header, DWORD header_length_total, LPVOID buffer, DWORD buffer_size, DWORD buffer_size_total ) const {
	try {
		beginRequest( header->c_str(), header_length_total, buffer, buffer_size, buffer_size_total );
	} catch ( maschinen::exception::Exception& e ) {
		throw e;
	}
}

void maschinen::wininet::HttpRequest::beginRequest( maschinen::wininet::HttpHeaders& header, DWORD header_length_total, LPVOID buffer, DWORD buffer_size, DWORD buffer_size_total ) const {
	try {
		beginRequest( header.serialize()->c_str(), header_length_total, buffer, buffer_size, buffer_size_total );
	} catch ( maschinen::exception::Exception& e ) {
		throw e;
	}
}

void maschinen::wininet::HttpRequest::endRequest() const {
	if ( HttpEndRequest( hrequest, NULL, 0, 0 ) == FALSE ) {
		TCHAR error[ MAX_LOADSTRING ];
		DWORD error_code = 0;
		if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) ) {
			OutputDebugStringW( error );
			OutputDebugStringW( L"\n" );
			OutputDebugStringW( to_tstring( error_code ).c_str() );
			throw maschinen::exception::Exception( error, error_code );
		}
		else throw maschinen::exception::Exception();
	}
}
//end SendRequest

//begin HttpOpenRequest
void maschinen::wininet::HttpRequest::openRequest( LPCTSTR url, LPCTSTR version, LPCTSTR referer, LPCTSTR* accept_types, DWORD flags, maschinen::wininet::HttpParams* params )
{
	//begin preparing date
	{
		this->url.clear();
		this->url.assign( url );
		sending_data.clear();
		if ( params != nullptr )
		{
			for ( auto i = params->begin( params->METHOD_GET ); i != params->end(); i++ )
			{
				if ( i != params->begin( params->METHOD_GET ) ) this->url.append( TEXT( "&" ) );
				else
				{
#ifdef _UNICODE
					if ( *this->url.crbegin() != L'?' )
#else
					if ( *this->url.crbegin() != '?' )
#endif // _UNICODE
						this->url.append( TEXT( "?" ) );
				}
				this->url.append( ( *i )->getVariable() );
				this->url.append( TEXT( "=" ) );
				this->url.append( ( *i )->getValue() );
			}
			for ( auto i = params->begin( params->METHOD_POST ); i != params->end(); i++ )
			{
				if ( i != params->begin( params->METHOD_POST ) ) sending_data.append( TEXT( "&" ) );
				sending_data.append( ( *i )->getVariable() );
				sending_data.append( TEXT( "=" ) );
				sending_data.append( ( *i )->getValue() );
			}
		}
	}
	//end prepareing data

	//begin open http request
	{
		hrequest = HttpOpenRequest( hconnect,
									TEXT( "POST" ),
									this->url.c_str(),
									( version == NULL ) ? NULL : version,
									( referer == NULL ) ? NULL : referer,
									accept_types, flags, NULL );
		if ( hrequest == NULL )
		{
			DWORD error_code;
			TCHAR error[ MAX_LOADSTRING ];
			if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) )
				throw maschinen::exception::Exception( error, error_code );
			else throw maschinen::exception::Exception();
		}
		DWORD time_out = 0xFFFFFFFF;
		InternetSetOption( hrequest, INTERNET_OPTION_CONNECT_TIMEOUT, (LPVOID)&time_out, sizeof( time_out ) );
	}
	// end open http request
}
//end HttpOpenRequest

//begin InternetConnect
void maschinen::wininet::HttpRequest::connect( LPCTSTR server_name, LPCTSTR username, LPCTSTR password )
{
	tstring temp_url = TEXT( "http://" ); temp_url.append( server_name ); 
	if ( !InternetCheckConnection( temp_url.c_str(), FLAG_ICC_FORCE_CONNECTION, 0 ) )
		throw maschinen::exception::Exception( L"cannot connect to server", 0 );
	hconnect = InternetConnect( hinet,
								server_name,
								INTERNET_DEFAULT_HTTP_PORT,
								username,
								password,
								INTERNET_SERVICE_HTTP,
								NULL,
								NULL );
	if ( hconnect == NULL )
	{
		DWORD error_code;
		TCHAR error[ MAX_LOADSTRING ];
		if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) )
			throw maschinen::exception::Exception( error, error_code );
		else throw maschinen::exception::Exception();
	}
}
//end InternetConnect

//begin InternetOpen
void maschinen::wininet::HttpRequest::open( LPCTSTR user_agent, DWORD type_access )
{
	if ( type_access == INTERNET_OPEN_TYPE_PROXY 
		 && (type_access != INTERNET_OPEN_TYPE_DIRECT 
			  || type_access != INTERNET_OPEN_TYPE_PRECONFIG
			  || type_access != INTERNET_OPEN_TYPE_PRECONFIG_WITH_NO_AUTOPROXY)) type_access = INTERNET_OPEN_TYPE_PRECONFIG;
	hinet = InternetOpen(
		user_agent, //user_agent
		type_access, //acess type
		NULL, //username if proxy
		NULL,  //password if proxy
		NULL );
	if ( hinet == NULL )
	{
		DWORD error_code;
		TCHAR error[ MAX_LOADSTRING ];
		if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) )
			throw maschinen::exception::Exception( error, error_code );
		else throw maschinen::exception::Exception();
	}
}

void maschinen::wininet::HttpRequest::openProxy( LPCTSTR user_agent, LPCTSTR proxy_name, LPCTSTR proxy_bypass )
{
	hinet = InternetOpen(
		user_agent, //user_agent
		INTERNET_OPEN_TYPE_PROXY, //acess type
		proxy_name, //username if proxy
		proxy_bypass,  //password if proxy
		NULL );
	if ( hinet == NULL )
	{
		DWORD error_code;
		TCHAR error[ MAX_LOADSTRING ];
		if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) )
			throw maschinen::exception::Exception( error, error_code );
		else throw maschinen::exception::Exception();
	}
}
//end InternetOpen

void maschinen::wininet::HttpRequest::closeRequest() const noexcept{if ( hrequest != NULL ) InternetCloseHandle( hrequest );}
void maschinen::wininet::HttpRequest::disconnect() const noexcept{if ( hconnect != NULL ) InternetCloseHandle( hconnect );}
void maschinen::wininet::HttpRequest::close() const noexcept{if (hinet != NULL) InternetCloseHandle(hinet);}
maschinen::wininet::HttpRequest::~HttpRequest(){closeRequest();disconnect();close();}

INT maschinen::wininet::HttpRequest::getStatusCode() const {
	TCHAR buffer[ MAX_LOADSTRING ];
	DWORD size_buffer = MAX_LOADSTRING;
	if ( HttpQueryInfo( hrequest, HTTP_QUERY_STATUS_CODE, buffer, &size_buffer, 0 ) ) 
		return _wtoi( buffer );
	return 0;
}

//begin set
void maschinen::wininet::HttpRequest::setReferer( LPCTSTR referer ) noexcept { this->referer.assign( referer ); }
void maschinen::wininet::HttpRequest::setAcceptTypes( LPCTSTR accept_types ) noexcept { this->accept_types.assign( accept_types ); }
void maschinen::wininet::HttpRequest::setRequestTimeOut( DWORD miliseconds ) {
	if ( InternetSetOption( hrequest, INTERNET_OPTION_CONNECT_TIMEOUT, ( LPVOID ) &miliseconds, sizeof( DWORD ) ) == FALSE ) {
		DWORD error_code;
		TCHAR error[ MAX_LOADSTRING ];
		if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) ) {
			if ( error_code != ERROR_INTERNET_OPERATION_CANCELLED )
				throw maschinen::exception::Exception( error, error_code );
		}
		else throw maschinen::exception::Exception();
	}
}

void maschinen::wininet::HttpRequest::setSendTimeOut( DWORD miliseconds ) {
	if ( InternetSetOption( hrequest, INTERNET_OPTION_SEND_TIMEOUT, ( LPVOID ) &miliseconds, sizeof( DWORD ) ) == FALSE ) {
		DWORD error_code;
		TCHAR error[ MAX_LOADSTRING ];
		if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) ) {
			if ( error_code != ERROR_INTERNET_OPERATION_CANCELLED )
				throw maschinen::exception::Exception( error, error_code );
		}
		else throw maschinen::exception::Exception();
	}
}

void maschinen::wininet::HttpRequest::setReceiveTimeOut( DWORD miliseconds ) {
	if ( InternetSetOption( hrequest, INTERNET_OPTION_RECEIVE_TIMEOUT, ( LPVOID ) &miliseconds, sizeof( DWORD ) ) == FALSE ) {
		DWORD error_code;
		TCHAR error[ MAX_LOADSTRING ];
		if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) ) {
			if ( error_code != ERROR_INTERNET_OPERATION_CANCELLED )
				throw maschinen::exception::Exception( error, error_code );
		}
		else throw maschinen::exception::Exception();
	}
}
//end set

//begin get
std::unique_ptr<tstring> maschinen::wininet::HttpRequest::getUrl() const noexcept { return std::make_unique<tstring>( url.c_str() ); }
std::unique_ptr<tstring> maschinen::wininet::HttpRequest::getSendingData() const noexcept {return std::make_unique<tstring>( sending_data.c_str() );}
void maschinen::wininet::HttpRequest::getLocalTimeExecuted( LPSYSTEMTIME local_time ) noexcept {copySystemTime( &this->local_time, local_time );}
void maschinen::wininet::HttpRequest::getSystemTimeExecuted( LPSYSTEMTIME system_time ) noexcept {copySystemTime( &this->system_time, system_time );}
//end get