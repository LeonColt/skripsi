#pragma once
#include "Maschinen.h"
#include "Controller.h"
namespace maschinen {
	namespace event {
		namespace listener {
			class MASCHINEN_API OnFocusListener {
			private:
				bool auto_delete;
			public:
				OnFocusListener();
				void setAutoDelete( bool auto_delete = false ) noexcept;
				bool isAutoDelete() const noexcept;
				virtual void onFocus(maschinen::Controller* control, bool focus) = 0;
				~OnFocusListener();
			};
		}
	}
}

