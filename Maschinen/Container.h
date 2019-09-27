#pragma once
#include <CommCtrl.h>
#include "ContainedController.h"
#include "Window.h"
#include "ChildrenKeeper.h"
#include "LayoutMatrix.h"
namespace maschinen
{
	class Window;
	class MASCHINEN_API Container : public maschinen::ContainedController
	{
	private:
		static LRESULT CALLBACK containerProcessStarter( HWND handler, UINT message, WPARAM wparam, LPARAM lparam, UINT_PTR uid_sub_class, DWORD_PTR dw_ref_data );
	protected:
		LRESULT containerProcess( HWND handler, UINT message, WPARAM wparam, LPARAM lparam );
		void onShow();
		void onHide();
		void onInit();
		void onSize( WPARAM wparam, LPARAM lparam );
		void onDestroy();
	public:
		Container();
		Container( UINT id, ContainedController* parent);
		void create();
		bool operator==(int type) const noexcept;
		bool operator!=(int type) const noexcept;
		bool compare(int type) const noexcept;
		void setParent( maschinen::ContainedController* parent );
		void setParentAndId( maschinen::ContainedController* parent, UINT id );
		~Container();
	};
}

