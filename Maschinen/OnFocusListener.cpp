#include "stdafx.h"
#include "OnFocusListener.h"


maschinen::event::listener::OnFocusListener::OnFocusListener() {
}

void maschinen::event::listener::OnFocusListener::setAutoDelete( bool auto_delete ) noexcept{this->auto_delete = auto_delete;}
bool maschinen::event::listener::OnFocusListener::isAutoDelete() const noexcept{return auto_delete;}

maschinen::event::listener::OnFocusListener::~OnFocusListener() {
}
