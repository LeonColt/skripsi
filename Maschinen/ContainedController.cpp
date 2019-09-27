#include "stdafx.h"
#include "ContainedController.h"

maschinen::ContainedController::ContainedController(){}
maschinen::ContainedController::ContainedController( UINT id, ContainedController* parent) : maschinen::VisibleController(id, parent){}

HWND maschinen::ContainedController::getWindowHandler() {
	if ( this->compare( WINDOW ) ) return getHandler();
	else {
		ContainedController* des = ( ( maschinen::ContainedController* )parent );
		while ( !des->compare( WINDOW ) )
			des = des->getParent();
		return des->getHandler();
	}
}

maschinen::ContainedController* maschinen::ContainedController::getWindowParent() noexcept {
	maschinen::ContainedController* con = this;
	while ( con->getParent() != nullptr ) {
		con = con->getParent();
	}
	return con;
}

void maschinen::ContainedController::setParentAndId( ContainedController* parent, UINT id)
{
	this->parent = parent;
	setId(id);
}

void maschinen::ContainedController::setParent(maschinen::ContainedController* parent)
{
	this->parent = parent;
}

void maschinen::ContainedController::setLayout( maschinen::layout::Layout* layout ) noexcept { this->wlayout = layout; layout->setOwner( this ); }
maschinen::layout::Layout* maschinen::ContainedController::getLayout() const noexcept { return wlayout; }
void maschinen::ContainedController::onPaint( HDC hdc ) {}

maschinen::ContainedController::~ContainedController()
{
	if ( wlayout ) delete wlayout;
}
