#pragma once
#include "Maschinen.h"
#include "ContainedController.h"
namespace maschinen {
	class ContainedController;
	namespace layout {
		class MASCHINEN_API Layout {
		protected:
			maschinen::ContainedController* owner = nullptr;
		public:
			Layout();
			void setOwner( ContainedController* control ) noexcept;
			virtual void calculate(LONG x, LONG y, LONG width, LONG height) = 0;
			virtual void calculate(LPRECT rect) = 0;
			virtual void calculate() = 0;
			virtual std::unique_ptr<Position> getCoordinate( UINT id ) const = 0;
			~Layout();
		};
	}
}

