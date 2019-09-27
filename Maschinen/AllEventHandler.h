#pragma once
#include "Maschinen.h"
#include "OnClickListener.h"
#include "OnCheckedChangeListener.h"
#include "OnListViewSelectListener.h"
#include "OnEditTextChangedListener.h"
#include "Window.h"

namespace maschinen {
	namespace event {
		namespace listener {
			class OnCheckedChangeListener;
			class OnListViewSelectListener;
		}
	}
}

struct ClickListenerKeeper {
	maschinen::event::OnClickListener* ocl;
	UINT id;
	bool auto_delete;
};

struct CheckedChangeListenerKeeper {
	maschinen::event::listener::OnCheckedChangeListener* occl;
	UINT id;
	bool auto_delete;
};

struct ListViewSelectListenerKeeper {
	maschinen::event::listener::OnListViewSelectListener* listener;
	UINT id;
	bool auto_delete;
};

struct EditTextChangedListenerKeeper {
	maschinen::event::listener::OnEditTextChangedListener* listener;
	UINT id;
	bool auto_delete;
};

struct WindowEventKeeper {
	ATOM window_id;
	std::vector<ClickListenerKeeper*> click_listener_keepers;
	std::vector<CheckedChangeListenerKeeper*> checked_change_listener_keeper;
	std::vector<ListViewSelectListenerKeeper*> table_select_keeper;
	std::vector<EditTextChangedListenerKeeper*> edit_text_changed_listener_keeper;
};

extern std::vector<WindowEventKeeper*> listener_keepers;

namespace maschinen {
	class Window;
}

MASCHINEN_API void setMenuOnClickListener( maschinen::Window* parent_window, unsigned int menu, maschinen::event::OnClickListener* on_click_listener, bool auto_delete = true );
MASCHINEN_API void unSetMenuOnClickListener( maschinen::Window* parent_window, unsigned int menu );