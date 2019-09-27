#include "stdafx.h"
#include "ProgressBar.h"
#include "Window.h"


maschinen::ProgressBar::ProgressBar()
{
}

maschinen::ProgressBar::ProgressBar( UINT id, ContainedController* parent) : UncontainedController(id, parent)
{
	( ( maschinen::Window* )parent->getWindowParent() )->getChildren()->addProgressBar( this, parent );
}

void maschinen::ProgressBar::setParent( ContainedController* parent ) {
	maschinen::UncontainedController::setParent( parent );
	( ( maschinen::Window* )parent->getWindowParent() )->getChildren()->addProgressBar( this, parent );
}
void maschinen::ProgressBar::setParentAndId( ContainedController* parent, UINT id ) {
	maschinen::UncontainedController::setParentAndId( parent, id );
	( ( maschinen::Window* )parent->getWindowParent() )->getChildren()->addProgressBar( this, parent );
}

void maschinen::ProgressBar::create()
{
	handler = CreateWindowEx(WS_EX_CLIENTEDGE, PROGRESS_CLASS, (LPCTSTR)NULL, WS_CHILD, getX(), getY(), getWidth(), getHeight(), getWindowHandler(), (HMENU)(UINT_PTR)getId(), getInstance(), NULL);
	SetWindowSubclass( handler, ProgressBarProcessStarter, getId(), reinterpret_cast< DWORD_PTR >( this ) );
	if(marquee) SetWindowLongPtr(handler, GWL_STYLE, PBS_MARQUEE);
}

LRESULT maschinen::ProgressBar::ProgressBarProcess( HWND handler, UINT message, WPARAM wparam, LPARAM lparam ) {
	switch ( message ) {
		case WM_NCDESTROY:
		{
			RemoveWindowSubclass( handler, ProgressBarProcessStarter, getId() );
		} break;
	}
	return DefSubclassProc( handler, message, wparam, lparam );
}

void maschinen::ProgressBar::styleMarquee(bool marquee)
{
	this->marquee = marquee;
	if ( handler != NULL ) return;
	if ( marquee ) {
		LONG_PTR style = GetWindowLongPtr( handler, GWL_STYLE );
		if ( !( style & PBS_MARQUEE ) ) SetWindowLongPtr( handler, GWL_STYLE, PBS_MARQUEE );
	}
	else {
		LONG_PTR style = GetWindowLongPtr( handler, GWL_STYLE );
		if ( style & PBS_MARQUEE ) style ^= PBS_MARQUEE;
		SetWindowLongPtr( handler, GWL_STYLE, WS_CHILD );
	}
}

void maschinen::ProgressBar::setMarqueeSpeed(bool on, int speed) {
	if (handler != nullptr && marquee) SendMessage(handler, PBM_SETMARQUEE, on ? TRUE : FALSE, (LPARAM)speed);
}

bool maschinen::ProgressBar::operator==( int type ) const noexcept { return type == PROGRESS_BAR; }
bool maschinen::ProgressBar::operator!=( int type ) const noexcept { return type == PROGRESS_BAR; }
bool maschinen::ProgressBar::compare( int type ) const noexcept { return type == PROGRESS_BAR; }

maschinen::ProgressBar::~ProgressBar()
{
}

LRESULT CALLBACK maschinen::ProgressBar::ProgressBarProcessStarter( HWND handler, UINT message, WPARAM wparam, LPARAM lparam, UINT_PTR uid_sub_class, DWORD_PTR dw_ref_data ) {
	ProgressBar* pb = reinterpret_cast< ProgressBar* >( dw_ref_data );
	return pb->ProgressBarProcess( handler, message, wparam, lparam );
}
