#include "stdafx.h"
#include "Layout.h"


maschinen::layout::Layout::Layout() {
}

void maschinen::layout::Layout::setOwner( ContainedController* control ) noexcept { this->owner = control; }

maschinen::layout::Layout::~Layout() {
}
