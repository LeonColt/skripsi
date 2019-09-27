#pragma once
#define POCO_NO_UNWINDOWS
#include "CustomAsyncTask.h"
#include "FileControl.h"
#include "DriveFixed.h"
#include "DriveRemovable.h"
class FileControl;
class LocalChangeDetector : public CustomAsyncTask {
private:
	bool flag_stop, flag_restart;
	HANDLE watcher[ 2 ];
	tstring search_file_name;
protected:
	void onPreExecute();
	LPVOID doInBackground( LPVOID input );
	void onPostExecute( LPVOID result );
public:
	LocalChangeDetector();
	void stop();
	bool isStopped() const noexcept;
	void restart();
	void search( LPCTSTR name, std::list<Local*>& buffer, Local* start_position );
	void setSearchName( LPCTSTR search_file_name );
	void setSearchName( tstring& search_file_name );
	void setSearchName( tstring* search_file_name );
	~LocalChangeDetector();
};

