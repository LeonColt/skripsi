#include "stdafx.h"
#include "Event.h"


maschinen::event::Event::Event( const UINT id ) : ID(id) {}

const UINT maschinen::event::Event::getSourceId() const noexcept {
	return ID;
}

maschinen::event::Event::~Event() {
}
