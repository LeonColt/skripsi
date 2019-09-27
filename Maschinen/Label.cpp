#include "stdafx.h"
#include "Label.h"


maschinen::Label::Label() {
}

maschinen::Label::Label( UINT id, ContainedController* parent ) : UncontainedController(id, parent) {
	( ( maschinen::Window* )parent->getWindowParent() )->getChildren()->addLabel( this, parent );
}

void maschinen::Label::create() {
	style |= WS_CHILD;
	style |= WS_BORDER;
	handler = CreateWindowEx( WS_EX_CLIENTEDGE, WC_STATIC, LPCTSTR( NULL ), style, getX(), getY(), getWidth(), getHeight(), getWindowHandler(), ( HMENU ) ( UINT_PTR ) getId(), getInstance(), NULL );
	HFONT font = ( HFONT ) GetStockObject( DEFAULT_GUI_FONT );
	SendMessage( handler, WM_SETFONT, ( WPARAM ) font, MAKELPARAM( FALSE, 0 ) );
}

void maschinen::Label::setText( LPCTSTR text ) const {
	if(handler != NULL) 
		SetWindowText( handler, text );
}

void maschinen::Label::setText( tstring& text ) const {
	SetWindowText( handler, text.c_str() );
}

std::unique_ptr<tstring> maschinen::Label::getText() const {
	std::unique_ptr<tstring> res = std::make_unique<tstring>();
	const int size = GetWindowTextLength( handler ) + 1;
	TCHAR* buffer = new TCHAR[ size ];
	GetWindowText( handler, buffer, size );
	res->assign( buffer );
	delete[] buffer;
	return res;
}

void maschinen::Label::setParent( maschinen::ContainedController* parent ) {
	UncontainedController::setParent( parent );
	( ( maschinen::Window* )parent->getWindowParent() )->getChildren()->addLabel( this, parent );
}

void maschinen::Label::setParentAndId( maschinen::ContainedController* parent, UINT id ) {
	UncontainedController::setParentAndId( parent, id );
	( ( maschinen::Window* )parent->getWindowParent() )->getChildren()->addLabel( this, parent );
}

maschinen::Label::~Label() {
}
