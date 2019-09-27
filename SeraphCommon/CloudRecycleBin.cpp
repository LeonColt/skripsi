#include "stdafx.h"
#include "CloudRecycleBin.h"


CloudRecycleBin::CloudRecycleBin() {}

CloudRecycleBin::~CloudRecycleBin() {
	for ( auto i = children.begin(); i != children.end(); ++i )
		delete *i;
	children.clear();
}
std::list<BaseFile*>::const_iterator CloudRecycleBin::begin() const noexcept { return children.cbegin(); }
std::list<BaseFile*>::const_iterator CloudRecycleBin::end() const noexcept { return children.cend(); }
bool CloudRecycleBin::hasAnyChildren() const noexcept { return children.size() > 0; }

void CloudRecycleBin::addChildren( BaseFile* child ) {
	children.push_back( child );
}