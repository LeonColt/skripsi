#include "stdafx.h"
#include "Edit.h"


maschinen::Edit::Edit()
{
	key_pressed_listener = nullptr;
	focus_listener = nullptr;
}

maschinen::Edit::Edit(UINT id, ContainedController* parent) : UncontainedController(id, parent)
{
	( ( maschinen::Window* )parent->getWindowParent() )->getChildren()->addEdit( this, parent );
}

void maschinen::Edit::create()
{
	style |= WS_CHILD;
	style |= WS_BORDER;
	handler = CreateWindowEx(WS_EX_CLIENTEDGE, WC_EDIT, (LPCTSTR)NULL, style, getX(), getY(), getWidth(), getHeight(), getWindowHandler(), (HMENU)(UINT_PTR)getId(), getInstance(), this);
	SetWindowSubclass( handler, EditProcessStarter, getId(), reinterpret_cast< DWORD_PTR >( this ) );
	HFONT font = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	SendMessage(handler, WM_SETFONT, (WPARAM)font, MAKELPARAM(FALSE, 0));
}

void maschinen::Edit::Draw()
{

}

void maschinen::Edit::update()
{

}

void maschinen::Edit::applyPassword(bool password)
{
	if (password) style |= ES_PASSWORD;
	else
	{
		style |= ES_PASSWORD;
		style ^= ES_PASSWORD;
	}
}

void maschinen::Edit::applyNumber(bool number)
{
	if (number) style |= ES_NUMBER;
	else
	{
		style |= ES_NUMBER;
		style ^= ES_NUMBER;
	}
}

void maschinen::Edit::applyReadOnly(bool read_only)
{
	if (read_only) style |= ES_READONLY;
	else
	{
		style |= ES_READONLY;
		style ^= ES_READONLY;
	}
}

void maschinen::Edit::applyRichText(bool rich_text)
{
	if (rich_text)
	{
		style |= ES_AUTOHSCROLL;
		style |= ES_AUTOVSCROLL;
		style |= ES_MULTILINE;
		style |= ES_NOHIDESEL;
	}
	else
	{
		style |= ES_AUTOHSCROLL;
		style |= ES_AUTOVSCROLL;
		style |= ES_MULTILINE;
		style |= ES_NOHIDESEL;
		style ^= ES_AUTOHSCROLL;
		style ^= ES_AUTOVSCROLL;
		style ^= ES_MULTILINE;
		style ^= ES_NOHIDESEL;
	}
}

void maschinen::Edit::setPlaceHolder(bool appear_after_focus, LPCWSTR str) const noexcept {SendMessage(handler, EM_SETCUEBANNER, (WPARAM)appear_after_focus, (LPARAM)str);}
void maschinen::Edit::setText( LPCTSTR text ) const noexcept {SetWindowText( handler, text );}
void maschinen::Edit::setText( tstring& text ) const noexcept {SetWindowText( handler, text.c_str() );}
void maschinen::Edit::setText( tstring* text ) const noexcept {SetWindowText( handler, text->c_str() );}
void maschinen::Edit::clearText() const noexcept {SetWindowText( handler, NULL );}
bool maschinen::Edit::isTextEmpty() const noexcept {return GetWindowTextLength( handler ) == 0;}

std::unique_ptr<tstring> maschinen::Edit::getText( ) const noexcept {
	std::unique_ptr<tstring> res = std::make_unique<tstring>();
	const int size = GetWindowTextLength( handler ) + 1;
	std::unique_ptr<TCHAR[]> buffer = std::make_unique<TCHAR[]>( size );
	GetWindowText( handler, buffer.get(), size );
	res->assign( buffer.get() );
	return res;
}

LRESULT CALLBACK maschinen::Edit::EditProcessStarter( HWND handler, UINT message, WPARAM wparam, LPARAM lparam, UINT_PTR uid_sub_class, DWORD_PTR dw_ref_data ) {
	Edit* edit = reinterpret_cast< Edit* >( dw_ref_data );
	return edit->EditProcess( handler, message, wparam, lparam );
}

LRESULT maschinen::Edit::EditProcess( HWND handler, UINT message, WPARAM wparam, LPARAM lparam ) {
	switch ( message ) {
		case WM_SETFOCUS:
		{
			if ( focus_listener ) focus_listener->onFocus( this, true );
		} break;

		case WM_KEYDOWN:
		{
			if ( key_pressed_listener ) key_pressed_listener->onKeyDown( this, wparam );
		} break;

		case WM_KEYUP:
		{
			if ( key_pressed_listener ) key_pressed_listener->onKeyUp( this, wparam );
		} break;

		case WM_KILLFOCUS:
		{
			if ( focus_listener ) focus_listener->onFocus( this, false );
		} break;

		case WM_DESTROY:
		{
			onDestroy();
		} break;
		case WM_NCDESTROY:
		{
			RemoveWindowSubclass( handler, EditProcessStarter, getId() );
		} break;
	}
	return DefSubclassProc( handler, message, wparam, lparam );
}

void maschinen::Edit::onDestroy() {
	Children* children = ( ( maschinen::Window* )getWindowParent() )->getChildren();
	if ( children )children->removeEdit( this );
	WindowEventKeeper* wek = ( ( maschinen::Window* )getWindowParent() )->getListeners();
	ClickListenerKeeper* clk = nullptr;
	for ( auto i = wek->click_listener_keepers.cbegin(); i != wek->click_listener_keepers.cend(); ++i ) {
		if ( ( *i )->id == getId() ) {
			clk = *i;
			break;
		}
	}
	if ( clk ) {
		wek->click_listener_keepers.erase( std::remove( wek->click_listener_keepers.begin(), wek->click_listener_keepers.end(), clk ), wek->click_listener_keepers.end() );
		if ( clk->auto_delete ) delete clk->ocl;
		delete clk;
	}
	EditTextChangedListenerKeeper* etclk = nullptr;
	for ( auto i = wek->edit_text_changed_listener_keeper.cbegin(); i != wek->edit_text_changed_listener_keeper.cend(); ++i ) {
		if ( ( *i )->id == getId() ) {
			etclk = *i;
			break;
		}
	}
	if ( etclk ) {
		wek->edit_text_changed_listener_keeper.erase( std::remove( wek->edit_text_changed_listener_keeper.begin(), wek->edit_text_changed_listener_keeper.end(), etclk ), wek->edit_text_changed_listener_keeper.end() );
		if ( etclk->auto_delete ) delete etclk->listener;
		delete etclk;
	}
	if ( key_pressed_listener ) {
		if ( key_pressed_listener->isAutoDelete() ) delete key_pressed_listener;
	}
	if( focus_listener ) {
		if ( focus_listener->isAutoDelete() ) delete focus_listener;
	}
}

void maschinen::Edit::setParent( maschinen::ContainedController* parent ) {
	maschinen::UncontainedController::setParent( parent );
	( ( maschinen::Window* )parent->getWindowParent() )->getChildren()->addEdit( this, parent );
}

void maschinen::Edit::setParentAndId( maschinen::ContainedController* parent, UINT id ) {
	maschinen::UncontainedController::setParentAndId( parent, id );
	( ( maschinen::Window* )parent->getWindowParent() )->getChildren()->addEdit( this, parent );
}

void maschinen::Edit::setOnClickListener( maschinen::event::OnClickListener* listener, bool auto_delete ) {
	ATOM window_id = GetClassWord( getWindowHandler(), GCW_ATOM );
	WindowEventKeeper* wek = nullptr;
	for ( auto i = listener_keepers.begin(); i != listener_keepers.end(); ++i ) {
		if ( ( *i )->window_id == window_id ) wek = *i;
	}
	if ( wek != nullptr ) {
		ClickListenerKeeper* clk = new ClickListenerKeeper();
		clk->ocl = listener;
		clk->id = getId();
		clk->auto_delete = auto_delete;
		wek->click_listener_keepers.push_back( clk );
	}
}

void maschinen::Edit::setOnEditTextChangedListener( maschinen::event::listener::OnEditTextChangedListener* listener, bool auto_delete ) {
	ATOM window_id = GetClassWord( getWindowHandler(), GCW_ATOM );
	WindowEventKeeper* wek = nullptr;
	for ( auto i = listener_keepers.cbegin(); i != listener_keepers.cend(); ++i ) {
		if ( ( *i )->window_id == window_id ) wek = *i;
	}
	if ( wek != nullptr ) {
		EditTextChangedListenerKeeper* keeper = new EditTextChangedListenerKeeper();
		keeper->listener = listener;
		keeper->id = getId();
		keeper->auto_delete = auto_delete;
		wek->edit_text_changed_listener_keeper.push_back( keeper );
	}
}

void maschinen::Edit::setOnKeyPressedChangeListener( maschinen::event::listener::OnKeyPressedListener* listener, bool auto_delete ) {
	if( this->key_pressed_listener ) {
		if ( this->key_pressed_listener->isAutoDelete() ) delete this->key_pressed_listener;
	}
	this->key_pressed_listener = listener;
	listener->setAutoDelete( auto_delete );
}

void maschinen::Edit::setOnFocusListener( maschinen::event::listener::OnFocusListener* listener, bool auto_delete ) {
	if(this->focus_listener ) {
		if ( this->focus_listener->isAutoDelete() ) delete this->focus_listener;
	}
	this->focus_listener = listener;
	listener->setAutoDelete( auto_delete );
}

bool maschinen::Edit::operator==( int type ) const noexcept { return type == EDIT; }
bool maschinen::Edit::operator!=( int type ) const noexcept { return type == EDIT; }
bool maschinen::Edit::compare( int type ) const noexcept { return type == EDIT; }

maschinen::Edit::~Edit(){}
