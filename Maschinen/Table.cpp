#include "stdafx.h"
#include "Table.h"


maschinen::list_view::Table::Table() : maschinen::list_view::ListViewBasic(){}
maschinen::list_view::Table::Table( UINT id, maschinen::ContainedController* parent) : maschinen::list_view::ListViewBasic(id, parent){
	( ( maschinen::Window* )parent->getWindowParent() )->getChildren()->addTable( this, parent );
}

void maschinen::list_view::Table::addColumn( LPCTSTR text, int width ) {
	LVCOLUMN col;
	ZeroMemory( &col, sizeof( LVCOLUMN ) );
	col.mask = LVCF_TEXT | LVCF_SUBITEM | LVCF_WIDTH;
	col.cx = width;
	col.iSubItem = getColumnCount();
	col.pszText = ( LPTSTR ) text;
	//col.iSubItem = 0;
	if ( ListView_InsertColumn( handler, getColumnCount(), &col ) == -1 ) {
		TCHAR error[ MAX_LOADSTRING ];
		DWORD error_code;
		if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) )
			throw maschinen::exception::Exception( error, error_code );
		else throw maschinen::exception::Exception();
	}
}

void maschinen::list_view::Table::addColumn( LPCTSTR text ) {
	LVCOLUMN col;
	ZeroMemory( &col, sizeof( LVCOLUMN ) );
	col.mask = LVCF_TEXT | LVCF_SUBITEM;
	col.cx = 200;
	col.iSubItem = getColumnCount();
	col.pszText = (LPTSTR)text;
	//col.iSubItem = 0;
	if ( ListView_InsertColumn( handler, getColumnCount(), &col ) == -1 ) {
		TCHAR error[ MAX_LOADSTRING ];
		DWORD error_code;
		if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) )
			throw maschinen::exception::Exception( error, error_code );
		else throw maschinen::exception::Exception();
	}
}

void maschinen::list_view::Table::addRow() {
	LVITEM item;
	ZeroMemory( &item, sizeof( LVITEM ) );
	item.iSubItem = 0;
	item.mask |= LVIF_TEXT; 
	item.pszText = LPSTR_TEXTCALLBACK;
	item.iItem = (int)getRowCount();
	ListView_InsertItem( handler, &item );
}

void maschinen::list_view::Table::setOnListViewSelectListener( maschinen::event::listener::OnListViewSelectListener* listener, bool auto_delete ) {
	WindowEventKeeper* wek = ( ( maschinen::Window* )getWindowParent() )->getListeners();
	if ( wek != nullptr ) {
		ListViewSelectListenerKeeper* keeper = new ListViewSelectListenerKeeper();
		keeper->listener = listener;
		keeper->id = getId();
		keeper->auto_delete = auto_delete;
		wek->table_select_keeper.push_back( keeper );
	}
}

void maschinen::list_view::Table::setText(LPCTSTR text, int row, int column) const{ ListView_SetItemText( handler, row, column, (LPTSTR)text );}
void maschinen::list_view::Table::setText( tstring& text, int row, int column ) const { ListView_SetItemText( handler, row, column, ( LPTSTR ) text.c_str() ); }
void maschinen::list_view::Table::setText( tstring* text, int row, int column ) const{ ListView_SetItemText( handler, row, column, ( LPTSTR ) text->c_str() ); }
std::unique_ptr<tstring> maschinen::list_view::Table::getText( int row, int column ) const {
	TCHAR buffer[ MAX_LOADSTRING ];
	ListView_GetItemText( handler, row, column, buffer, ( int ) MAX_LOADSTRING );
	std::unique_ptr<tstring> temp = std::make_unique<tstring>(buffer);
	return temp;
}

void maschinen::list_view::Table::create()
{
	style = WS_CHILD | LVS_REPORT;
	ListViewBasic::create();
	SetWindowSubclass( handler, TableProcessStarter, getId(), reinterpret_cast< DWORD_PTR >( this ) );
	ListView_SetExtendedListViewStyle(handler, extended_style);
}

void maschinen::list_view::Table::autoWidth(bool auto_width)
{
	this->auto_width = auto_width;
}

bool maschinen::list_view::Table::isAutoWidth()
{
	return auto_width;
}

void maschinen::list_view::Table::applyShowGridLines(bool show_grid_lines)
{
	if (show_grid_lines)
		extended_style |= LVS_EX_GRIDLINES;
	else
	{
		extended_style |= LVS_EX_GRIDLINES;
		extended_style ^= LVS_EX_GRIDLINES;
	}
}

void maschinen::list_view::Table::applyHeaderDragDrop(bool header_drag_drop)
{
	if (header_drag_drop)
		extended_style |= LVS_EX_HEADERDRAGDROP;
	else
	{
		extended_style |= LVS_EX_HEADERDRAGDROP;
		extended_style ^= LVS_EX_HEADERDRAGDROP;
	}
}

void maschinen::list_view::Table::applyFullRowSelect(bool full_row_select)
{
	if (full_row_select)
		extended_style |= LVS_EX_FULLROWSELECT;
	else
	{
		extended_style |= LVS_EX_FULLROWSELECT;
		extended_style ^= LVS_EX_FULLROWSELECT;
	}
}

void maschinen::list_view::Table::applyCellEditable( bool cell_editable ) {this->cell_editable = cell_editable;}

void maschinen::list_view::Table::deleteRow( int index ) {
	ListView_DeleteItem( handler, index );
}
int maschinen::list_view::Table::getRowCount() {return (int)ListView_GetItemCount( handler );}

int maschinen::list_view::Table::getColumnCount() {
	HWND header_handler = ListView_GetHeader( handler );
	return Header_GetItemCount( header_handler );
}

Position maschinen::list_view::Table::getCellCoordinate( int row, int column ) const noexcept {
	RECT rect;
	ZeroMemory( &rect, sizeof( RECT ) );
	if( column == 0 ) {
		ListView_GetItemRect( handler, row, &rect, LVIR_BOUNDS );
		RECT rect_left_helper;
		ListView_GetItemRect( handler, row, &rect_left_helper, LVIR_LABEL );
		rect.left = rect_left_helper.left;
	}
	else ListView_GetSubItemRect( handler, row, column, LVIR_BOUNDS, &rect );
	POINT p;
	p.x = rect.left;
	p.y = rect.top;
	ClientToScreen( handler, &p );
	ScreenToClient( getWindowHandler(), &p );
	Position pos;
	pos.x = p.x;
	pos.y = p.y;
	pos.width = rect.right - rect.left;
	pos.height = rect.bottom - rect.top;
	return pos;
}

void maschinen::list_view::Table::setEditOnCell( int row, int column, DWORD edit_style, bool empty_edit, LPCTSTR place_holder ) {
	RECT sub_item_rect;
	ListView_GetSubItemRect( handler, row, column, LVIR_BOUNDS, &sub_item_rect );
	int height = sub_item_rect.bottom - sub_item_rect.top;
	int width = sub_item_rect.right - sub_item_rect.left;
	if ( column == 0 ) width /= getColumnCount();
	lvei = std::make_unique<ListViewEditItem>( getWindowHandler(), handler, row, column );
	lvei->setPosition( sub_item_rect.top, sub_item_rect.left, width, height );
	lvei->create( edit_style, (empty_edit) ? NULL : getText(row, column)->c_str(), place_holder );
	lvei->focus();
}

void maschinen::list_view::Table::setEditOnCell( int row, int column, bool empty_edit, LPCTSTR place_holder, bool is_number ) {
	if( lvei.get() ){
		if ( !lvei->isDestroyed() )  lvei->destroy();
	}
	lvei.reset();
	DWORD style;
	RECT rect;
	ZeroMemory( &rect, sizeof( RECT ) );
	if( column == 0 ) {
		ListView_GetItemRect( handler, row, &rect, LVIR_BOUNDS );
		RECT rc2;
		ListView_GetItemRect( handler, row, &rc2, LVIR_LABEL );
		rect.left = rc2.left;
	}
	else {
		ListView_GetSubItemRect( handler, row, column, LVIR_BOUNDS, &rect );
	}
	if ( is_number ) style |= ES_NUMBER;
	POINT p;
	p.x = rect.left;
	p.y = rect.top;
	ClientToScreen( handler, &p );
	ScreenToClient( getWindowHandler(), &p );
	int height = rect.bottom - rect.top;
	int width = rect.right - rect.left;
	if ( column == 0 ) width /= getColumnCount();
	lvei = std::make_unique<ListViewEditItem>( getWindowHandler(), handler, row, column );
	lvei->setPosition( p.x, p.y, width, height );
	lvei->create( style, ( empty_edit ) ? NULL : getText( row, column )->c_str(), place_holder );
	lvei->focus();
}

LRESULT maschinen::list_view::Table::TableProcess( HWND handler, UINT message, WPARAM wparam, LPARAM lparam ) {
	switch ( message ) {
		case WM_SHOWWINDOW:
		{
			if ( wparam == TRUE && lparam == 0 && getColumnCount() > 0 ) {
				RECT rect;
				GetClientRect( handler, &rect );
				LONG width = ( rect.right - rect.left ) / getColumnCount();
				for ( auto i = 0; i < getColumnCount(); i++ ) {
					ListView_SetColumnWidth( handler, i, width );
				}
			}
		} break;

		case WM_DESTROY:
		{
			onDestroy();
		} break;

		case WM_NCDESTROY:
		{
			RemoveWindowSubclass( handler, TableProcessStarter, getId() );
		} break;
	}
	return DefSubclassProc( handler, message, wparam, lparam );
}

LRESULT CALLBACK maschinen::list_view::Table::TableProcessStarter( HWND handler, UINT message, WPARAM wparam, LPARAM lparam, UINT_PTR uid_sub_class, DWORD_PTR dw_ref_data ) {
	Table* lv = reinterpret_cast<Table*>(dw_ref_data);
	return lv->TableProcess( handler, message, wparam, lparam );
}

UINT maschinen::list_view::Table::getSelectedItemCount() const { return ListView_GetSelectedCount( handler ); }
UINT maschinen::list_view::Table::getSelectedColumn() const { return ListView_GetSelectedColumn( handler ); }

void maschinen::list_view::Table::onDestroy() {
	Children* children = ( ( maschinen::Window* )getWindowParent() )->getChildren();
	if ( children )children->removeTable( this );
	WindowEventKeeper* wek = ( ( maschinen::Window* )getWindowParent() )->getListeners();
	ListViewSelectListenerKeeper* lvslk = nullptr;
	for ( auto i = wek->table_select_keeper.cbegin(); i != wek->table_select_keeper.cend(); ++i ) {
		if ( ( *i )->id == getId() ) {
			lvslk = *i; break;
		}
	}
	if ( lvslk ) {
		delete lvslk;
	}
}

void maschinen::list_view::Table::setParent( maschinen::ContainedController* parent ) {
	maschinen::UncontainedController::setParent( parent );
	( ( maschinen::Window* )parent->getWindowParent() )->getChildren()->addTable( this, parent );
}
void maschinen::list_view::Table::setParentAndId( maschinen::ContainedController* parent, UINT id ) {
	maschinen::UncontainedController::setParentAndId( parent, id );
	( ( maschinen::Window* )parent->getWindowParent() )->getChildren()->addTable( this, parent );
}
bool maschinen::list_view::Table::operator==( int type ) const noexcept { return type == TABLE; }
bool maschinen::list_view::Table::operator!=( int type ) const noexcept { return type == TABLE; }
bool maschinen::list_view::Table::compare( int type ) const noexcept { return type == TABLE; }

maschinen::list_view::Table::~Table()
{
	destroy();
}
