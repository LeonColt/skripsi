#include "stdafx.h"
#include "MenuItem.h"


maschinen::menu::MenuItem::MenuItem() {
	ZeroMemory( &item, sizeof( MENUITEMINFO ) );
	item.cbSize = sizeof( MENUITEMINFO );
}

void maschinen::menu::MenuItem::asIcon( UINT id, maschinen::Bitmap* bitmap ) {
	item.fMask |= MIIM_ID;
	item.wID = id;
	item.fMask |= MIIM_FTYPE;
	item.fType |= MFT_BITMAP;
	item.fMask |= MIIM_BITMAP;
	item.hbmpItem = bitmap->getHandler();
}

void maschinen::menu::MenuItem::asText( UINT id, LPCTSTR text ) {
	item.fMask |= MIIM_ID;
	item.wID = id;
	item.fMask |= MIIM_FTYPE;
	item.fType |= MFT_STRING;
	item.fMask |= MIIM_STRING;
	item.dwTypeData = ( LPTSTR ) text;
	item.cch = (UINT)tcslen( text );
}

void maschinen::menu::MenuItem::setSubMenu( maschinen::menu::PopUpMenu* sub_menu ) {
	item.fMask |= MIIM_SUBMENU;
	item.hSubMenu = sub_menu->getHandler();
}

void maschinen::menu::MenuItem::asSeperator() {
	item.fType |= IMFT_SEPARATOR;
}

void maschinen::menu::MenuItem::setImageForChecked( maschinen::Bitmap* bitmap ) {
	item.fMask |= MIIM_CHECKMARKS;
	item.hbmpChecked = bitmap->getHandler();
}

void maschinen::menu::MenuItem::setImageForUnchecked( maschinen::Bitmap* bitmap ) {
	item.fMask |= MIIM_CHECKMARKS;
	item.hbmpUnchecked = bitmap->getHandler();
}

void maschinen::menu::MenuItem::setData( LPVOID data ) {
	item.fMask |= MIIM_DATA;
	item.dwItemData = reinterpret_cast< ULONG_PTR >( data );
}

void maschinen::menu::MenuItem::setTextOrderRightToLeft( bool value ) {
	item.fMask |= MIIM_FTYPE;
	if(value ) item.fType |= MFT_RIGHTORDER;
	else {
		item.fType |= MFT_RIGHTORDER;
		item.fType ^= MFT_RIGHTORDER;
	}
}

void maschinen::menu::MenuItem::setRightJustify( bool value ) {
	item.fMask |= MIIM_FTYPE;
	if(value ) item.fType |= MFT_RIGHTJUSTIFY;
	else {
		item.fType |= MFT_RIGHTJUSTIFY;
		item.fType ^= MFT_RIGHTJUSTIFY;
	}
}

LPMENUITEMINFO maschinen::menu::MenuItem::get() {
	return &item;
}

void maschinen::menu::MenuItem::operator=( maschinen::menu::MenuItem* menu ) {item = menu->item;}
void maschinen::menu::MenuItem::operator=( MENUITEMINFO item ) {
	this->item = item;
}

UINT maschinen::menu::MenuItem::getId() {
	return item.wID;
}

maschinen::menu::MenuItem::~MenuItem() {
}
