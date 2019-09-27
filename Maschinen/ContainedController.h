#pragma once
#include "VisibleController.h"
#include "Layout.h"
namespace maschinen
{
	namespace layout {
		class Layout;
	}
	class MASCHINEN_API ContainedController : public maschinen::VisibleController
	{
	protected:
		bool on_init = true;
		maschinen::layout::Layout* wlayout = nullptr;
		virtual void onCreate() = 0;
		virtual void onInit() = 0;
		virtual void onHide() = 0;
		virtual void onShow() = 0;
		virtual void onSize( WPARAM wparam, LPARAM lparam ) = 0;
		virtual void onPaint( HDC hdc );
		virtual void onDestroy() = 0;
	public:
		ContainedController();
		ContainedController(UINT id, ContainedController* parent);
		virtual void create() = 0;
		HWND getWindowHandler();
		inline ContainedController* getParent() const noexcept { return dynamic_cast< ContainedController* >( parent ); }
		ContainedController* getWindowParent() noexcept;
		void setParentAndId(maschinen::ContainedController* parent, UINT id);
		void setParent(maschinen::ContainedController* parent);
		void setLayout( maschinen::layout::Layout* layout ) noexcept;
		maschinen::layout::Layout* getLayout() const noexcept;
		~ContainedController();
	};
}