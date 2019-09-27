#include "stdafx.h"
#include "CheckBox.h"


maschinen::CheckBox::CheckBox()
{
}

maschinen::CheckBox::CheckBox( UINT id, ContainedController* parent) : UncontainedController(id, parent)
{
	( ( maschinen::Window* )parent->getWindowParent() )->getChildren()->addCheckBox( this, parent );
}

void maschinen::CheckBox::create()
{
	HFONT font = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	DWORD style = WS_CHILD | WS_BORDER | BS_CHECKBOX;
	handler = CreateWindowEx(WS_EX_CLIENTEDGE, WC_BUTTON, (LPCTSTR)NULL, style, getX(), getY(),getWidth(), getHeight(), getWindowHandler(), (HMENU)(UINT_PTR)getId(), getInstance(), NULL);
	SendMessage(handler, WM_SETFONT, (WPARAM)font, MAKELPARAM(FALSE, 0));
}

void maschinen::CheckBox::setOnCheckedChangeListener( maschinen::event::listener::OnCheckedChangeListener* on_checked_change_listener, bool auto_delete ) {
	ATOM window_id = GetClassWord( getWindowHandler(), GCW_ATOM );
	WindowEventKeeper* wek = nullptr;
	for ( auto i = listener_keepers.begin(); i != listener_keepers.end(); ++i ) {
		if ( ( *i )->window_id == window_id ) wek = *i;
	}
	if ( wek != nullptr ) {
		CheckedChangeListenerKeeper* cckp = new CheckedChangeListenerKeeper();
		cckp->occl = on_checked_change_listener;
		cckp->auto_delete = auto_delete;
		cckp->id = getId();
		wek->checked_change_listener_keeper.push_back( cckp );
	}
}

void maschinen::CheckBox::setChecked( bool checked  ) const {checked ? Button_SetCheck( handler, BST_CHECKED ) : Button_SetCheck( handler, BST_UNCHECKED );}
bool maschinen::CheckBox::isChecked() const{return (Button_GetCheck(handler) == BST_CHECKED);}

void maschinen::CheckBox::setText( LPCTSTR text ) const noexcept {
	SetWindowText( handler, text );
}
void maschinen::CheckBox::setText( tstring& text ) const noexcept {
	SetWindowText( handler, text.c_str() );
}
void maschinen::CheckBox::setTexT( tstring* text ) const noexcept {
	SetWindowText( handler, text->c_str() );
}

void maschinen::CheckBox::setParent( maschinen::ContainedController* parent ) {
	maschinen::UncontainedController::setParent( parent );
	( ( maschinen::Window* )parent->getWindowParent() )->getChildren()->addCheckBox( this, parent );
}
void maschinen::CheckBox::setParentAndId( maschinen::ContainedController* parent, UINT id ) {
	maschinen::UncontainedController::setParentAndId( parent, id );
	( ( maschinen::Window* )parent->getWindowParent() )->getChildren()->addCheckBox( this, parent );
}
bool maschinen::CheckBox::operator==( int type ) const noexcept { return type == CHECKBOX; }
bool maschinen::CheckBox::operator!=( int type ) const noexcept { return type == CHECKBOX; }
bool maschinen::CheckBox::compare( int type ) const noexcept { return type == CHECKBOX; }

maschinen::CheckBox::~CheckBox()
{
}
