#pragma once
#include <CommCtrl.h>
#include "Maschinen.h"
namespace maschinen {
	namespace list_view {
		class MASCHINEN_API ListViewSubItem {
			friend class ListViewItem;
		private:
			LVITEM sub_item;
			HWND handler;
		public:
			ListViewSubItem();
			void setText( LPCTSTR text ) noexcept;
			void setText( tstring* text ) noexcept;
			void setText( tstring& text ) noexcept;
			void setPosition( int position ) noexcept;
			LPLVITEM get() noexcept;
			~ListViewSubItem();
		};
	}
}

