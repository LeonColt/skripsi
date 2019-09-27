#include "stdafx.h"
#include "OnListViewSelectEvent.h"


maschinen::event::OnListViewSelectEvent::OnListViewSelectEvent( const UINT id, const DWORD method, const maschinen::list_view::ListViewBasic* const lv ) :
	Event(id),
	LIST_VIEW_LEFT_CLICK(NM_CLICK), 
	LIST_VIEW_DOUBLE_CLICK(NM_DBLCLK), 
	LIST_VIEW_RIGHT_CLICK(NM_RCLICK), LIST_VIEW_DOUBLE_RIGHT_CLICK(NM_RDBLCLK), METHOD(method), LV(lv) {
	int index = -1;
	do {
		index = ListView_GetNextItem( lv->getHandler(), index, LVNI_SELECTED );
		if ( index != -1 ) selected_index.push_back( index );
	} while ( index != -1 );
}

const DWORD maschinen::event::OnListViewSelectEvent::getMethod() const noexcept {return METHOD;}
const maschinen::list_view::ListViewBasic* const maschinen::event::OnListViewSelectEvent::getListView() const noexcept {return LV;}
std::vector<int>::const_iterator maschinen::event::OnListViewSelectEvent::beginSelected() const noexcept { return selected_index.cbegin(); }
std::vector<int>::const_iterator maschinen::event::OnListViewSelectEvent::endSelected() const noexcept { return selected_index.cend(); }

maschinen::event::OnListViewSelectEvent::~OnListViewSelectEvent() {
}
