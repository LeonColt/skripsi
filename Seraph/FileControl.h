#pragma once
#include <memory>
#include <shellapi.h>
#include "CustomContainer.h"
#include "Button.h"
#include "Edit.h"
#include "Table.h"
#include "ProgressBar.h"
#include "resources.h"
#include "SeraphCommon.h"
#include "PopUpMenu.h"
#include "LocalChangeDetector.h"
#include "CloudFileOperator.h"
#include "CloudRecycleBin.h"
#include "FileExecutor.h"
#include "FileControlSelectSwitcher.h"

#include "WininetConnect.h"
#include "Exception.h"
#include "ClipBoard.h"
#include "OnClickListener.h"
#include "OnListViewSelectListener.h"
#include "OnKeyPressedListener.h"
#include "OnFocusListener.h"
#include "CloudProperty.h"
#include "LocalProperty.h"
#include "StatusBar.h"
#include "URLParser.h"
#include "DriveLoader.h"
#include "FileTypeMapping.h"

const std::wstring var_get_file_get_children_by_parent_id = L"cf-gf-23433434";
class FileExecutor;
class LocalChangeDetector;
class CloudRecycleBin;
class FileControlSelectSwitcher;
class CloudFileOperator;
struct CutCopyPasteFileOperation {
	const INT COPY = 1;
	const INT CUT = 2;
	const INT NO_OPERATION = 0;
	INT operation = NO_OPERATION;
	std::list<tstring> files;
};
class FileControl : public CustomContainer, 
	maschinen::event::OnClickListener,
	maschinen::event::listener::OnListViewSelectListener,
	maschinen::event::listener::OnKeyPressedListener,
	maschinen::event::listener::OnFocusListener
{
private:
	maschinen::StatusBar* status;

	std::unique_ptr<FileExecutor> fe;
	maschinen::Button btt_back;
	maschinen::Edit url, search, renamer;
	maschinen::ProgressBar pb;
	std::unique_ptr<maschinen::list_view::ListViewBasic> lv;
	maschinen::menu::PopUpMenu pop_up_menu;

	std::unique_ptr<LocalChangeDetector> local;
	std::unique_ptr<CloudFileOperator> cloud;
	std::unique_ptr<FileControlSelectSwitcher> switcher;

	std::unique_ptr<BaseFile> pointer = nullptr, previous_ptr;
	CutCopyPasteFileOperation file_operation;

	std::list<std::unique_ptr<BaseFile>> file_buffer;
	std::deque<std::unique_ptr<BaseFile>> file_history;

	std::list<std::unique_ptr<Property>> properties;

	bool flag_updating_list_view;

	void opener();

	bool flag_search_cloud_finish, flag_search_local_finish;

	FileTypeMapping mapper;
protected:
	void onCreate();
	void onShow();
	void onHide();
	void onDestroy();
public:
	FileControl();
	void onClick( maschinen::event::OnClickEvent& e );
	void onListViewSelect( maschinen::event::OnListViewSelectEvent& e );
	void onKeyUp( maschinen::Controller* control, WPARAM key );
	void onKeyDown( maschinen::Controller* control, WPARAM key );
	void onFocus( maschinen::Controller* control, bool focus );
	void refreshListView();
	void updateListView();
	void prosesListView();
	void setFileControLoadDisable();
	void setFileControlLoadEnable();
	void restartRequest();
	void stopRefresher();
	CloudFileOperator* getCloud() const noexcept;
	void resetBuffer( std::list<BaseFile*> buffer );
	void addFileToBuffer( BaseFile* file );
	void cleanBuffer();
	void deleteProperty(Property* window_property);
	void setStatusBar(maschinen::StatusBar* status);
	std::list<std::unique_ptr<BaseFile>>::const_iterator begin();
	std::list<std::unique_ptr<BaseFile>>::const_iterator end();
	BaseFile* getPointer();
	void setPointer( BaseFile* pointer );
	bool isListViewUpdating() const noexcept;
	void setFlagSearchCloudFinish(bool flag_search_cloud_finish );
	void setFlagSearchLocalFinish(bool flag_search_local_finish );
	void endSearch();
	~FileControl();
};

