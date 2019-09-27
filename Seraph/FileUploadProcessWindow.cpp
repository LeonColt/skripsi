#include "stdafx.h"
#include "FileUploadProcessWindow.h"


FileUploadProcessWindow::FileUploadProcessWindow( FileUploadOperationWindow* const listering ) : LISTERING( listering ) {
}

void FileUploadProcessWindow::onCreate() {
	lbl_status_var.setParentAndId( this, LABEL_FILE_UPLOAD_PROCESS_STATUS_VAR );
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &lbl_status_var, 0, 0, 0, 3, 1, 0 );
	lbl_status_var.applyTabStop();
	lbl_status_var.create();
	lbl_status_var.setText( L"Status : " );

	lbl_status_val.setParentAndId( this, LABEL_FILE_UPLOAD_PROCESS_STATUS_VAL );
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &lbl_status_val, 4, 0, 0, 10, 1, 0 );
	lbl_status_val.applyTabStop();
	lbl_status_val.create();
	lbl_status_val.setText( S_STATUS_DISCONNECT.c_str() );

	lbl_size_var.setParentAndId( this, LABEL_FILE_UPLOAD_PROCESS_SIZE_VAR );
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &lbl_size_var, 0, 2, 0, 3, 1, 0 );
	lbl_size_var.applyTabStop();
	lbl_size_var.create();
	lbl_size_var.setText( L"Size : " );

	lbl_size_val.setParentAndId( this, LABEL_FILE_UPLOAD_PROCESS_SIZE_VAL );
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &lbl_size_val, 4, 2, 0, 10, 1, 0 );
	lbl_size_val.applyTabStop();
	lbl_size_val.create();
	lbl_size_val.setText( fileSizeToStr((ULONGLONG) meta_data->getSize() )->c_str() );

	lbl_uploaded_var.setParentAndId( this, LABEL_FILE_UPLOAD_PROCESS_DOWNLOADED_VAR );
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &lbl_uploaded_var, 0, 4, 0, 3, 1, 0 );
	lbl_uploaded_var.applyTabStop();
	lbl_uploaded_var.create();
	lbl_uploaded_var.setText( L"Uploaded : " );

	lbl_uploaded_val.setParentAndId( this, LABEL_FILE_UPLOAD_PROCESS_DOWNLOADED_VAL );
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &lbl_uploaded_val, 4, 4, 0, 10, 1, 0 );
	lbl_uploaded_val.applyTabStop();
	lbl_uploaded_val.create();

	lbl_speed_var.setParentAndId( this, LABEL_FILE_UPLOAD_PROCESS_SPEED_VAR );
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &lbl_speed_var, 0, 6, 0, 3, 1, 0 );
	lbl_speed_var.applyTabStop();
	lbl_speed_var.create();
	lbl_speed_var.setText( L"Speed : " );

	lbl_speed_val.setParentAndId( this, LABEL_FILE_UPLOAD_PROCESS_SPEED_VAL );
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &lbl_speed_val, 4, 6, 0, 10, 1, 0 );
	lbl_speed_val.applyTabStop();
	lbl_speed_val.create();
	lbl_speed_val.setText( L"0Bps" );

	lbl_remaining_time_var.setParentAndId( this, LABEL_FILE_UPLOAD_PROCESS_REMAIN_TIME_VAR );
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &lbl_remaining_time_var, 0, 8, 0, 3, 1, 0 );
	lbl_remaining_time_var.applyTabStop();
	lbl_remaining_time_var.create();
	lbl_remaining_time_var.setText( L"Remaining time : " );

	lbl_remaining_time_val.setParentAndId( this, LABEL_FILE_UPLOAD_PROCESS_REMAIN_TIME_VAL );
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &lbl_remaining_time_val, 4, 8, 0, 10, 1, 0 );
	lbl_remaining_time_val.applyTabStop();
	lbl_remaining_time_val.create();
	lbl_remaining_time_val.setText( L"infinite" );

	lbl_bufer_size_val.setParentAndId( this, LABEL_FILE_UPLOAD_PROCESS_BUFFER_SIZE );
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &lbl_bufer_size_val, 0, 10, 0, 3, 1, 0 );
	lbl_bufer_size_val.applyTabStop();
	lbl_bufer_size_val.create();
	lbl_bufer_size_val.setText( L"Buffer size : " );

	edit_buffer_size.setParentAndId( this, EDIT_FILE_UPLOAD_PROCESS_BUFFER_SIZE );
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &edit_buffer_size, 4, 10, 0, 10, 1, 0 );
	edit_buffer_size.create();
	edit_buffer_size.setText( to_tstring( 2 * MAX_LOADSTRING ) );


	btt_start_pause.setParentAndId( this, BUTTON_FILE_UPLOAD_PROCESS_START_PAUSE );
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &btt_start_pause, 0, 13, 0, 10, 3, 0 );
	btt_start_pause.applyTabStop();
	btt_start_pause.create();
	btt_start_pause.setOnClickListener( this );

	btt_cancel.setParentAndId( this, BUTTON_FILE_UPLOAD_PROCESS_CANCEL );
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &btt_cancel, 11, 13, 0, 10, 3, 0 );
	btt_cancel.applyTabStop();
	btt_cancel.create();
	btt_cancel.setText( L"cancel" );
	btt_start_pause.setOnClickListener( this );
}

void FileUploadProcessWindow::onShow() {
	CustomWindow::onShow();
	UploadedFileTotalSizeAT* total_size_getter = new UploadedFileTotalSizeAT();
	total_size_getter->setAutoDelete();
	total_size_getter->setAkey( akey );
	total_size_getter->setErrorMarker( TEXT( "Upload File Error" ) );
	total_size_getter->setParent( this );
	total_size_getter->setSetting( setting );
	total_size_getter->setUserCredential( user_credential );
	total_size_getter->setUploadId( meta_data->getId()->c_str() );
	total_size_getter->execute();
	btt_start_pause.setText( S_START );
}

void FileUploadProcessWindow::onHide() {
	CustomWindow::onHide();
}

void FileUploadProcessWindow::onClose() {
	hide();
}

FileUploadProcessWindow::~FileUploadProcessWindow() {}

void FileUploadProcessWindow::setMetaData( std::unique_ptr<MetaDataUpload> meta_data ) {
	this->meta_data = std::move( meta_data );
}

MetaDataUpload* FileUploadProcessWindow::getMetaData() {return meta_data.get();}

void FileUploadProcessWindow::startMode() {
	edit_buffer_size.disable();
	btt_start_pause.setText( S_PAUSE.c_str() );
	lbl_status_val.setText( S_STATUS_CONNECT.c_str() );
}

void FileUploadProcessWindow::pauseMode() {
	btt_start_pause.setText( S_START.c_str() );
	lbl_status_val.setText( S_STATUS_DISCONNECT.c_str() );
	edit_buffer_size.enable();
}

void FileUploadProcessWindow::finishMode() {
	lbl_status_val.setText( S_STATUS_DISCONNECT.c_str() );
	edit_buffer_size.enable();
	LISTERING->updateListView();
	destroy();
}

void FileUploadProcessWindow::onClick( maschinen::event::OnClickEvent& e ) {
	if ( e.getSourceId() == BUTTON_FILE_UPLOAD_PROCESS_START_PAUSE ) {
		if ( btt_start_pause.getText()->compare( S_START.c_str() ) == 0 ) {
			startMode();
			meta_data->startUpload( wcstoul( edit_buffer_size.getText()->c_str(), nullptr, 10 ) );
		}
		else {
			disableButtonStartPause();
			meta_data->pause();
		}
	}
}

void FileUploadProcessWindow::setTotalUploaded( QWORD total_uploaded ) {
	this->total_uploaded = total_uploaded;
	lbl_uploaded_val.setText( fileSizeToStr( total_uploaded )->c_str() );
}

QWORD FileUploadProcessWindow::getTotalUploaded() const noexcept {
	return total_uploaded;
}

void FileUploadProcessWindow::setStatus( LPCTSTR status ) {
	lbl_status_val.setText( status );
}

void FileUploadProcessWindow::setStatus( tstring& status ) {
	lbl_status_val.setText( status.c_str() );
}

void FileUploadProcessWindow::setStatusDefault() {
	if ( btt_start_pause.getText()->compare( S_START.c_str() ) == 0 ) 
		lbl_status_val.setText( S_STATUS_DISCONNECT.c_str() );
	else lbl_status_val.setText( S_STATUS_CONNECT.c_str() );
}

void FileUploadProcessWindow::setSpeed( ULONGLONG speed, time_t interval ) {
	lbl_speed_val.setText( fileSizeToStr( speed / interval )->c_str() );
}

void FileUploadProcessWindow::setRemainingTime( ULONGLONG data_uploaded, ULONGLONG speed ) {
	if ( speed == 0 ) lbl_remaining_time_val.setText( L"infinite" );
	else lbl_remaining_time_val.setText( floorSeconds( ( meta_data->getSize() - data_uploaded ) / speed )->c_str() );
}

void FileUploadProcessWindow::disableButtonStartPause() {
	btt_start_pause.disable();
}
void FileUploadProcessWindow::enableButtonStartPause() {
	btt_start_pause.enable();
}

FileUploadOperationWindow* FileUploadProcessWindow::getListering(){
	return LISTERING;
}