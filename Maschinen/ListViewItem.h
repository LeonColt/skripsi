#pragma once
#include <CommCtrl.h>
#include "Maschinen.h"
#include "ListViewBasic.h"
#include "ListViewSubItem.h"
namespace maschinen
{
	namespace list_view
	{
		class ListViewBasic;
		class MASCHINEN_API ListViewItem
		{
			friend class ListView;
		private:
			HWND handler;
			maschinen::list_view::ListView* owner = nullptr;
			std::vector<ListViewSubItem*> children;
			LVITEM item;
		public:
			ListViewItem();
			void setText( LPCTSTR text );
			void setText( tstring& text );
			void setText( tstring* text );
			void setPosition(int position);
			void setImage(int image_index);
			void setNumberofColumns(int number_of_columns);
			void setOrderColumn(UINT order[]);
			void setGroup( int group_id );
			void addSubItem( maschinen::list_view::ListViewSubItem* sub_item ) const noexcept;
			~ListViewItem();


			//begin state
			void setStateActivating( bool state = true ) noexcept;
			void setStateCut(bool state = true) noexcept;
			void setStateDropHilited( bool state = true ) noexcept;
			void setStateFocused( bool state = true ) noexcept;
			void setStateOverLayMask( bool state = true ) noexcept;
			void setStateSelected( bool state = true ) noexcept;
			void setStateImageMask( bool state = true ) noexcept;
			//end state

			//begin get
			std::unique_ptr<tstring> getText(const size_t buffer_size = 0 ) const noexcept;
			LPLVITEM get() noexcept;
			//end get
		};
	}
}