#include "stdafx.h"
#include "OnKeyPressedListener.h"


maschinen::event::listener::OnKeyPressedListener::OnKeyPressedListener() {
}

void maschinen::event::listener::OnKeyPressedListener::setAutoDelete( bool auto_delete ) noexcept {
	this->auto_delete = auto_delete;
}

bool maschinen::event::listener::OnKeyPressedListener::isAutoDelete() const noexcept {
	return auto_delete;
}

maschinen::event::listener::OnKeyPressedListener::~OnKeyPressedListener() {
}
