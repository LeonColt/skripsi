#pragma once
#include "Maschinen.h"
#include "Event.h"
namespace maschinen {
	namespace event {
		class OnClickEvent : public maschinen::event::Event {
		public:
			OnClickEvent(const UINT id);
			~OnClickEvent();
		};
	}
}

