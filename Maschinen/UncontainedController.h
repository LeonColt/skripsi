#pragma once
#include <CommCtrl.h>
#include "VisibleController.h"
#include "ContainedController.h"
namespace maschinen
{
	class MASCHINEN_API UncontainedController : public maschinen::VisibleController
	{
	public:
		UncontainedController();
		UncontainedController(unsigned int id, maschinen::ContainedController* parent);
		inline maschinen::ContainedController* getParent() const noexcept { return dynamic_cast< maschinen::ContainedController*>( parent ); }
		maschinen::ContainedController* getWindowParent() const noexcept;
		HWND getWindowHandler() const noexcept;
		virtual void setParent(maschinen::ContainedController* parent);
		virtual void setParentAndId(maschinen::ContainedController* parent, UINT id);
		void applyTabStop(bool tab_stop = true);
		~UncontainedController();
	};
}