#include "stdafx.h"
#include "Button.h"


maschinen::Button::Button()
{
}

maschinen::Button::Button( UINT id, ContainedController* parent) : maschinen::UncontainedController(id, parent) {
	( ( maschinen::Window* )parent->getWindowParent() )->getChildren()->addButton( this, parent );
}

LRESULT maschinen::Button::ButtonProcess( HWND handler, UINT message, WPARAM wparam, LPARAM lparam ) {
	switch ( message ) {
		case WM_NCDESTROY: RemoveWindowSubclass( handler, ButtonProcessStarter, getId() );
	}
	return DefSubclassProc( handler, message, wparam, lparam );
}

void maschinen::Button::setText( LPCTSTR text ) const noexcept {
	SetWindowText( handler, text );
}
void maschinen::Button::setText( const tstring& text ) const noexcept {
	SetWindowText( handler, text.c_str() );
}
void maschinen::Button::setText( tstring& text ) const noexcept {
	SetWindowText( handler, text.c_str() );
}
void maschinen::Button::setText( tstring* text ) const noexcept {
	SetWindowText( handler, text->c_str() );
}

std::unique_ptr<tstring> maschinen::Button::getText() const noexcept {
	const int size = GetWindowTextLength( handler ) + 1;
	std::unique_ptr<tstring> res = std::make_unique<tstring>( TEXT( "" ) );
	std::unique_ptr<TCHAR[]> buffer = std::make_unique<TCHAR[]>( size );
	GetWindowText( handler, buffer.get(), size );
	res->assign( buffer.get() );
	return res;
}

void maschinen::Button::create()
{
	HFONT font = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	DWORD style = WS_CHILD | WS_TABSTOP | WS_BORDER | BS_DEFPUSHBUTTON | BS_CENTER | BS_VCENTER;
	handler = CreateWindowEx(WS_EX_CLIENTEDGE, WC_BUTTON, (LPCTSTR)NULL, style, getX(), getY(), getWidth(), getHeight(), getWindowHandler(), (HMENU)(UINT_PTR)getId(), getInstance(), NULL);
	SetWindowSubclass( handler, ButtonProcessStarter, getId(), reinterpret_cast< DWORD_PTR >( this ) );
	if ( handler == NULL ) {
		TCHAR buffer[ MAX_LOADSTRING ];
		getLastErrorAsString( buffer, MAX_LOADSTRING );
		OutputDebugStringW( buffer );
	}
	SendMessage(getHandler(), WM_SETFONT, (WPARAM)font, MAKELPARAM(FALSE, 0));
}

void maschinen::Button::setOnClickListener( maschinen::event::OnClickListener* const on_click_listener, bool auto_delete ) {
	ATOM window_id = GetClassWord( getWindowHandler(), GCW_ATOM );
	WindowEventKeeper* wek = nullptr;
	for ( auto i = listener_keepers.begin(); i != listener_keepers.end(); ++i ) {
		if ( ( *i )->window_id == window_id ) wek = *i;
	}
	if ( wek != nullptr ) {
		ClickListenerKeeper* clk = new ClickListenerKeeper();
		clk->ocl = on_click_listener;
		clk->id = getId();
		clk->auto_delete = auto_delete;
		wek->click_listener_keepers.push_back( clk );
	}
}

void maschinen::Button::setParent( maschinen::ContainedController* parent ) {
	maschinen::UncontainedController::setParent( parent );
	( ( maschinen::Window* )parent->getWindowParent() )->getChildren()->addButton( this, parent );
}

void maschinen::Button::setParentAndId( maschinen::ContainedController* parent, UINT id ) {
	maschinen::UncontainedController::setParentAndId( parent, id );
	( ( maschinen::Window* )parent->getWindowParent() )->getChildren()->addButton( this, parent );
}

bool maschinen::Button::operator==( int type ) const noexcept { return type == BUTTON; }
bool maschinen::Button::operator!=( int type ) const noexcept { return type == BUTTON; }
bool maschinen::Button::compare( int type ) const noexcept {return type == BUTTON;}

LRESULT CALLBACK maschinen::Button::ButtonProcessStarter( HWND handler, UINT message, WPARAM wparam, LPARAM lparam, UINT_PTR uid_sub_class, DWORD_PTR dw_ref_data ) {
	Button* button = reinterpret_cast< Button* >( dw_ref_data );
	return button->ButtonProcess( handler, message, wparam, lparam );
}

maschinen::Button::~Button()
{
}
