#pragma once
#include "ContainedController.h"
namespace maschinen
{
	class Descartes;
	class MASCHINEN_API InvisibleController : public maschinen::Controller
	{
	protected:
		maschinen::ContainedController* parent;
	public:
		InvisibleController();
		void setParent(maschinen::ContainedController* parent);
		maschinen::ContainedController* getParent();
		~InvisibleController();
	};
}

