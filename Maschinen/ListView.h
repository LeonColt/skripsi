#pragma once
#include "Maschinen.h"
#include "ChildrenKeeper.h"
#include "ListViewBasic.h"
#include "ListViewItem.h"
#include "ListViewColumn.h"
#include "ListViewGroup.h"
namespace maschinen {
	namespace list_view {
		class ListView : public maschinen::list_view::ListViewBasic {
		public:
			ListView();
			ListView( UINT id, ContainedController* parent );
			void addColumn(maschinen::list_view::ListViewColumn& column) const;
			void addItem(maschinen::list_view::ListViewItem& item) const;
			void addGroup( maschinen::list_view::ListViewGroup& group ) const;
			~ListView();


			std::unique_ptr<maschinen::list_view::ListViewItem> getItem( int index );
			std::unique_ptr<maschinen::list_view::ListViewGroup> getGroupById( int group_id ) const noexcept;
			std::unique_ptr<maschinen::list_view::ListViewGroup> getGroupByIndex( int index ) const noexcept;


			void setParent( maschinen::ContainedController* parent );
			void setParentAndId( maschinen::ContainedController* parent, UINT id );
			bool operator==( int type ) const noexcept;
			bool operator!=( int type ) const noexcept;
			bool compare( int type ) const noexcept;
		};
	}
}

