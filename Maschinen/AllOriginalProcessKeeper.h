#pragma once
#include "Maschinen.h"
#include "NullReferenceControlObjectException.h"

struct MASCHINEN_API OriginalProcessKeeper {
	WNDPROC orignal_process;
	UINT id;
};

extern WNDPROC list_view_edit_original_process_keeper;

extern MASCHINEN_API std::vector<OriginalProcessKeeper*> custom_original_process_keepers;

MASCHINEN_API void createOriginalProcessKeeper( UINT id, WNDPROC original_process, int type );
MASCHINEN_API void  deleteOriginalProcessKeeper( UINT id, int type );
MASCHINEN_API LRESULT defaultControlProcess( int type, HWND& handler, UINT message, WPARAM wparam, LPARAM lparam );

