#include "stdafx.h"
#include "AllEventHandler.h"

std::vector<WindowEventKeeper*> listener_keepers;

void setMenuOnClickListener( maschinen::Window* parent_window, unsigned int menu, maschinen::event::OnClickListener* on_click_listener, bool auto_delete ) {
	ATOM window_id = 0;
	if ( parent_window == nullptr ) return;
	else if ( parent_window->compare( WINDOW ) ) window_id = GetClassWord( parent_window->getHandler(), GCW_ATOM );
	else return;
	WindowEventKeeper* wek = nullptr;
	for ( auto i = listener_keepers.begin(); i != listener_keepers.end(); ++i ) {
		if ( ( *i )->window_id == window_id ) wek = *i;
	}
	if ( wek != nullptr ) {
		ClickListenerKeeper* clk = new ClickListenerKeeper();
		clk->ocl = on_click_listener;
		clk->id = menu;
		clk->auto_delete = auto_delete;
		wek->click_listener_keepers.push_back( clk );
	}
}

void unSetMenuOnClickListener( maschinen::Window* parent_window, unsigned int menu ) {
	ATOM window_id = 0;
	if ( parent_window == nullptr ) return;
	else if ( parent_window->compare( WINDOW ) ) window_id = GetClassWord( parent_window->getHandler(), GCW_ATOM );
	else return;
	WindowEventKeeper* wek = nullptr;
	for ( auto i = listener_keepers.begin(); i != listener_keepers.end(); ++i ) {
		if ( ( *i )->window_id == window_id ) wek = *i;
	}
	ClickListenerKeeper* clk = nullptr;
	for ( auto i = wek->click_listener_keepers.begin(); i != wek->click_listener_keepers.end(); ++i ) {
		if ( ( *i )->id == menu ) clk = *i;
	}
	delete clk;
	wek->click_listener_keepers.erase( std::remove( wek->click_listener_keepers.begin(), wek->click_listener_keepers.end(), clk ), wek->click_listener_keepers.end() );
}