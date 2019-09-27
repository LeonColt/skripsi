#pragma once
#include "Maschinen.h"
#include "Controller.h"
namespace maschinen {
	namespace event {
		namespace listener {
			class MASCHINEN_API OnKeyPressedListener {
			private:
				bool auto_delete;
			public:
				OnKeyPressedListener();
				void setAutoDelete( bool auto_delete = false ) noexcept;
				bool isAutoDelete() const noexcept;
				virtual void onKeyDown( maschinen::Controller* control, WPARAM key ) = 0;
				virtual void onKeyUp( maschinen::Controller* control, WPARAM key ) = 0;
				~OnKeyPressedListener();
			};
		}
	}
}

