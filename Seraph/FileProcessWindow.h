#pragma once
#include "CustomWindow.h"
#include "MetaDataDownload.h"
#include "resources.h"
#include "Label.h"
#include "Button.h"
#include "Edit.h"
#include "UniversalMethode.h"
#include "Downloader.h"
#include "ComboBox.h"
#include "OnClickListener.h"
class MetaDataDownload;
class FileProcessWindow : public CustomWindow, maschinen::event::OnClickListener {
private:
	const tstring S_START = TEXT( "Start" ), S_PAUSE = TEXT( "Pause" ), S_REDOWNLOAD = TEXT( "Redownload" );
	const tstring S_STATUS_CONNECT = TEXT( "Connect" ), S_STATUS_DISCONNECT = TEXT( "Disconnect" );
	maschinen::Label lbl_status_var, lbl_status_val, lbl_size_var, lbl_size_val, 
		lbl_downloaded_var, lbl_downloaded_val, lbl_speed_var, lbl_speed_val, 
		lbl_remaining_time_var, lbl_remaining_time_val, lbl_bufer_size_val;
	maschinen::Edit edit_buffer_size;
	maschinen::Button btt_start_pause, btt_cancel;
	std::unique_ptr<MetaDataDownload> meta_data;
protected:
	void onCreate();
	void onClose();
	void onDestroy();
	void onPaint( HDC hdc );
public:
	FileProcessWindow();
	void setMetaData( std::unique_ptr<MetaDataDownload> meta_data);
	MetaDataDownload* getMetaData();
	void setDataDownloaded( ULONGLONG data_downloaded );
	void setDownloadSpeed( ULONGLONG current_downloaded, time_t time );
	void setRemainingTime( ULONGLONG data_uploaded, ULONGLONG speed );
	void setStatus( LPCTSTR status );
	void setStatus( tstring& status );
	void setStatus( tstring* status );
	void startMode();
	void pauseMode();
	void finishMode();
	void removeFile();
	void onClick( maschinen::event::OnClickEvent& e );
	~FileProcessWindow();
};

