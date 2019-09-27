#pragma once
#include "Maschinen.h"
#include "OnClickEvent.h"
namespace maschinen {
	namespace event {
		class MASCHINEN_API OnClickListener {
		public:
			OnClickListener();
			virtual void onClick(OnClickEvent& e) = 0;
			~OnClickListener();
		};
	}
}

