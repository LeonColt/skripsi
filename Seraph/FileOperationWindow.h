#pragma once
#include <memory>
#include <shellapi.h>
#include "CustomWindow.h"
#include "ProgressBar.h"
#include "Table.h"
#include "ListViewColumn.h"
#include "ListViewGroup.h"
#include "resources.h"
#include "FileProcessWindow.h"
#include "PopUpMenu.h"
#include "Resource.h"
#include "MetaDataDownload.h"
#include "DirectoryHandler.h"
#include "ComponentObjectModel.h"
#include "FileOperation.h"
#include "OnClickListener.h"
#include "OnListViewSelectListener.h"

class FileOperationWindow : public CustomWindow, maschinen::event::OnClickListener, maschinen::event::listener::OnListViewSelectListener
{
private:
	std::vector<std::unique_ptr<FileProcessWindow>> processors;
	maschinen::menu::PopUpMenu menu;
	std::unique_ptr<maschinen::list_view::ListViewBasic> lv;
	FileProcessWindow* pop_up_selected = nullptr;
	FileProcessWindow* getFileProcessWindowByDownloadId(LPCTSTR id_download);
protected:
	void onCreate();
	void onShow();
	void onClose();
	void onDestroy();
public:
	FileOperationWindow();
	void updateListView();
	void onClick( maschinen::event::OnClickEvent& e );
	void onListViewSelect( maschinen::event::OnListViewSelectEvent& e );
	std::unique_ptr<tstring> getPath();
	~FileOperationWindow();
};