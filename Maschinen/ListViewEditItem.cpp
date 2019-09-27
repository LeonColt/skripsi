#include "stdafx.h"
#include "ListViewEditItem.h"

maschinen::list_view::ListViewEditItem::ListViewEditItem( const HWND parent_handler, const HWND lv_handler, const int row, const int column ) : PARENT_HANDLER( parent_handler ), LV_HANDLER( lv_handler ), ROW( row ), COLUMN(column) {
}

bool maschinen::list_view::ListViewEditItem::onGainFocus() {
	return false;
}

bool maschinen::list_view::ListViewEditItem::onLostFocus() {
	return false;
}

void maschinen::list_view::ListViewEditItem::setPosition( int x, int y, int width, int height ) {
	position = MAKEQWORD( x, y, width, height );
	if ( handler != INVALID_HANDLE_VALUE )
		SetWindowPos( handler, NULL, x, y, width, height, SWP_NOZORDER );
}

void maschinen::list_view::ListViewEditItem::create( DWORD style, LPCTSTR text , LPCTSTR place_holder ) {
	DWORD temp = WS_CHILD | ES_LEFT | WS_VISIBLE;
	//temp |= style;
	handler = CreateWindowEx( WS_EX_CLIENTEDGE, WC_EDIT, (text == NULL) ? NULL : text, temp, LOWORDLEFT( position ), HIWORDLEFT( position ), LOWORDRIGHT( position ), HIWORDRIGHT( position ), PARENT_HANDLER, 0, GetModuleHandle( NULL ), this );
	//if(text == NULL && place_holder != NULL )  SendMessage( handler, EM_SETCUEBANNER, ( WPARAM ) true, ( LPARAM ) place_holder );
	//list_view_edit_original_process_keeper = (WNDPROC)SetWindowLongPtr( handler, GWLP_WNDPROC, ( LONG_PTR ) listViewEditItemProcessStarter );
}

LRESULT CALLBACK maschinen::list_view::ListViewEditItem::listViewEditItemProcessStarter( HWND handler, UINT message, WPARAM wparam, LPARAM lparam ) {
	ListViewEditItem* lvei = nullptr;
	if ( message == WM_NCCREATE ) {
		LPCREATESTRUCT lpcs = reinterpret_cast< LPCREATESTRUCT >( lparam );
		lvei = static_cast< ListViewEditItem* >( lpcs->lpCreateParams );
		SetWindowLongPtr( handler, GWLP_USERDATA, ( LONG_PTR ) lvei );
	}
	else lvei = reinterpret_cast< ListViewEditItem* >( GetWindowLongPtr( handler, GWLP_USERDATA ) );
	if ( lvei ) return lvei->listViewEditItemProcess( handler, message, wparam, lparam );
	else return CallWindowProc( list_view_edit_original_process_keeper, handler, message, wparam, lparam );
}

LRESULT maschinen::list_view::ListViewEditItem::listViewEditItemProcess( HWND handler, UINT message, WPARAM wparam, LPARAM lparam ) {
	switch ( message ) {
		case WM_KILLFOCUS:
		{
			LV_DISPINFO lv_dispinfo;
			ZeroMemory( &lv_dispinfo, sizeof( LV_DISPINFO ) );
			PostMessage( handler, WM_CLOSE, NULL, NULL );
			lv_dispinfo.hdr.hwndFrom = handler;
			lv_dispinfo.hdr.idFrom = 0;
			lv_dispinfo.hdr.code = LVN_ENDLABELEDIT;
			lv_dispinfo.item.mask = LVIF_TEXT;
			lv_dispinfo.item.iItem = ROW;
			lv_dispinfo.item.iSubItem = COLUMN;
			TCHAR buffer[ MAX_LOADSTRING ];
			if ( GetWindowText( handler, buffer, MAX_LOADSTRING ) == 0 ) lv_dispinfo.item.pszText = NULL;
			else {
				lv_dispinfo.item.pszText = buffer;
				lv_dispinfo.item.cchTextMax = lstrlen( buffer );
			}
			SendMessage( PARENT_HANDLER, WM_NOTIFY, ( WPARAM )GetDlgCtrlID(LV_HANDLER), (LPARAM)&lv_dispinfo );
			destroy();
		} break;
		default: return CallWindowProc( original_process, handler, message, wparam, lparam );
	}
	return CallWindowProc( original_process, handler, message, wparam, lparam );
}
void maschinen::list_view::ListViewEditItem::focus() const {SetFocus( handler );}
void maschinen::list_view::ListViewEditItem::destroy() const {DestroyWindow( handler );}
bool maschinen::list_view::ListViewEditItem::isDestroyed() const noexcept {return IsWindow( handler ) == 0 ? true : false;}

maschinen::list_view::ListViewEditItem::~ListViewEditItem() {
}
