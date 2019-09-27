#pragma once
#include "ContainedController.h"
#include "Container.h"
#include "Layout.h"
#include "AsynchronousTask.h"
#include "Exception.h"
#include "ChildrenKeeper.h"
#include "AllEventHandler.h"
#include "OnClickEvent.h"
#include "OnListViewSelectEvent.h"
struct WindowEventKeeper;
namespace maschinen
{
	class Window;
	class Children;
	struct InitCellTransporter
	{
		RECT rect;
		RECT inner_rect;
		HWND handler = NULL;
	};
	class MASCHINEN_API Window : public maschinen::ContainedController {
	private:
		WNDCLASSEX window_class;
		InitCellTransporter ict;
		std::unique_ptr<Children> children;
		std::unique_ptr<WindowEventKeeper> wek;
		static int jumlah_windows;
		static bool process_started;
		bool first_show;
		void onCalculate();
		static LRESULT CALLBACK WindowProcessStarter( HWND handler, UINT message, WPARAM wparam, LPARAM lparam );
	protected:
		void onInit();
		void onHide();
		void onShow();
		void onSize( WPARAM wparam, LPARAM lparam );
		void onDestroy();
		virtual void onClose();
		virtual bool onAdditional(UINT message, WPARAM wparam, LPARAM lparam);
		virtual LRESULT WindowProcess(HWND handler, UINT message, WPARAM wparam, LPARAM lparam);
	public:
		Window();
		void setWindowProcess(WNDPROC window_process);
		void setExtraByte(int extra_byte);
		void setWindowExtraByte(int window_extra_byte);
		void setIcon(HICON icon);
		void setSmallIcon(HICON small_icon);
		void setCursor(HCURSOR cursor);
		void setBackground(HBRUSH background);
		void setMenuName(LPCTSTR menu_name);
		void setParent(Window* window);
		//begin style
		void setStyle(UINT style) noexcept;
		void setStyleByteAlignClient(bool byte_align_client = true);
		void setStyleByteAlignWindow(bool byte_align_window = true);
		void setStyleClassDC(bool class_dc = true);
		void setStyleDoubleClicks(bool double_clicks = true);
		void setStyleDropShadow(bool drop_shadow = true);
		void setStyleGlobalClass(bool global_class = true);
		void setStyleHorizontalRedraw(bool horizontal_redraw = true);
		void setStyleVerticalRedraw(bool vertical_redraw = true);
		void setStyleNoClose(bool noclose = true);
		void setStyleOwnDC(bool own_dc = true);
		void setStyleParentDC(bool parent_dc = true);
		void setStyleSaveBits(bool save_bits = true);
		//end style
		maschinen::Window* getParent() const noexcept;
		maschinen::Children* getChildren() const noexcept;
		WindowEventKeeper* getListeners() const noexcept;
		void update() const noexcept;
		void invalidate() noexcept;
		void create();

		void setTitle( LPCTSTR title );
		void setTitle( tstring& title );
		void setTitle( tstring* title );

		bool operator==( int type ) const noexcept;
		bool operator!=( int type ) const noexcept;
		bool compare( int type ) const noexcept;
		bool operator==( maschinen::Window* window ) const noexcept;
		bool operator!=( maschinen::Window* window ) const noexcept;
		bool compare( maschinen::Window* window ) const noexcept;

		//begin compare window
		bool operator==( maschinen::ContainedController& control ) const noexcept;
		bool operator!=( maschinen::ContainedController& control ) const noexcept;
		bool compare( maschinen::ContainedController& control ) const noexcept;
		bool operator==( maschinen::ContainedController* control ) const noexcept;
		bool operator!=( maschinen::ContainedController* control ) const noexcept;
		bool compare(maschinen::ContainedController* control) const noexcept;
		//end compare window
		~Window();
	};
}