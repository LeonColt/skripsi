#include "stdafx.h"
#include "InvisibleController.h"


maschinen::InvisibleController::InvisibleController()
{
}

maschinen::ContainedController* maschinen::InvisibleController::getParent()
{
	return parent;
}

void maschinen::InvisibleController::setParent(maschinen::ContainedController* parent)
{
	this->parent = parent;
}

maschinen::InvisibleController::~InvisibleController()
{
}
