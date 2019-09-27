#pragma once
#include "Maschinen.h"
#include "Event.h"
#include "CheckBox.h"
namespace maschinen {
	class CheckBox;
	namespace event {
		namespace listener {
			class MASCHINEN_API OnCheckedChangeListener {
			public:
				OnCheckedChangeListener();
				virtual void onCheckedChanged( maschinen::CheckBox* check_box, bool is_checked ) = 0;
				~OnCheckedChangeListener();
			};
		}
	}
}

