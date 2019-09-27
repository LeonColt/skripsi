#include "stdafx.h"
#include "VisibleController.h"


maschinen::VisibleController::VisibleController() { ZeroMemory( &pos, sizeof( Position ) ); handler = NULL; }
maschinen::VisibleController::VisibleController( UINT id, maschinen::VisibleController* parent ) : maschinen::Controller( id ) { ZeroMemory( &pos, sizeof( Position ) ); this->parent = parent; handler = NULL; }
void maschinen::VisibleController::enable() const noexcept {if (handler != NULL) EnableWindow(handler, TRUE);}
void maschinen::VisibleController::disable() const noexcept {if (handler != NULL) EnableWindow(handler, FALSE);}
void maschinen::VisibleController::destroy() noexcept {if (handler != NULL) DestroyWindow(handler);}
bool maschinen::VisibleController::isDestroyed()  const noexcept{return (IsWindow(handler) > 0) ? false : true;}
bool maschinen::VisibleController::isVisible()  const noexcept{return ( !isDestroyed() ) ? ( (IsWindowVisible(handler) == FALSE) ? false : true ) : false;}
void maschinen::VisibleController::show() const noexcept { ShowWindow( handler, SW_SHOW ); }
void maschinen::VisibleController::forceMinimize() const noexcept { ShowWindow( handler, SW_FORCEMINIMIZE);}
void maschinen::VisibleController::hide() const noexcept{ ShowWindow( handler, SW_HIDE);}
void maschinen::VisibleController::maximize() const noexcept { ShowWindow( handler, SW_MAXIMIZE);}
void maschinen::VisibleController::minimize() const noexcept{ShowWindow( handler, SW_MINIMIZE);}
void maschinen::VisibleController::restore() const noexcept{ShowWindow( handler, SW_RESTORE);}
void maschinen::VisibleController::showDefault() const noexcept{ShowWindow( handler, SW_SHOWDEFAULT);}

void maschinen::VisibleController::setPosition( LONG x, LONG y, LONG width, LONG height ) noexcept { 
	pos.x = x; pos.y = y; pos.width = width; pos.height = height;
	if( handler != NULL ) SetWindowPos( handler, NULL, x, y, width, height, SWP_NOZORDER );
}

void maschinen::VisibleController::setStyleClipSibling( bool style ) {
	if( handler == NULL ) {
		if( style ) {
			if( !(this->style & WS_CLIPCHILDREN) ) this->style |= WS_CLIPCHILDREN;
		}
		else {
			if ( this->style & WS_CLIPCHILDREN ) {
				this->style ^= WS_CLIPCHILDREN;
			}
		}
	}
	else {
		this->style = (DWORD)GetWindowLongPtr( handler, GWL_STYLE );
		if ( style ) {
			if ( !( this->style & WS_CLIPCHILDREN ) ) {
				this->style |= WS_CLIPCHILDREN;
				SetWindowLongPtr( handler, GWL_STYLE, this->style );
			}
		}
		else {
			if ( this->style & WS_CLIPCHILDREN ) {
				this->style ^= WS_CLIPCHILDREN;
				SetWindowLongPtr( handler, GWL_STYLE, this->style );
			}
		}
	}
}

maschinen::VisibleController::~VisibleController() { }
