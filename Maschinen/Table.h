#pragma once
#include <windowsx.h>
#include "ListViewBasic.h"
#include "ListViewEditItem.h"
#include "ChildrenKeeper.h"
#include "AllOriginalProcessKeeper.h"
#include "AllEventHandler.h"
namespace maschinen
{
	namespace list_view
	{
		class MASCHINEN_API Table : public maschinen::list_view::ListViewBasic
		{
		private:
			bool auto_width, cell_editable;
			std::unique_ptr<ListViewEditItem> lvei;
			static LRESULT CALLBACK TableProcessStarter( HWND handler, UINT message, WPARAM wparam, LPARAM lparam, UINT_PTR uid_sub_class, DWORD_PTR dw_ref_data );
		protected:
			virtual void onDestroy();
			virtual LRESULT TableProcess( HWND handler, UINT message, WPARAM wparam, LPARAM lparam );
		public:
			Table();
			Table(UINT id, maschinen::ContainedController* parent);
			void setOnListViewSelectListener( maschinen::event::listener::OnListViewSelectListener* listener, bool auto_delete = false );
			void create();
			void addColumn( LPCTSTR text );
			void addColumn( LPCTSTR text, int width );
			void addRow();
			void autoWidth(bool auto_width = true);
			bool isAutoWidth();
			void setText(LPCTSTR text, int row, int column) const;
			void setText( tstring& text, int row, int column ) const;
			void setText( tstring* text, int row, int column ) const;
			std::unique_ptr<tstring> getText( int row, int column ) const;

			//begin extended style
			void applyShowGridLines(bool show_grid_lines = true);
			void applyHeaderDragDrop(bool header_drag_drop = true);
			void applyFullRowSelect(bool full_row_select = true);
			void applyCellEditable( bool cell_editable = true );
			//end extended style

			//begin get
			void deleteRow( int index );
			int getRowCount();
			int getColumnCount();
			UINT getSelectedItemCount() const;
			UINT getSelectedColumn() const;
			//end get

			Position getCellCoordinate(int row, int column) const noexcept;
			void setEditOnCell( int row, int column, DWORD edit_style, bool empty_edit = false, LPCTSTR place_holder = NULL );
			void setEditOnCell(int row, int column, bool empty_edit = false, LPCTSTR place_holder = NULL, bool is_number = false);

			void setParent( maschinen::ContainedController* parent );
			void setParentAndId( maschinen::ContainedController* parent, UINT id );
			bool operator==( int type ) const noexcept;
			bool operator!=( int type ) const noexcept;
			bool compare( int type ) const noexcept;

			~Table();
		};
	}
}