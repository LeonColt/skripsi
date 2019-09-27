#include "stdafx.h"
#include "ListViewItem.h"


maschinen::list_view::ListViewItem::ListViewItem(){ZeroMemory( &item, sizeof( LVITEM ) );item.iSubItem = 0;}
void maschinen::list_view::ListViewItem::setGroup( int group_id ) {item.mask |= LVIF_GROUPID;item.iGroupId = group_id;}

void maschinen::list_view::ListViewItem::setOrderColumn(UINT order[])
{
	item.puColumns = order;
}

void maschinen::list_view::ListViewItem::setNumberofColumns(int number_of_columns)
{
	item.mask |= LVIF_COLUMNS;
	if (number_of_columns > 20) item.cColumns = 20;
	else item.cColumns = number_of_columns;
}

void maschinen::list_view::ListViewItem::setText( LPCTSTR text ) { item.mask |= LVIF_TEXT; item.pszText = (LPWSTR)text; }
void maschinen::list_view::ListViewItem::setText( tstring& text ) { item.mask |= LVIF_TEXT; item.pszText = ( LPWSTR ) text.c_str(); }
void maschinen::list_view::ListViewItem::setText( tstring* text ) { item.mask |= LVIF_TEXT; item.pszText = ( LPWSTR ) text->c_str(); }

void maschinen::list_view::ListViewItem::setPosition(int position)
{
	item.iItem = position;
}

void maschinen::list_view::ListViewItem::setImage(int image_index)
{
	item.mask |= LVIF_IMAGE;
	item.iImage = image_index;
}

void maschinen::list_view::ListViewItem::addSubItem( maschinen::list_view::ListViewSubItem* sub_item ) const noexcept {
	sub_item->get()->iItem = item.iItem;
}

maschinen::list_view::ListViewItem::~ListViewItem(){}
LPLVITEM maschinen::list_view::ListViewItem::get() noexcept { return &item; }




//begin state
void maschinen::list_view::ListViewItem::setStateActivating( bool state  ) noexcept { item.mask |= LVIF_STATE;  if ( state ) item.state |= LVIS_ACTIVATING; else { item.state |= LVIS_ACTIVATING; item.state ^= LVIS_ACTIVATING; }}
void maschinen::list_view::ListViewItem::setStateCut( bool state  ) noexcept{ item.mask |= LVIF_STATE; if ( state ) item.state |= LVIS_CUT; else { item.state |= LVIS_CUT; item.state ^= LVIS_CUT; }}
void maschinen::list_view::ListViewItem::setStateDropHilited( bool state ) noexcept{ item.mask |= LVIF_STATE; if ( state ) item.state |= LVIS_DROPHILITED; else { item.state |= LVIS_DROPHILITED; item.state ^= LVIS_DROPHILITED; }}
void maschinen::list_view::ListViewItem::setStateFocused( bool state ) noexcept{ item.mask |= LVIF_STATE; if ( state ) item.state |= LVIS_FOCUSED; else { item.state |= LVIS_FOCUSED; item.state ^= LVIS_FOCUSED; }}
void maschinen::list_view::ListViewItem::setStateOverLayMask( bool state ) noexcept{ item.mask |= LVIF_STATE; if ( state ) item.state |= LVIS_OVERLAYMASK; else { item.state |= LVIS_OVERLAYMASK; item.state ^= LVIS_OVERLAYMASK; }}
void maschinen::list_view::ListViewItem::setStateSelected( bool state ) noexcept{ item.mask |= LVIF_STATE; if ( state ) item.state |= LVIS_SELECTED; else { item.state |= LVIS_SELECTED; item.state ^= LVIS_SELECTED; }}
void maschinen::list_view::ListViewItem::setStateImageMask( bool state ) noexcept { item.mask |= LVIF_STATE; if ( state ) item.state |= LVIS_STATEIMAGEMASK; else { item.state |= LVIS_STATEIMAGEMASK; item.state ^= LVIS_STATEIMAGEMASK; }}
//end state

//begin get
std::unique_ptr<tstring> maschinen::list_view::ListViewItem::getText( const size_t buffer_size ) const noexcept {
	LVITEM temp;
	ZeroMemory( &temp, sizeof( LVITEM ) );
	temp.iItem = item.iItem;
	temp.mask = LVIF_TEXT;
	std::unique_ptr<TCHAR[]> buffer;
	if ( buffer_size ) buffer = std::make_unique<TCHAR[]>( buffer_size ); else buffer = std::make_unique<TCHAR[]>( MAX_LOADSTRING );
	std::unique_ptr<tstring> res = std::make_unique<tstring>();
	temp.pszText = (LPWSTR)buffer.get();
	temp.cchTextMax = ( int ) ( ( buffer_size ) ? buffer_size : MAX_LOADSTRING );
	ListView_GetItem( handler, &temp );
	res->assign( temp.pszText );
	return res;
}
//end get
