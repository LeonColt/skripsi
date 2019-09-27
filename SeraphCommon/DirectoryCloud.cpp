#include "stdafx.h"
#include "DirectoryCloud.h"


DirectoryCloud::DirectoryCloud() {
}

bool DirectoryCloud::create() {return false;}
bool DirectoryCloud::hasAnyChildren() { return children.size() > 0; }
void DirectoryCloud::addChildren( Cloud* child ) { 
	bool exists = false;
	for ( auto i = begin(); i != end(); ++i ) 
	{
		if ( ( *i )->getId() == child->getId() ) 
		{
			exists = true; break;
		}
	}
	if ( exists ) 
		throw maschinen::exception::Exception( TEXT( "Child Already Exists" ) );
	else children.push_back( child );}
void DirectoryCloud::addChildren( Cloud& child ) { 
	bool exists = false;
	for ( auto i = begin(); i != end(); ++i ) {
		if ( ( *i )->getId() == child.getId() ) {
			exists = true; break;
		}
	}
	if ( exists ) throw maschinen::exception::Exception( TEXT( "Child Already Exists" ) );
	else children.push_back( &child );
}
std::list<Cloud*>* DirectoryCloud::getChildren() noexcept {return &children;}
Cloud* DirectoryCloud::getChild( QWORD id ) const noexcept {
	for ( auto i = begin(); i != end(); i++ ) {
		if ( ( *i )->getId() == id ) return *i;
	}
	return nullptr;
}

bool DirectoryCloud::hasChildren( Cloud* child ) const noexcept { 
	if ( child != nullptr ) {
		for ( auto i = begin(); i != end(); i++ ) {
			if ( ( *i )->getId() == child->getId() ) return true;
		}
	}
	return false;
}
bool DirectoryCloud::hasChildren( QWORD id ) const noexcept {for ( auto i = begin(); i != end(); i++ ) {if ( ( *i )->getId() == id ) return true;}return false;}
std::list<Cloud*>::const_iterator DirectoryCloud::begin() const noexcept { return children.cbegin(); }
std::list<Cloud*>::const_iterator DirectoryCloud::end() const noexcept { return children.cend(); }

Cloud* DirectoryCloud::findChildrenById( QWORD id, Cloud* begin_search ) {
	Cloud* node = nullptr;
	if ( begin_search == nullptr ) node = this; else node = begin_search;
	if ( node->getId() == id ) return node;
	else if ( DirectoryCloud* dc = dynamic_cast< DirectoryCloud* >( node ) ) {
		for ( auto i = dc->begin(); i != dc->end(); i++ ) {
			if ( ( *i )->getId() == id ) return *i;
		}
		for ( auto i = dc->begin(); i != dc->end(); i++ ) {
			node = findChildrenById( id, *i );
			if ( node != nullptr ) {
				if ( node->getId() == id ) return node;
			}
		}
	}
	return nullptr;
}

void DirectoryCloud::clear() {
	for ( auto i = children.begin(); i != children.end(); ++i ) {
		if ( *i != nullptr ) {
			delete *i;
			*i = nullptr;
		}
	}
	children.clear();
}

DirectoryCloud::~DirectoryCloud() {
	clear();
}

void DirectoryCloud::erase( Cloud* child ) {
	if ( child == nullptr ) return;
	if ( !hasChildren( child ) ) return;
	children.erase( std::remove( children.begin(), children.end(), child ), children.end() );
	delete child;
	child = nullptr;
}

void DirectoryCloud::erase( QWORD id ) {
	Cloud* child = nullptr;
	for ( auto i = begin(); i != end(); i++ ) {
		if ( ( *i )->getId() == id ) {
			child = *i; break;
		}
	}
	if ( child ) {
		children.erase( std::remove( children.begin(), children.end(), child ), children.end() );
		delete child;
		child = nullptr;
	}
}

void DirectoryCloud::operator=( DirectoryCloud& cloud ) noexcept {
	Cloud::operator=( cloud );
	children.assign( cloud.children.begin(), cloud.children.end() );
}
void DirectoryCloud::operator=( DirectoryCloud* cloud ) noexcept{
	Cloud::operator=( cloud );
	children.assign( cloud->children.begin(), cloud->children.end() );
}
void DirectoryCloud::assign( DirectoryCloud& cloud ) noexcept{
	Cloud::assign( cloud );
	children.assign( cloud.children.begin(), cloud.children.end() );
}
void DirectoryCloud::assign( DirectoryCloud* cloud ) noexcept{
	Cloud::assign( cloud );
	children.assign( cloud->children.begin(), cloud->children.end() );
}