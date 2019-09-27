#include "stdafx.h"
#include "HttpHeaders.h"


maschinen::wininet::HttpHeaders::HttpHeaders() {
}

void maschinen::wininet::HttpHeaders::add( LPCTSTR variable, LPCTSTR value ) {
	HttpHeader header;
	header.setVariable( variable );
	header.setValue( value );
	headers.push_back( header );
}

void maschinen::wininet::HttpHeaders::add( const tstring& variable, const tstring& value ) {
	HttpHeader header;
	header.setVariable( variable );
	header.setValue( value );
	headers.push_back( header );
}
void maschinen::wininet::HttpHeaders::add( const tstring* variable, const tstring* value ) {
	HttpHeader header;
	header.setVariable( variable );
	header.setValue( value );
	headers.push_back( header );
}

void maschinen::wininet::HttpHeaders::add( LPCTSTR variable, const tstring& value ) {
	HttpHeader header;
	header.setVariable( variable );
	header.setValue( value );
	headers.push_back( header );
}
void maschinen::wininet::HttpHeaders::add( const tstring& variable, LPCTSTR value ) {
	HttpHeader header;
	header.setVariable( variable );
	header.setValue( value );
	headers.push_back( header );
}

void maschinen::wininet::HttpHeaders::add( LPCTSTR variable, const tstring* value ) {
	HttpHeader header;
	header.setVariable( variable );
	header.setValue( value );
	headers.push_back( header );
}
void maschinen::wininet::HttpHeaders::add( const tstring* variable, LPCTSTR value ) {
	HttpHeader header;
	header.setVariable( variable );
	header.setValue( value );
	headers.push_back( header );
}

void maschinen::wininet::HttpHeaders::add( const tstring& variable, const tstring* value ) {
	HttpHeader header;
	header.setVariable( variable );
	header.setValue( value );
	headers.push_back( header );
}
void maschinen::wininet::HttpHeaders::add( const tstring* variable, const tstring& value ) {
	HttpHeader header;
	header.setVariable( variable );
	header.setValue( value );
	headers.push_back( header );
}

void maschinen::wininet::HttpHeaders::add( maschinen::wininet::HttpHeader& header ) {
	HttpHeader t_header;
	t_header.setVariable( header.getVariable() );
	t_header.setValue( header.getValue() );
	headers.push_back( t_header );
}

void maschinen::wininet::HttpHeaders::add( maschinen::wininet::HttpHeader* header ) {
	HttpHeader t_header;
	t_header.setVariable( header->getVariable() );
	t_header.setValue( header->getValue() );
	headers.push_back( t_header );
}

std::unique_ptr<tstring> maschinen::wininet::HttpHeaders::serialize() {
	std::unique_ptr<tstring> res = std::make_unique<tstring>( TEXT( "" ) );
	for ( auto i = headers.begin(); i != headers.end(); ++i ) {
		res->append( i->getVariable() );
		res->append( TEXT( " : " ) );
		res->append( i->getValue() );
		res->append( TEXT( "\r\n" ) );
	}
	if ( headers.size() > 0 ) res->append( TEXT( "\r\n" ) );
	return res;
}

maschinen::wininet::HttpHeaders::~HttpHeaders() {
}
