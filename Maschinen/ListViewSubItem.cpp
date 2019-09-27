#include "stdafx.h"
#include "ListViewSubItem.h"


maschinen::list_view::ListViewSubItem::ListViewSubItem() {ZeroMemory( &sub_item, sizeof( LVITEM ) ); sub_item.iSubItem = 0;}
void maschinen::list_view::ListViewSubItem::setPosition( int position ) noexcept {sub_item.iSubItem = position;}

void maschinen::list_view::ListViewSubItem::setText( LPCTSTR text ) noexcept { sub_item.mask |= LVFIF_TEXT; sub_item.pszText = (LPWSTR)text; }
void maschinen::list_view::ListViewSubItem::setText( tstring* text ) noexcept{ sub_item.mask |= LVFIF_TEXT; sub_item.pszText = ( LPWSTR ) text->c_str(); }
void maschinen::list_view::ListViewSubItem::setText( tstring& text ) noexcept{ sub_item.mask |= LVFIF_TEXT; sub_item.pszText = ( LPWSTR ) text.c_str(); }


maschinen::list_view::ListViewSubItem::~ListViewSubItem() {
}

LPLVITEM maschinen::list_view::ListViewSubItem::get() noexcept {return &sub_item;}
