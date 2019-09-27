#pragma once
#include "CustomWindow.h"
#include "MetaDataUpload.h"
#include "Label.h"
#include "Edit.h"
#include "Button.h"
#include "resources.h"
#include "UploadedFileTotalSizeAT.h"
#include "OnClickListener.h"
#include "FileUploadOperationWindow.h"
class FileUploadOperationWindow;
class MetaDataUpload;
class UploadedFileTotalSizeAT;
class FileUploadProcessWindow : public CustomWindow, public maschinen::event::OnClickListener {
	const tstring S_START = TEXT( "Start" ), S_PAUSE = TEXT( "Pause" );
	const tstring S_STATUS_CONNECT = TEXT( "Connect" ), S_STATUS_DISCONNECT = TEXT( "Disconnect" );
	maschinen::Label lbl_status_var, lbl_status_val, lbl_size_var, lbl_size_val,
		lbl_uploaded_var, lbl_uploaded_val , lbl_speed_var, lbl_speed_val,
		lbl_remaining_time_var, lbl_remaining_time_val, lbl_bufer_size_val;
	maschinen::Edit edit_buffer_size;
	maschinen::Button btt_start_pause, btt_cancel;
	std::unique_ptr<MetaDataUpload> meta_data;
	QWORD total_uploaded;
	FileUploadOperationWindow* const LISTERING;
protected:
	void onCreate();
	void onShow();
	void onHide();
	void onClose();
public:
	FileUploadProcessWindow( FileUploadOperationWindow* const listering );
	void setMetaData( std::unique_ptr<MetaDataUpload> meta_data );
	MetaDataUpload* getMetaData();
	void startMode();
	void pauseMode();
	void finishMode();
	void onClick( maschinen::event::OnClickEvent& e );
	void setTotalUploaded( QWORD total_uploaded );
	QWORD getTotalUploaded() const noexcept;
	void setStatus( LPCTSTR status );
	void setStatus( tstring& status );
	void setStatusDefault();
	void setSpeed(ULONGLONG speed, time_t interval);
	void setRemainingTime( ULONGLONG data_downloaded, ULONGLONG speed );
	void disableButtonStartPause();
	void enableButtonStartPause();
	FileUploadOperationWindow* getListering();
	~FileUploadProcessWindow();
};

