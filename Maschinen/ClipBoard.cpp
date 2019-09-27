#include "stdafx.h"
#include "ClipBoard.h"


maschinen::ClipBoard::ClipBoard() {
	state = OpenClipboard( NULL );
}

void maschinen::ClipBoard::empty() {
	if ( !EmptyClipboard() ) {
		TCHAR error[ MAX_LOADSTRING ];
		DWORD error_code;
		if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) ) {
			throw maschinen::exception::Exception( error, error_code );
		}
		else throw maschinen::exception::Exception();
	}
}

void maschinen::ClipBoard::setText( LPCTSTR text ) {

}

void maschinen::ClipBoard::setText( tstring& text ) {

}

void maschinen::ClipBoard::setPath( LPCTSTR path ) {
	const size_t size = sizeof( DROPFILES ) + ( ( tcslen( path ) + 2 ) * sizeof( TCHAR ) );
	HGLOBAL global = GlobalAlloc( GMEM_MOVEABLE, size );
	DROPFILES* df = ( DROPFILES* ) GlobalLock( global );
	ZeroMemory( df, size );
	df->pFiles = sizeof( DROPFILES );
#ifdef _UNICODE
	df->fWide = TRUE;
#else 
	df->fWide = FALSE;
#endif // _UNICODE
	LPTSTR str = ( LPTSTR ) ( df + 1 );
	lstrcpy( str, path );
	GlobalUnlock( global );
	SetClipboardData( CF_HDROP, global );
}

void maschinen::ClipBoard::setPath( tstring& path ) {
	setPath( path.c_str() );
}

maschinen::ClipBoard::~ClipBoard() {
	if ( state ) CloseClipboard();
}
