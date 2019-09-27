#pragma once
#include <CommCtrl.h>
#include "Maschinen.h"
#include "Event.h"
#include "ListViewBasic.h"
namespace maschinen {
	namespace list_view {
		class ListViewBasic;
	}
	namespace event {
		class MASCHINEN_API OnListViewSelectEvent : public maschinen::event::Event {
		private:
			const DWORD METHOD;
			const maschinen::list_view::ListViewBasic* const LV;
			std::vector<int> selected_index;
		public:
			const DWORD LIST_VIEW_LEFT_CLICK, LIST_VIEW_DOUBLE_CLICK, LIST_VIEW_RIGHT_CLICK, LIST_VIEW_DOUBLE_RIGHT_CLICK;
			OnListViewSelectEvent(const UINT id, const DWORD method, const maschinen::list_view::ListViewBasic* const lv);
			const DWORD getMethod() const noexcept;
			const maschinen::list_view::ListViewBasic* const getListView() const noexcept;
			std::vector<int>::const_iterator beginSelected() const noexcept;
			std::vector<int>::const_iterator endSelected() const noexcept;
			~OnListViewSelectEvent();
		};
	}
}

