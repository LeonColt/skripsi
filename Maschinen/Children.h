#pragma once
#include "Maschinen.h"
#include "Label.h"
#include "Button.h"
#include "Edit.h"
#include "CheckBox.h"
#include "ProgressBar.h"
#include "Table.h"
#include "ListView.h"
#include "CheckBox.h"
#include "PopUpMenu.h"
#include "Container.h"
#include "StatusBar.h"
#include "Window.h"

namespace maschinen {
	class Window;
	class Label;
	class Button;
	class CheckBox;
	class Edit;
	namespace list_view {
		class ListViewBasic;
		class Table;
		class ListView;
	}
	namespace menu {
		class PopUpMenu;
	}
	class Container;
	class StatusBar;
	struct MASCHINEN_API LabelChildInfo {
		Label* control = nullptr;
		ContainedController* parent = nullptr;
	};
	struct MASCHINEN_API ButtonChildInfo {
		Button* control = nullptr;
		ContainedController* parent = nullptr;
	};
	struct MASCHINEN_API EditChildInfo {
		Edit* control = nullptr;
		ContainedController* parent = nullptr;
	};
	struct MASCHINEN_API CheckBoxChildInfo {
		CheckBox* control = nullptr;
		ContainedController* parent = nullptr;
	};
	struct MASCHINEN_API TableChildInfo {
		list_view::Table* control = nullptr;
		ContainedController* parent = nullptr;
	};
	struct MASCHINEN_API ListViewChildInfo {
		list_view::ListView* control = nullptr;
		ContainedController* parent = nullptr;
	};
	struct MASCHINEN_API ProgressBarChildInfo {
		ProgressBar* control = nullptr;
		ContainedController* parent = nullptr;
	};
	struct MASCHINEN_API PopUpMenuChildInfo {
		menu::PopUpMenu* control = nullptr;
		ContainedController* parent = nullptr;
	};
	struct MASCHINEN_API CustomControlChildInfo {
		UncontainedController* control = nullptr;
		ContainedController* parent = nullptr;
	};
	struct MASCHINEN_API ContainerChildInfo {
		Container* control = nullptr;
		ContainedController* parent = nullptr;
	};
	struct MASCHINEN_API StatusBarChildInfo {
		StatusBar* control = nullptr;
		ContainedController* parent = nullptr;
	};
	class MASCHINEN_API Children {
	private:
		maschinen::Window* const WDW;

		std::vector<LabelChildInfo*> label_children;
		std::vector<ButtonChildInfo*> button_children;
		std::vector<EditChildInfo*> edit_children;
		std::vector<CheckBoxChildInfo*> check_box_children;
		std::vector<ProgressBarChildInfo*> progress_bar_children;
		std::vector<PopUpMenuChildInfo*> pop_up_menu_children;
		std::vector<CustomControlChildInfo*> custom_control_children;
		std::vector<ContainerChildInfo*> container_children;
		std::vector<TableChildInfo*> table_children;
		std::vector<ListViewChildInfo*> list_view_children;
		std::vector<StatusBarChildInfo*> status_bar_children;
	public:
		Children( maschinen::Window* const WDW );

		maschinen::Window* getWindow() const noexcept;

		void addLabel( Label* control, ContainedController* parent );
		void removeLabel( Label* control );
		std::vector<LabelChildInfo*>::const_iterator beginLabel() const;
		std::vector<LabelChildInfo*>::const_iterator endLabel() const;
		size_t getLabelCount() const noexcept;

		void addButton( Button* control, ContainedController* parent );
		void removeButton( Button* control );
		std::vector<ButtonChildInfo*>::const_iterator beginButton() const;
		std::vector<ButtonChildInfo*>::const_iterator endButton() const;
		size_t getButtonCount() const noexcept;

		void addEdit( Edit* control, ContainedController* parent);
		void removeEdit( Edit* control );
		std::vector<maschinen::EditChildInfo*>::const_iterator beginEdit() const;
		std::vector<maschinen::EditChildInfo*>::const_iterator endEdit() const;
		size_t getEditCount() const noexcept;

		void addCheckBox(CheckBox* control, ContainedController* parent);
		void removeCheckBox( CheckBox* control );
		std::vector<maschinen::CheckBoxChildInfo*>::const_iterator beginCheckBox() const;
		std::vector<maschinen::CheckBoxChildInfo*>::const_iterator endCheckBox() const;
		size_t getCheckBoxCount() const noexcept;

		void addProgressBar( ProgressBar* control, ContainedController* parent );
		void removeProgressBar( ProgressBar* control );
		std::vector<maschinen::ProgressBarChildInfo*>::const_iterator beginProgressBar() const;
		std::vector<maschinen::ProgressBarChildInfo*>::const_iterator endProgressBar() const;
		size_t getProgressBarCount() const noexcept;

		void addTable(maschinen::list_view::Table* control, ContainedController* parent);
		void removeTable( maschinen::list_view::Table* control );
		std::vector<TableChildInfo*>::const_iterator beginTable() const;
		std::vector<TableChildInfo*>::const_iterator endTable() const;
		size_t getTableCount() const noexcept;

		void addListView( maschinen::list_view::ListView* control, ContainedController* parent );
		void removeListView( maschinen::list_view::ListView* control );
		std::vector<ListViewChildInfo*>::const_iterator beginListView() const;
		std::vector<ListViewChildInfo*>::const_iterator endListView() const;
		size_t getListViewCount() const noexcept;

		void addPopUpMenu( maschinen::menu::PopUpMenu* control, ContainedController* parent );
		void removePopUpMenu( maschinen::menu::PopUpMenu* control );
		std::vector<PopUpMenuChildInfo*>::const_iterator beginPopUpMenu() const;
		std::vector<PopUpMenuChildInfo*>::const_iterator endPopUpMenu() const;
		size_t getPopUpMenuCount() const noexcept;

		void addCustomControl(maschinen::UncontainedController* control, maschinen::ContainedController* parent);
		void removeCustomControl(maschinen::UncontainedController* control);
		std::vector<CustomControlChildInfo*>::const_iterator beginCustomControl() const;
		std::vector<CustomControlChildInfo*>::const_iterator endCustomControl() const;
		size_t getCustomControlCount() const noexcept;

		void addContainer( maschinen::Container* control, maschinen::ContainedController* parent );
		void removeContainer(maschinen::Container* control );
		std::vector<ContainerChildInfo*>::const_iterator beginContainer() const;
		std::vector<ContainerChildInfo*>::const_iterator endContainer() const;
		size_t getContainerCount() const noexcept;

		void addStatusBar( maschinen::StatusBar* control, maschinen::ContainedController* parent );
		void removeStatusBar( maschinen::StatusBar* status_bar );
		std::vector<StatusBarChildInfo*>::const_iterator beginStatusBar() const;
		std::vector<StatusBarChildInfo*>::const_iterator endStatusBar() const;
		size_t getStatusBarCount() const noexcept;

		~Children();
	};
}

