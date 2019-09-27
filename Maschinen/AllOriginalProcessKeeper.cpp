#include "stdafx.h"
#include "AllOriginalProcessKeeper.h"

WNDPROC list_view_edit_original_process_keeper = NULL;
std::vector<OriginalProcessKeeper*> container_original_process_keepers;
std::vector<OriginalProcessKeeper*> edit_original_process_keepers;
std::vector<OriginalProcessKeeper*> list_view_detail_original_process_keepers;
std::vector<OriginalProcessKeeper*> custom_original_process_keepers;

void createOriginalProcessKeeper( UINT id, WNDPROC original_process, int type ) {
	OriginalProcessKeeper* opk = new OriginalProcessKeeper();
	opk->id = id;
	opk->orignal_process = original_process;
	custom_original_process_keepers.push_back( opk );
}

void deleteOriginalProcessKeeper( UINT id, int type ) {
	OriginalProcessKeeper* opk = nullptr;
	for ( auto i = custom_original_process_keepers.begin(); i != custom_original_process_keepers.end(); ++i ) {
		if ( ( *i )->id == id ) opk = *i;
	}
	if(opk )
		custom_original_process_keepers.erase( std::remove( custom_original_process_keepers.begin(), custom_original_process_keepers.end(), opk ), custom_original_process_keepers.end() );
}

LRESULT defaultControlProcess( int type, HWND& handler, UINT message, WPARAM wparam, LPARAM lparam ) {
	int id = GetDlgCtrlID( handler );
	for ( auto i = custom_original_process_keepers.begin(); i != custom_original_process_keepers.end(); i++ ) {
		if ( id == ( *i )->id ) return CallWindowProc( ( *i )->orignal_process, handler, message, wparam, lparam );
	}
	throw maschinen::exception::NullReferenceControlObjectException();
}