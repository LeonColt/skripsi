#include "stdafx.h"
#include "Local.h"


Local::Local() {}
void Local::rename( LPCTSTR new_name ) const {
	if ( MoveFile( getPath()->c_str(), new_name ) == FALSE ) {
		TCHAR buffer[ MAX_LOADSTRING ];
		DWORD error_code;
		if ( getLastErrorAsString( buffer, MAX_LOADSTRING, error_code ) )
			throw SeraphException( buffer, error_code );
		else throw SeraphException();
	}
}
bool Local::compare( Local* local ) {
	if ( local == nullptr ) return false;
	else return getPath()->compare( local->getPath()->c_str() ) == 0;
}

bool Local::compare( Local& local ) {
	return getPath()->compare( local.getPath()->c_str() ) == 0;
}
bool Local::operator==( Local* local ) {
	if ( local == nullptr ) return false;
	else return getPath()->compare( local->getPath()->c_str() ) == 0;
}
bool Local::operator==( Local& local ) {
	return getPath()->compare( local.getPath()->c_str() ) == 0;
}

Local::~Local() {
}
