#include "stdafx.h"
#include "Controller.h"


maschinen::Controller::Controller(){}
maschinen::Controller::Controller( UINT id){this->id = id;}
bool maschinen::Controller::operator==(int type) const noexcept {return type == UNKNOW_TYPE;}
bool maschinen::Controller::operator!=(int type) const noexcept {return type != UNKNOW_TYPE;}
bool maschinen::Controller::compare(int type) const noexcept {return type == UNKNOW_TYPE;}

maschinen::Controller::~Controller()
{
}
