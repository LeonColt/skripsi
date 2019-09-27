#pragma once
#include <list>
#include "BaseFile.h"
#include "CloudFileOperator.h"
#include "CloudRecycleBin.h"
#include "CloudRecycleBinObject.h"
class LocalFileOperator;
class CloudRecycleBin;
class FileControl;
class FileControlSelectSwitcher {
private:
	FileControl* fc;
	std::list<BaseFile*> selected;
public:
	FileControlSelectSwitcher( FileControl* fc );
	void retriveSelectedFile( maschinen::list_view::ListViewBasic* lv, maschinen::event::OnListViewSelectEvent& e );
	void addSelected( BaseFile* file );
	bool isSingleSelection() const noexcept;
	bool isMultipleSelection() const noexcept;
	bool isNoSelection() const noexcept;
	BaseFile* get() const noexcept;
	std::list<BaseFile*>::const_iterator begin() const noexcept;
	std::list<BaseFile*>::const_iterator end() const noexcept;
	~FileControlSelectSwitcher();
};

