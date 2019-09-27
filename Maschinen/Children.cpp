#include "stdafx.h"
#include "Children.h"


maschinen::Children::Children( maschinen::Window* const WDW ) : WDW(WDW) {
}

maschinen::Window* maschinen::Children::getWindow() const noexcept {return WDW;}

void maschinen::Children::addLabel( maschinen::Label* control, ContainedController* parent ) {
	LabelChildInfo* lci = new LabelChildInfo();
	lci->control = control;
	lci->parent = parent;
	label_children.push_back( lci );
}
void maschinen::Children::removeLabel( maschinen::Label* control ) {
	LabelChildInfo* lci = nullptr;
	for ( auto i = label_children.begin(); i != label_children.end(); ++i ) {
		if ( control->getId() == ( *i )->control->getId() ) {lci = *i; break;}
	}
	if ( lci ) {
		label_children.erase( std::remove( label_children.begin(), label_children.end(), lci ), label_children.end() );
		delete lci;
	}
}
std::vector<maschinen::LabelChildInfo*>::const_iterator maschinen::Children::beginLabel() const { return label_children.cbegin(); }
std::vector<maschinen::LabelChildInfo*>::const_iterator maschinen::Children::endLabel() const { return label_children.cend(); }
size_t maschinen::Children::getLabelCount() const noexcept { return label_children.size(); }

void maschinen::Children::addButton( Button* control, ContainedController* parent ) {
	ButtonChildInfo* bci = new ButtonChildInfo();
	bci->control = control;
	bci->parent = parent;
	button_children.push_back( bci );
}
void maschinen::Children::removeButton( Button* control ) {
	ButtonChildInfo* bci = nullptr;
	for ( auto i = button_children.begin(); i != button_children.end(); ++i ) {
		if ( control->getId() == ( *i )->control->getId() ) { bci = *i; break; }
	}
	if ( bci ) {
		button_children.erase( std::remove( button_children.begin(), button_children.end(), bci ), button_children.end() );
		delete bci;
	}
}
std::vector<maschinen::ButtonChildInfo*>::const_iterator maschinen::Children::beginButton() const { return button_children.cbegin(); }
std::vector<maschinen::ButtonChildInfo*>::const_iterator maschinen::Children::endButton() const { return button_children.cend(); }
size_t maschinen::Children::getButtonCount() const noexcept { return button_children.size(); }

void maschinen::Children::addEdit( Edit* control, ContainedController* parent ) {
	EditChildInfo* eci = new EditChildInfo();
	eci->control = control;
	eci->parent = parent;
	edit_children.push_back( eci );
}
void maschinen::Children::removeEdit( Edit* control ) {
	EditChildInfo* eci = nullptr;
	for ( auto i = edit_children.begin(); i != edit_children.end(); ++i ) {
		if ( control->getId() == ( *i )->control->getId() ) { eci = *i; break; }
	}
	if ( eci ) {
		edit_children.erase( std::remove( edit_children.begin(), edit_children.end(), eci ), edit_children.end() );
		delete eci;
	}
}
std::vector<maschinen::EditChildInfo*>::const_iterator maschinen::Children::beginEdit() const { return edit_children.cbegin(); }
std::vector<maschinen::EditChildInfo*>::const_iterator maschinen::Children::endEdit() const { return edit_children.cend(); }
size_t maschinen::Children::getEditCount() const noexcept { return edit_children.size(); }

void maschinen::Children::addCheckBox( CheckBox* control, ContainedController* parent ) {
	CheckBoxChildInfo* cbi = new CheckBoxChildInfo();
	cbi->control = control;
	cbi->parent = parent;
	check_box_children.push_back( cbi );
}
void maschinen::Children::removeCheckBox( CheckBox* control ) {
	CheckBoxChildInfo* cbi = nullptr;
	for ( auto i = check_box_children.begin(); i != check_box_children.end(); ++i ) {
		if ( control->getId() == ( *i )->control->getId() ) { cbi = *i; break; }
	}
	if ( cbi ) {
		check_box_children.erase( std::remove( check_box_children.begin(), check_box_children.end(), cbi ), check_box_children.end() );
		delete cbi;
	}
}
std::vector<maschinen::CheckBoxChildInfo*>::const_iterator maschinen::Children::beginCheckBox() const { return check_box_children.cbegin(); }
std::vector<maschinen::CheckBoxChildInfo*>::const_iterator maschinen::Children::endCheckBox() const { return check_box_children.cend(); }
size_t maschinen::Children::getCheckBoxCount() const noexcept { return check_box_children.size(); }

void maschinen::Children::addProgressBar( ProgressBar* control, ContainedController* parent ) {
	ProgressBarChildInfo* pbci = new ProgressBarChildInfo();
	pbci->control = control;
	pbci->parent = parent;
	progress_bar_children.push_back( pbci );
}
void maschinen::Children::removeProgressBar( ProgressBar* control ) {
	ProgressBarChildInfo* pbci = nullptr;
	for ( auto i = progress_bar_children.cbegin(); i != progress_bar_children.cend(); ++i ) {
		if ( control->getId() == ( *i )->control->getId() ) { pbci = *i; break; }
	}
	if ( pbci ) {
		progress_bar_children.erase( std::remove( progress_bar_children.begin(), progress_bar_children.end(), pbci ), progress_bar_children.end() );
		delete pbci;
	}
}
std::vector<maschinen::ProgressBarChildInfo*>::const_iterator maschinen::Children::beginProgressBar() const { return progress_bar_children.cbegin(); }
std::vector<maschinen::ProgressBarChildInfo*>::const_iterator maschinen::Children::endProgressBar() const { return progress_bar_children.cend(); }
size_t maschinen::Children::getProgressBarCount() const noexcept { return progress_bar_children.size(); }

void maschinen::Children::addTable( maschinen::list_view::Table* control, ContainedController* parent ) {
	TableChildInfo* lvd = new TableChildInfo();
	lvd->control = control;
	lvd->parent = parent;
	table_children.push_back( lvd );
}
void maschinen::Children::removeTable( maschinen::list_view::Table* control ) {
	TableChildInfo* lvd = nullptr;
	for ( auto i = table_children.begin(); i != table_children.end(); ++i ) {
		if ( control->getId() == ( *i )->control->getId() ) {lvd = *i; break;}
	}
	if ( lvd ) {
		table_children.erase( std::remove( table_children.begin(), table_children.end(), lvd ), table_children.end() );
		delete lvd;
	}
}
std::vector<maschinen::TableChildInfo*>::const_iterator maschinen::Children::beginTable() const { return table_children.cbegin(); }
std::vector<maschinen::TableChildInfo*>::const_iterator maschinen::Children::endTable() const { return table_children.cend(); }
size_t maschinen::Children::getTableCount() const noexcept { return table_children.size(); }

void maschinen::Children::addListView( maschinen::list_view::ListView* control, ContainedController* parent ) {
	ListViewChildInfo* lv = new ListViewChildInfo();
	lv->control = control;
	lv->parent = parent;
	list_view_children.push_back( lv );
}
void maschinen::Children::removeListView( maschinen::list_view::ListView* control ) {
	ListViewChildInfo* lv = nullptr;
	for ( auto i = list_view_children.cbegin(); i != list_view_children.cend(); ++i ) {
		if ( control->getId() == ( *i )->control->getId() ) { lv = *i; break; }
	}
	if ( lv ) {
		list_view_children.erase( std::remove( list_view_children.begin(), list_view_children.end(), lv ), list_view_children.end() );
		delete lv;
	}
}
std::vector<maschinen::ListViewChildInfo*>::const_iterator maschinen::Children::beginListView() const { return list_view_children.cbegin(); }
std::vector<maschinen::ListViewChildInfo*>::const_iterator maschinen::Children::endListView() const { return list_view_children.cend(); }
size_t maschinen::Children::getListViewCount() const noexcept { return list_view_children.size(); }

void maschinen::Children::addPopUpMenu( maschinen::menu::PopUpMenu* control, ContainedController* parent ) {
	PopUpMenuChildInfo* pmci = new PopUpMenuChildInfo();
	pmci->control = control;
	pmci->parent = parent;
	pop_up_menu_children.push_back( pmci );
}
void maschinen::Children::removePopUpMenu( maschinen::menu::PopUpMenu* control ) {
	PopUpMenuChildInfo* pmci = nullptr;
	for ( auto i = pop_up_menu_children.begin(); i != pop_up_menu_children.end(); ++i ) {
		if ( control->getHandler() == ( *i )->control->getHandler() ) { pmci = *i; break; }
	}
	if ( pmci ) {
		pop_up_menu_children.erase( std::remove( pop_up_menu_children.begin(), pop_up_menu_children.end(), pmci ), pop_up_menu_children.end() );
		delete pmci;
	}
}
std::vector<maschinen::PopUpMenuChildInfo*>::const_iterator maschinen::Children::beginPopUpMenu() const { return pop_up_menu_children.cbegin(); }
std::vector<maschinen::PopUpMenuChildInfo*>::const_iterator maschinen::Children::endPopUpMenu() const { return pop_up_menu_children.cend(); }
size_t maschinen::Children::getPopUpMenuCount() const noexcept { return pop_up_menu_children.size(); }

void maschinen::Children::addCustomControl( maschinen::UncontainedController* control, maschinen::ContainedController* parent ) {
	CustomControlChildInfo* ccci = new CustomControlChildInfo();
	ccci->control = control;
	ccci->parent = parent;
	custom_control_children.push_back( ccci );
}
void maschinen::Children::removeCustomControl( maschinen::UncontainedController* control ) {
	CustomControlChildInfo* ccci = nullptr;
	for ( auto i = custom_control_children.begin(); i != custom_control_children.end(); ++i ) {
		if ( control->getId() == ( *i )->control->getId() ) {
			ccci = *i; break;
		}
	}
	if ( ccci ) {
		custom_control_children.erase( std::remove( custom_control_children.begin(), custom_control_children.end(), ccci ), custom_control_children.end() );
		delete ccci;
	}
}
std::vector<maschinen::CustomControlChildInfo*>::const_iterator maschinen::Children::beginCustomControl() const { return custom_control_children.cbegin(); }
std::vector<maschinen::CustomControlChildInfo*>::const_iterator maschinen::Children::endCustomControl() const { return custom_control_children.cend(); }
size_t  maschinen::Children::getCustomControlCount() const noexcept { return custom_control_children.size(); }

void maschinen::Children::addContainer( maschinen::Container* control, maschinen::ContainedController* parent ) {
	ContainerChildInfo* cci = new ContainerChildInfo();
	cci->control = control;
	cci->parent = parent;
	container_children.push_back( cci );
}
void maschinen::Children::removeContainer( maschinen::Container* control ) {
	ContainerChildInfo* cci = nullptr;
	for ( auto i = container_children.begin(); i != container_children.end(); ++i ) {
		if ( control->getId() == ( *i )->control->getId() ) {
			cci = *i;break;
		}
	}
	if ( cci ) {
		container_children.erase( std::remove( container_children.begin(), container_children.end(), cci ), container_children.end() );
		delete cci;
	}
}
std::vector<maschinen::ContainerChildInfo*>::const_iterator maschinen::Children::beginContainer() const { return container_children.cbegin(); }
std::vector<maschinen::ContainerChildInfo*>::const_iterator maschinen::Children::endContainer() const { return container_children.cend(); }
size_t maschinen::Children::getContainerCount() const noexcept { return container_children.size(); }

void maschinen::Children::addStatusBar( maschinen::StatusBar* control, maschinen::ContainedController* parent ){
	StatusBarChildInfo* sbci = new StatusBarChildInfo();
	sbci->control = control;
	sbci->parent = parent;
	status_bar_children.push_back( sbci );
}
void maschinen::Children::removeStatusBar( maschinen::StatusBar* status_bar ) {
	StatusBarChildInfo* sbci = nullptr;
	for ( auto i = status_bar_children.cbegin(); i != status_bar_children.cend(); ++i ) {
		if ( status_bar->getId() == ( *i )->control->getId() ) { sbci = *i; break; }
	}
	if ( sbci ) {
		status_bar_children.erase( std::remove( status_bar_children.begin(), status_bar_children.end(), sbci ), status_bar_children.end() );
		delete sbci;
	}
}
std::vector<maschinen::StatusBarChildInfo*>::const_iterator maschinen::Children::beginStatusBar() const { return status_bar_children.cbegin(); }
std::vector<maschinen::StatusBarChildInfo*>::const_iterator maschinen::Children::endStatusBar() const { return status_bar_children.cend(); }
size_t maschinen::Children::getStatusBarCount() const noexcept { return status_bar_children.size(); }

maschinen::Children::~Children() {
	for ( auto i = container_children.cbegin(); i != container_children.cend(); ++i ) delete*i;
	container_children.clear();
	for ( auto i = button_children.cbegin(); i != button_children.cend(); ++i ) delete *i;
	button_children.clear();

	for ( auto i = edit_children.cbegin(); i != edit_children.cend(); ++i ) delete *i;
	edit_children.clear();

	for ( auto i = check_box_children.cbegin(); i != check_box_children.cend(); ++i ) delete *i;
	check_box_children.clear();

	for ( auto i = progress_bar_children.cbegin(); i != progress_bar_children.cend(); ++i ) delete *i;
	progress_bar_children.clear();

	for ( auto i = table_children.cbegin(); i != table_children.cend(); ++i ) delete *i;
	table_children.clear();

	for ( auto i = list_view_children.cbegin(); i != list_view_children.cend(); ++i ) delete *i;
	list_view_children.clear();

	for ( auto i = pop_up_menu_children.cbegin(); i != pop_up_menu_children.cend(); ++i ) delete *i;
	pop_up_menu_children.clear();
	
	for ( auto i = custom_control_children.cbegin(); i != custom_control_children.cend(); ++i ) delete *i;
	custom_control_children.clear();

	for ( auto i = status_bar_children.cbegin(); i != status_bar_children.cend(); ++i ) delete *i;
	status_bar_children.clear();

	window_children.erase( std::remove( window_children.begin(), window_children.end(), this ), window_children.end() );
}
