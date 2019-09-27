#include "stdafx.h"
#include "StatusBar.h"


maschinen::StatusBar::StatusBar() {
}

void maschinen::StatusBar::create() {
	style |= WS_CHILD;
	style |= WS_VISIBLE;
	handler = CreateWindowEx( WS_EX_CLIENTEDGE, STATUSCLASSNAME, TEXT( "" ), style, 0, 0, 0, 0, getWindowHandler(), ( HMENU ) ( UINT_PTR ) getId(), getInstance(), NULL );
}

void maschinen::StatusBar::simple( bool simple ) const {
	SendMessage( handler, SB_SIMPLE, ( simple ) ? TRUE : FALSE, MAKELPARAM( 0, 0 ) );
}

void maschinen::StatusBar::setText(LPCTSTR text, WPARAM mode ) const {
	SendMessage( handler, SB_SETTEXT, mode, reinterpret_cast<LPARAM>(text) );
}

void maschinen::StatusBar::setParent( maschinen::ContainedController* parent ) {
	maschinen::UncontainedController::setParent( parent );
	( ( maschinen::Window* )parent->getWindowParent() )->getChildren()->addStatusBar( this, parent );
}

void maschinen::StatusBar::setParentAndId( maschinen::ContainedController* parent, UINT id ) {
	maschinen::UncontainedController::setParentAndId( parent, id );
	( ( maschinen::Window* )parent->getWindowParent() )->getChildren()->addStatusBar( this, parent );
}
bool maschinen::StatusBar::operator==( int type ) const noexcept { return type == STATUSBAR; }
bool maschinen::StatusBar::operator!=( int type ) const noexcept { return type == STATUSBAR; }
bool maschinen::StatusBar::compare( int type ) const noexcept { return type == STATUSBAR; }

maschinen::StatusBar::~StatusBar() {
}
