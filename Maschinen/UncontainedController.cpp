#include "stdafx.h"
#include "UncontainedController.h"


maschinen::UncontainedController::UncontainedController(){}
maschinen::UncontainedController::UncontainedController(unsigned int id, maschinen::ContainedController* parent) : maschinen::VisibleController(id, parent){}

maschinen::ContainedController* maschinen::UncontainedController::getWindowParent() const noexcept {
	maschinen::ContainedController* temp = ( ( maschinen::ContainedController* )parent );
	while ( !temp->compare( WINDOW ) ) temp = temp->getParent();
	return temp;
}
HWND maschinen::UncontainedController::getWindowHandler() const noexcept {
	maschinen::ContainedController* temp = ((maschinen::ContainedController*)parent);
	while (!temp->compare(WINDOW)) temp = temp->getParent();
	return temp->getHandler();
}

void maschinen::UncontainedController::setParentAndId(maschinen::ContainedController* parent, UINT id)
{
	this->parent = parent;
	setId(id);
}

void maschinen::UncontainedController::setParent(maschinen::ContainedController* parent){this->parent = parent;}

void maschinen::UncontainedController::applyTabStop(bool tab_stop)
{
	if (tab_stop) style |= WS_TABSTOP;
	else
	{
		style |= WS_TABSTOP;
		style ^= WS_TABSTOP;
	}
}

maschinen::UncontainedController::~UncontainedController()
{
}
