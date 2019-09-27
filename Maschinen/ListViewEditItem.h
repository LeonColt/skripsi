#pragma once
#include <CommCtrl.h>
#include "Maschinen.h"
#include "AllOriginalProcessKeeper.h"
namespace maschinen {
	namespace list_view {
		class ListViewEditItem {
		private:
			const HWND PARENT_HANDLER, LV_HANDLER;
			const int ROW, COLUMN;
			HWND handler;
			QWORD position;  
			WNDPROC original_process;
			static LRESULT CALLBACK listViewEditItemProcessStarter( HWND handler, UINT message, WPARAM wparam, LPARAM lparam );
			LRESULT listViewEditItemProcess( HWND handler, UINT message, WPARAM wparam, LPARAM lparam );
		protected:
			virtual bool onGainFocus();
			virtual bool onLostFocus();
		public:
			ListViewEditItem( const HWND parent_handler, const HWND lv_handler, const int row, const int column );
			void setPosition(int x, int y, int width, int height);
			void create( DWORD style, LPCTSTR text = NULL, LPCTSTR place_holder = NULL );
			void focus() const;
			void destroy() const;
			bool isDestroyed() const noexcept;
			~ListViewEditItem();
		};
	}
}