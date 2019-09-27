#pragma once
#include "Maschinen.h"
#include "OnListViewSelectEvent.h"
namespace maschinen {
	namespace event {
		class OnListViewSelectEvent;
		namespace listener {
			class MASCHINEN_API OnListViewSelectListener {
			public:
				OnListViewSelectListener();
				virtual void onListViewSelect( maschinen::event::OnListViewSelectEvent& e ) = 0;
				~OnListViewSelectListener();
			};
		}
	}
}

