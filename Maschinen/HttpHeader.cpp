#include "stdafx.h"
#include "HttpHeader.h"


maschinen::wininet::HttpHeader::HttpHeader() {
}

void maschinen::wininet::HttpHeader::setVariable( LPCTSTR variable ) {
	this->variable.assign( variable );
}

void maschinen::wininet::HttpHeader::setValue( LPCTSTR value ) {
	this->value.assign( value );
}

void maschinen::wininet::HttpHeader::setVariable( tstring& variable ) {
	this->variable.assign( variable.c_str() );
}

void maschinen::wininet::HttpHeader::setValue( tstring& value ) {
	this->value.assign( value.c_str() );
}

void maschinen::wininet::HttpHeader::setVariable( tstring* variable ) {
	this->variable.assign( variable->c_str() );
}

void maschinen::wininet::HttpHeader::setValue( tstring* value ) {
	this->value.assign( value->c_str() );
}

void maschinen::wininet::HttpHeader::setVariable( const tstring& variable ) {
	this->variable.assign( variable.c_str() );
}
void maschinen::wininet::HttpHeader::setValue( const tstring& value ) {
	this->value.assign( value.c_str() );
}

void maschinen::wininet::HttpHeader::setVariable( const tstring* variable ) {
	this->variable.assign( variable->c_str() );
}
void maschinen::wininet::HttpHeader::setValue( const tstring* value ) {
	this->value.assign( value->c_str() );
}

const TCHAR* maschinen::wininet::HttpHeader::getVariable() {
	return variable.c_str();
}

const TCHAR* maschinen::wininet::HttpHeader::getValue() {
	return value.c_str();
}

maschinen::wininet::HttpHeader::~HttpHeader() {
}
