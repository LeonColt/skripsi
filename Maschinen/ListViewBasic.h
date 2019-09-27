#pragma once
#include <CommCtrl.h>
#include "UncontainedController.h"
#include "ListViewImageList.h"
#include "NullReferenceControlObjectException.h"
#include "OnListViewSelectListener.h"
namespace maschinen
{
	namespace event {namespace listener {class OnListViewSelectListener;}}
	namespace list_view
	{
		class ListViewItem;
		class MASCHINEN_API ListViewBasic : public maschinen::UncontainedController
		{
		protected:
			DWORD flag, mask, extended_style;
			int jumlah_column;
		public:
			ListViewImageList image_list;
			ListViewBasic();
			ListViewBasic( UINT id, maschinen::ContainedController* parent);
			virtual void setOnListViewSelectListener( maschinen::event::listener::OnListViewSelectListener* listener, bool auto_delete = false ) = 0;
			virtual void create();
			void setMask(DWORD mask);
			void setFlag(DWORD flag);
			void applyImageListView();
			void applyImageListViewSmall();
			void applyImageListViewState();
			void applyImageListViewGroupHeader();
			void deleteAllItems() const noexcept;

			//begin extended style
			void twoClickActive(bool two_click_active = true);
			void setStyleAlignTop( bool style = true );
			//end extended style

			//begin item state
			void setItemStateCutPaste(int position);
			void setItemStateDragDrop(int position);
			void setItemStateFocused(int position);
			void setItemStateSelected(int position);
			//end item state

			UINT getSelectedCount() const noexcept;
			std::unique_ptr<std::vector<int>> getSelectedItemIndex() const noexcept;
			int getFocusedItemIndex() const noexcept;
			int getGroupCount() const noexcept;

			bool operator==( int type ) const noexcept;
			bool operator!=( int type ) const noexcept;
			bool compare( int type ) const noexcept;

			~ListViewBasic();
		};
	}
}