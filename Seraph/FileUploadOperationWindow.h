#pragma once
#include "CustomWindow.h"
#include "ListViewBasic.h"
#include "PopUpMenu.h"
#include "Table.h"
#include "resources.h"
#include "MetaDataUpload.h"
#include "FileUploadProcessWindow.h"
#include "Resource.h"
#include "MetaDataDoesNotMatchException.h"
#include "OnClickListener.h"
#include "OnListViewSelectListener.h"
class FileUploadProcessWindow;
class FileUploadOperationWindow : public CustomWindow, maschinen::event::OnClickListener, maschinen::event::listener::OnListViewSelectListener {
private:
	std::list<std::unique_ptr<FileUploadProcessWindow>> processors;
	std::unique_ptr<maschinen::list_view::ListViewBasic> lv;
	maschinen::menu::PopUpMenu menu;
	FileUploadProcessWindow* pop_up_selected = nullptr;
	FileUploadProcessWindow* getProcessWindowById( LPCTSTR id );
protected:
	void onCreate();
	void onShow();
	void onClose();
	void onDestroy();
public:
	FileUploadOperationWindow();
	void onClick( maschinen::event::OnClickEvent& e );
	void onListViewSelect( maschinen::event::OnListViewSelectEvent& e );
	void eraseProcessor(FileUploadProcessWindow* processor);
	void updateListView();
	std::unique_ptr<tstring> getPath();
	~FileUploadOperationWindow();
};

