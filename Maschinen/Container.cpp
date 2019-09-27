#include "stdafx.h"
#include "Container.h"


maschinen::Container::Container(){}

maschinen::Container::Container( UINT id, maschinen::ContainedController* parent) : maschinen::ContainedController(id, parent){
	( ( maschinen::Window* )parent->getWindowParent() )->getChildren()->addContainer( this, parent );
}

LRESULT CALLBACK maschinen::Container::containerProcessStarter( HWND handler, UINT message, WPARAM wparam, LPARAM lparam, UINT_PTR uid_sub_class, DWORD_PTR dw_ref_data ) {
	Container* container = reinterpret_cast<Container*>(dw_ref_data);
	return container->containerProcess( handler, message, wparam, lparam );
}
LRESULT maschinen::Container::containerProcess( HWND handler, UINT message, WPARAM wparam, LPARAM lparam ) {
	switch ( message ) {
		case WM_SHOWWINDOW:
		{
			if ( wparam == TRUE ) {
				if ( lparam == 0 ) {
					if ( on_init ) {
						onInit();
						on_init = false;
					}
					onShow();
				}
			}
			else onHide();
		} break;
		case WM_SIZE: onSize( wparam, lparam ); break;
		case WM_DESTROY: onDestroy(); break;
		case WM_NCDESTROY: RemoveWindowSubclass( handler, containerProcessStarter, getId() ); break;
	}
	return DefSubclassProc( handler, message, wparam, lparam );
}

void maschinen::Container::onSize( WPARAM wparam, LPARAM lparam ) {
	if ( wparam != SIZE_MINIMIZED ) {
		RECT rect;
		GetClientRect( handler, &rect );
		MapWindowPoints( handler, getWindowHandler(), ( LPPOINT ) &rect, 2 );
		wlayout->calculate(&rect);
		size_t number_of_children = 0;
		for ( auto i = ( ( maschinen::Window* )getWindowParent() )->getChildren()->beginContainer(); i != ( ( maschinen::Window* )getWindowParent() )->getChildren()->endContainer(); ++i ) {
			if ( ( *i )->parent->getId() == getId() ) number_of_children++;
		}
		for ( auto i = ( ( maschinen::Window* )getWindowParent() )->getChildren()->beginLabel(); i != ( ( maschinen::Window* )getWindowParent() )->getChildren()->endLabel(); ++i ) {
			if ( ( *i )->parent->getId() == getId() ) number_of_children++;
		}
		for ( auto i = ( ( maschinen::Window* )getWindowParent() )->getChildren()->beginButton(); i != ( ( maschinen::Window* )getWindowParent() )->getChildren()->endButton(); ++i ) {
			if ( ( *i )->parent->getId() == getId() ) number_of_children++;
		}
		for ( auto i = ( ( maschinen::Window* )getWindowParent() )->getChildren()->beginEdit(); i != ( ( maschinen::Window* )getWindowParent() )->getChildren()->endEdit(); ++i ) {
			if ( ( *i )->parent->getId() == getId() ) number_of_children++;
		}
		for ( auto i = ( ( maschinen::Window* )getWindowParent() )->getChildren()->beginCheckBox(); i != ( ( maschinen::Window* )getWindowParent() )->getChildren()->endCheckBox(); ++i ) {
			if ( ( *i )->parent->getId() == getId() ) number_of_children++;
		}
		for ( auto i = ( ( maschinen::Window* )getWindowParent() )->getChildren()->beginTable(); i != ( ( maschinen::Window* )getWindowParent() )->getChildren()->endTable(); ++i ) {
			if ( ( *i )->parent->getId() == getId() ) number_of_children++;
		}
		for ( auto i = ( ( maschinen::Window* )getWindowParent() )->getChildren()->beginCustomControl(); i != ( ( maschinen::Window* )getWindowParent() )->getChildren()->endCustomControl(); ++i ) {
			if ( ( *i )->parent->getId() == getId() ) number_of_children++;
		}
		HDWP hdwp = BeginDeferWindowPos( ( int ) number_of_children );
		for ( auto i = ( ( maschinen::Window* )getWindowParent() )->getChildren()->beginContainer(); i != ( ( maschinen::Window* )getWindowParent() )->getChildren()->endContainer(); ++i ) {
			if ( ( *i )->parent->getId() == getId() ) {
				std::unique_ptr<Position> pos = std::move( wlayout->getCoordinate( ( *i )->control->getId() ) );
				DeferWindowPos( hdwp, ( *i )->control->getHandler(), NULL, pos->x, pos->y, pos->width, pos->height, SWP_NOZORDER );
			}
		}
		for ( auto i = ( ( maschinen::Window* )getWindowParent() )->getChildren()->beginLabel(); i != ( ( maschinen::Window* )getWindowParent() )->getChildren()->endLabel(); ++i ) {
			if ( ( *i )->parent->getId() == getId() ) {
				std::unique_ptr<Position> pos = std::move( wlayout->getCoordinate( ( *i )->control->getId() ) );
				DeferWindowPos( hdwp, ( *i )->control->getHandler(), NULL, pos->x, pos->y, pos->width, pos->height, SWP_NOZORDER );
			}
		}
		for ( auto i = ( ( maschinen::Window* )getWindowParent() )->getChildren()->beginButton(); i != ( ( maschinen::Window* )getWindowParent() )->getChildren()->endButton(); ++i ) {
			if ( ( *i )->parent->getId() == getId() ) {
				std::unique_ptr<Position> pos = std::move( wlayout->getCoordinate( ( *i )->control->getId() ) );
				DeferWindowPos( hdwp, ( *i )->control->getHandler(), NULL, pos->x, pos->y, pos->width, pos->height, SWP_NOZORDER );
			}
		}
		for ( auto i = ( ( maschinen::Window* )getWindowParent() )->getChildren()->beginEdit(); i != ( ( maschinen::Window* )getWindowParent() )->getChildren()->endEdit(); ++i ) {
			if ( ( *i )->parent->getId() == getId() ) {
				std::unique_ptr<Position> pos = std::move( wlayout->getCoordinate( ( *i )->control->getId() ) );
				DeferWindowPos( hdwp, ( *i )->control->getHandler(), NULL, pos->x, pos->y, pos->width, pos->height, SWP_NOZORDER );
			}
		}
		for ( auto i = ( ( maschinen::Window* )getWindowParent() )->getChildren()->beginCheckBox(); i != ( ( maschinen::Window* )getWindowParent() )->getChildren()->endCheckBox(); ++i ) {
			if ( ( *i )->parent->getId() == getId() ) {
				std::unique_ptr<Position> pos = std::move( wlayout->getCoordinate( ( *i )->control->getId() ) );
				DeferWindowPos( hdwp, ( *i )->control->getHandler(), NULL, pos->x, pos->y, pos->width, pos->height, SWP_NOZORDER );
			}
		}
		for ( auto i = ( ( maschinen::Window* )getWindowParent() )->getChildren()->beginTable(); i != ( ( maschinen::Window* )getWindowParent() )->getChildren()->endTable(); ++i ) {
			if ( ( *i )->parent->getId() == getId() ) {
				std::unique_ptr<Position> pos = std::move( wlayout->getCoordinate( ( *i )->control->getId() ) );
				DeferWindowPos( hdwp, ( *i )->control->getHandler(), NULL, pos->x, pos->y, pos->width, pos->height, SWP_NOZORDER );
			}
		}
		for ( auto i = ( ( maschinen::Window* )getWindowParent() )->getChildren()->beginCustomControl(); i != ( ( maschinen::Window* )getWindowParent() )->getChildren()->endCustomControl(); ++i ) {
			if ( ( *i )->parent->getId() == getId() ) {
				std::unique_ptr<Position> pos = std::move( wlayout->getCoordinate( ( *i )->control->getId() ) );
				DeferWindowPos( hdwp, ( *i )->control->getHandler(), NULL, pos->x, pos->y, pos->width, pos->height, SWP_NOZORDER );
			}
		}
		EndDeferWindowPos( hdwp );
	}
}

void maschinen::Container::onDestroy() {
}

void maschinen::Container::onInit() {
	RECT rect;
	GetClientRect( handler, &rect );
	MapWindowPoints( handler, getWindowHandler(), ( LPPOINT ) &rect, 2 );
	wlayout->calculate( &rect );
	for ( auto i = ( ( maschinen::Window* )getWindowParent() )->getChildren()->beginContainer(); i != ( ( maschinen::Window* )getWindowParent() )->getChildren()->endContainer(); ++i ) {
		if ( ( *i )->parent->getId() == getId() ) {
			std::unique_ptr<Position> pos = std::move( wlayout->getCoordinate( ( *i )->control->getId() ) );
			( *i )->control->setPosition( pos->x, pos->y, pos->width, pos->height );
		}
	}
	for ( auto i = ( ( maschinen::Window* )getWindowParent() )->getChildren()->beginLabel(); i != ( ( maschinen::Window* )getWindowParent() )->getChildren()->endLabel(); ++i ) {
		if ( ( *i )->parent->getId() == getId() ) {
			std::unique_ptr<Position> pos = std::move( wlayout->getCoordinate( ( *i )->control->getId() ) );
			( *i )->control->setPosition( pos->x, pos->y, pos->width, pos->height );
		}
	}
	for ( auto i = ( ( maschinen::Window* )getWindowParent() )->getChildren()->beginButton(); i != ( ( maschinen::Window* )getWindowParent() )->getChildren()->endButton(); ++i ) {
		if ( ( *i )->parent->getId() == getId() ) {
			std::unique_ptr<Position> pos = std::move( wlayout->getCoordinate( ( *i )->control->getId() ) );
			( *i )->control->setPosition( pos->x, pos->y, pos->width, pos->height );
		}
	}
	for ( auto i = ( ( maschinen::Window* )getWindowParent() )->getChildren()->beginEdit(); i != ( ( maschinen::Window* )getWindowParent() )->getChildren()->endEdit(); ++i ) {
		if ( ( *i )->parent->getId() == getId() ) {
			std::unique_ptr<Position> pos = std::move( wlayout->getCoordinate( ( *i )->control->getId() ) );
			( *i )->control->setPosition( pos->x, pos->y, pos->width, pos->height );
		}
	}
	for ( auto i = ((maschinen::Window*)getWindowParent())->getChildren()->beginCheckBox(); i != ( ( maschinen::Window* )getWindowParent() )->getChildren()->endCheckBox(); ++i ) {
		if ( ( *i )->parent->getId() == getId() ) {
			std::unique_ptr<Position> pos = std::move( wlayout->getCoordinate( ( *i )->control->getId() ) );
			( *i )->control->setPosition( pos->x, pos->y, pos->width, pos->height );
		}
	}
	for ( auto i = ( ( maschinen::Window* )getWindowParent() )->getChildren()->beginProgressBar(); i != ( ( maschinen::Window* )getWindowParent() )->getChildren()->endProgressBar(); ++i ) {
		if ( ( *i )->parent->getId() == getId() ) {
			std::unique_ptr<Position> pos = std::move( wlayout->getCoordinate( ( *i )->control->getId() ) );
			( *i )->control->setPosition( pos->x, pos->y, pos->width, pos->height );
		}
	}
	for ( auto i = ( ( maschinen::Window* )getWindowParent() )->getChildren()->beginTable(); i != ( ( maschinen::Window* )getWindowParent() )->getChildren()->endTable(); ++i ) {
		if ( ( *i )->parent->getId() == getId() ) {
			std::unique_ptr<Position> pos = std::move( wlayout->getCoordinate( ( *i )->control->getId() ) );
			( *i )->control->setPosition( pos->x, pos->y, pos->width, pos->height );
		}
	}
	for ( auto i = ( ( maschinen::Window* )getWindowParent() )->getChildren()->beginCustomControl(); i != ( ( maschinen::Window* )getWindowParent() )->getChildren()->endCustomControl(); ++i ) {
		if ( ( *i )->parent->getId() == getId() ) {
			std::unique_ptr<Position> pos = std::move( wlayout->getCoordinate( ( *i )->control->getId() ) );
			( *i )->control->setPosition( pos->x, pos->y, pos->width, pos->height );
		}
	}
	on_init = false;
}

void maschinen::Container::onShow() {
	for ( auto i = ((maschinen::Window*)getWindowParent())->getChildren()->beginContainer(); i != ( ( maschinen::Window* )getWindowParent() )->getChildren()->endContainer(); ++i ) {
		if ( ( *i )->parent->getId() == getId() ) ( *i )->control->show();
	}
	for ( auto i = ( ( maschinen::Window* )getWindowParent() )->getChildren()->beginLabel(); i != ( ( maschinen::Window* )getWindowParent() )->getChildren()->endLabel(); ++i ) {
		if ( ( *i )->parent->getId() == getId() ) ( *i )->control->show();
	}
	for ( auto i = ( ( maschinen::Window* )getWindowParent() )->getChildren()->beginButton(); i != ( ( maschinen::Window* )getWindowParent() )->getChildren()->endButton(); ++i ) {
		if ( ( *i )->parent->getId() == getId() ) ( *i )->control->show();
	}
	for ( auto i = ( ( maschinen::Window* )getWindowParent() )->getChildren()->beginCheckBox(); i != ( ( maschinen::Window* )getWindowParent() )->getChildren()->endCheckBox(); ++i ) {
		if ( ( *i )->parent->getId() == getId() ) ( *i )->control->show();
	}
	for ( auto i = ( ( maschinen::Window* )getWindowParent() )->getChildren()->beginProgressBar(); i != ( ( maschinen::Window* )getWindowParent() )->getChildren()->endProgressBar(); ++i ) {
		if ( ( *i )->parent->getId() == getId() ) ( *i )->control->show();
	}
	for ( auto i = ( ( maschinen::Window* )getWindowParent() )->getChildren()->beginEdit(); i != ( ( maschinen::Window* )getWindowParent() )->getChildren()->endEdit(); ++i ) {
		if ( ( *i )->parent->getId() == getId() ) ( *i )->control->show();
	}
	for ( auto i = ( ( maschinen::Window* )getWindowParent() )->getChildren()->beginTable(); i != ( ( maschinen::Window* )getWindowParent() )->getChildren()->endTable(); ++i ) {
		if ( ( *i )->parent->getId() == getId() ) ( *i )->control->show();
	}
	for ( auto i = ( ( maschinen::Window* )getWindowParent() )->getChildren()->beginCustomControl(); i != ( ( maschinen::Window* )getWindowParent() )->getChildren()->endCustomControl(); ++i ) {
		if ( ( *i )->parent->getId() == getId() ) ( *i )->control->show();
	}
}
void maschinen::Container::onHide() {
	for ( auto i = ( ( maschinen::Window* )getWindowParent() )->getChildren()->beginContainer(); i != ( ( maschinen::Window* )getWindowParent() )->getChildren()->endContainer(); ++i ) {
		if ( ( *i )->parent->getId() == getId() ) ( *i )->control->hide();
	}
	for ( auto i = ( ( maschinen::Window* )getWindowParent() )->getChildren()->beginLabel(); i != ( ( maschinen::Window* )getWindowParent() )->getChildren()->endLabel(); ++i ) {
		if ( ( *i )->parent->getId() == getId() ) ( *i )->control->hide();
	}
	for ( auto i = ( ( maschinen::Window* )getWindowParent() )->getChildren()->beginButton(); i != ( ( maschinen::Window* )getWindowParent() )->getChildren()->endButton(); ++i ) {
		if ( ( *i )->parent->getId() == getId() ) ( *i )->control->hide();
	}
	for ( auto i = ( ( maschinen::Window* )getWindowParent() )->getChildren()->beginProgressBar(); i != ( ( maschinen::Window* )getWindowParent() )->getChildren()->endProgressBar(); ++i ) {
		if ( ( *i )->parent->getId() == getId() ) ( *i )->control->hide();
	}
	for ( auto i = ( ( maschinen::Window* )getWindowParent() )->getChildren()->beginCheckBox(); i != ( ( maschinen::Window* )getWindowParent() )->getChildren()->endCheckBox(); ++i ) {
		if ( ( *i )->parent->getId() == getId() ) ( *i )->control->hide();
	}
	for ( auto i = ( ( maschinen::Window* )getWindowParent() )->getChildren()->beginEdit(); i != ( ( maschinen::Window* )getWindowParent() )->getChildren()->endEdit(); ++i ) {
		if ( ( *i )->parent->getId() == getId() ) ( *i )->control->hide();
	}
	for ( auto i = ( ( maschinen::Window* )getWindowParent() )->getChildren()->beginTable(); i != ( ( maschinen::Window* )getWindowParent() )->getChildren()->endTable(); ++i ) {
		if ( ( *i )->parent->getId() == getId() ) ( *i )->control->hide();
	}
	for ( auto i = ( ( maschinen::Window* )getWindowParent() )->getChildren()->beginCustomControl(); i != ( ( maschinen::Window* )getWindowParent() )->getChildren()->endCustomControl(); ++i ) {
		if ( ( *i )->parent->getId() == getId() ) ( *i )->control->hide();
	}
}

void maschinen::Container::create()
{
	style = WS_CHILD;
	handler = CreateWindowEx(WS_EX_CLIENTEDGE, WC_STATIC, (LPCTSTR)NULL, style, getX(), getY(), getWidth(), getHeight(), getWindowHandler(), (HMENU)(UINT_PTR)getId(), getInstance(), this);
	SetWindowSubclass( handler, containerProcessStarter, getId(), reinterpret_cast<DWORD_PTR>(this) );
	onCreate();
}

bool maschinen::Container::operator==(int type)  const noexcept{return type == CONTAINER;}
bool maschinen::Container::operator!=(int type)  const noexcept{return type != CONTAINER;}
bool maschinen::Container::compare(int type)  const noexcept{return type == CONTAINER;}

void maschinen::Container::setParent( maschinen::ContainedController* parent ) {
	maschinen::ContainedController::setParent( parent );
	( ( maschinen::Window* )parent->getWindowParent() )->getChildren()->addContainer( this, parent );
}
void maschinen::Container::setParentAndId( maschinen::ContainedController* parent, UINT id ) {
	maschinen::ContainedController::setParentAndId( parent, id );
	( ( maschinen::Window* )parent->getWindowParent() )->getChildren()->addContainer( this, parent );
}

maschinen::Container::~Container()
{
}
