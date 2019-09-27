#include "stdafx.h"
#include "ListView.h"


maschinen::list_view::ListView::ListView() {
}

maschinen::list_view::ListView::ListView( UINT id, ContainedController* parent ) : maschinen::list_view::ListViewBasic(id, parent) {
	if ( parent->compare( WINDOW ) ) {
		Children* children = ( ( maschinen::Window* )parent )->getChildren();
		children->addListView( this, parent );
	}
	else {
		for ( auto i = window_children.begin(); i != window_children.end(); ++i ) {
			if ( ( *i )->getWindow()->getId() == parent->getWindowParent()->getId() )
				( *i )->getWindow()->getChildren()->addListView( this, parent );
		}
	}
}

void maschinen::list_view::ListView::addColumn( maschinen::list_view::ListViewColumn& column ) const {
	if ( ListView_InsertColumn( handler, column.getPosition(), column.get() ) == -1 ) {
		TCHAR buffer[ MAX_LOADSTRING ];
		DWORD error_code;
		if ( getLastErrorAsString( buffer, MAX_LOADSTRING, error_code ) )
			throw maschinen::exception::Exception( buffer, error_code );
		else throw maschinen::exception::Exception();
	}
}
void maschinen::list_view::ListView::addItem( maschinen::list_view::ListViewItem& item ) const {
	if ( ListView_InsertItem( handler, item.get() ) == -1 ) {
		TCHAR buffer[ MAX_LOADSTRING ];
		DWORD error_code;
		if ( getLastErrorAsString( buffer, MAX_LOADSTRING, error_code ) )
			throw maschinen::exception::Exception( buffer, error_code );
		else throw maschinen::exception::Exception();
	}
}
void maschinen::list_view::ListView::addGroup( maschinen::list_view::ListViewGroup& group ) const {
	if ( ListView_EnableGroupView( handler, TRUE ) == -1 ) {
		TCHAR buffer[ MAX_LOADSTRING ];
		DWORD error_code;
		if ( getLastErrorAsString( buffer, MAX_LOADSTRING, error_code ) )
			throw maschinen::exception::Exception( buffer, error_code );
		else throw maschinen::exception::Exception();
	}
	if ( ListView_InsertGroup( handler, -1, group.get() ) == -1 ) {
		TCHAR buffer[ MAX_LOADSTRING ];
		DWORD error_code;
		if ( getLastErrorAsString( buffer, MAX_LOADSTRING, error_code ) )
			throw maschinen::exception::Exception( buffer, error_code );
		else throw maschinen::exception::Exception();
	}
}


maschinen::list_view::ListView::~ListView() {
}


std::unique_ptr<maschinen::list_view::ListViewGroup> maschinen::list_view::ListView::getGroupById( int id ) const noexcept {
	std::unique_ptr<maschinen::list_view::ListViewGroup> lvg = std::make_unique<maschinen::list_view::ListViewGroup>();
	ListView_GetGroupInfo( handler, id, lvg->get() );
	return lvg;
}
std::unique_ptr<maschinen::list_view::ListViewGroup> maschinen::list_view::ListView::getGroupByIndex( int index ) const noexcept {
	std::unique_ptr<maschinen::list_view::ListViewGroup> lvg = std::make_unique<maschinen::list_view::ListViewGroup>();
	ListView_GetGroupInfoByIndex( handler, index, lvg->get() );
	return lvg;
}
std::unique_ptr<maschinen::list_view::ListViewItem> maschinen::list_view::ListView::getItem( int index ) {
	std::unique_ptr<maschinen::list_view::ListViewItem> item = std::make_unique<maschinen::list_view::ListViewItem>();
	if ( index < ListView_GetItemCount( handler ) ) {
		//item->handler = handler;
		return item;
	}
	else return nullptr;
	return item;
}

void maschinen::list_view::ListView::setParent( maschinen::ContainedController* parent ) {
	maschinen::UncontainedController::setParent( parent );
	if ( parent->compare( WINDOW ) ) {
		Children* children = ( ( maschinen::Window* )parent )->getChildren();
		children->addListView( this, parent );
	}
	else {
		for ( auto i = window_children.begin(); i != window_children.end(); ++i ) {
			if ( ( *i )->getWindow()->getId() == parent->getWindowParent()->getId() )
				( *i )->getWindow()->getChildren()->addListView( this, parent );
		}
	}
}
void maschinen::list_view::ListView::setParentAndId( maschinen::ContainedController* parent, UINT id ) {
	maschinen::UncontainedController::setParentAndId( parent, id );
}
bool maschinen::list_view::ListView::operator==( int type ) const noexcept { return type == LISTVIEW; }
bool maschinen::list_view::ListView::operator!=( int type ) const noexcept { return type != LISTVIEW; }
bool maschinen::list_view::ListView::compare( int type ) const noexcept { return type == LISTVIEW; }
