#include "stdafx.h"
#include "FileProcessWindow.h"


FileProcessWindow::FileProcessWindow() {
}

void FileProcessWindow::onCreate() {
	lbl_status_var.setParentAndId( this, LABEL_FILE_PROCESS_STATUS_VAR );
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &lbl_status_var, 0, 0, 0, 3, 1, 0 );
	lbl_status_var.applyTabStop();
	lbl_status_var.create();
	lbl_status_var.setText( L"Status : " );

	lbl_status_val.setParentAndId( this, LABEL_FILE_PROCESS_STATUS_VAL );
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &lbl_status_val, 4, 0, 0, 10, 1, 0 );
	lbl_status_val.applyTabStop();
	lbl_status_val.create();
	lbl_status_val.setText( S_STATUS_DISCONNECT.c_str() );

	lbl_size_var.setParentAndId( this, LABEL_FILE_PROCESS_SIZE_VAR );
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &lbl_size_var, 0, 2, 0, 3, 1, 0 );
	lbl_size_var.applyTabStop();
	lbl_size_var.create();
	lbl_size_var.setText( L"Size : " );

	lbl_size_val.setParentAndId( this, LABEL_FILE_PROCESS_SIZE_VAL );
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &lbl_size_val, 4, 2, 0, 10, 1, 0 );
	lbl_size_val.applyTabStop();
	lbl_size_val.create();
	lbl_size_val.setText( fileSizeToStr(meta_data->getSize())->c_str() );

	lbl_downloaded_var.setParentAndId( this, LABEL_FILE_PROCESS_DOWNLOADED_VAR );
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &lbl_downloaded_var, 0, 4, 0, 3, 1, 0 );
	lbl_downloaded_var.applyTabStop();
	lbl_downloaded_var.create();
	lbl_downloaded_var.setText( L"Downloaded : " );

	lbl_downloaded_val.setParentAndId( this, LABEL_FILE_PROCESS_DOWNLOADED_VAL );
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &lbl_downloaded_val, 4, 4, 0, 10, 1, 0 );
	lbl_downloaded_val.applyTabStop();
	lbl_downloaded_val.create();
	lbl_downloaded_val.setText( fileSizeToStr( meta_data->getByteWritten() )->c_str() );

	lbl_speed_var.setParentAndId( this, LABEL_FILE_PROCESS_SPEED_VAR );
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &lbl_speed_var, 0, 6, 0, 3, 1, 0 );
	lbl_speed_var.applyTabStop();
	lbl_speed_var.create();
	lbl_speed_var.setText( L"Speed : " );

	lbl_speed_val.setParentAndId( this, LABEL_FILE_PROCESS_SPEED_VAL );
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &lbl_speed_val, 4, 6, 0, 10, 1, 0 );
	lbl_speed_val.applyTabStop();
	lbl_speed_val.create();
	lbl_speed_val.setText( L"0Bps" );

	lbl_remaining_time_var.setParentAndId( this, LABEL_FILE_PROCESS_REMAIN_TIME_VAR );
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &lbl_remaining_time_var, 0, 8, 0, 3, 1, 0 );
	lbl_remaining_time_var.applyTabStop();
	lbl_remaining_time_var.create();
	lbl_remaining_time_var.setText( L"Remaining time : " );

	lbl_remaining_time_val.setParentAndId( this, LABEL_FILE_PROCESS_REMAIN_TIME_VAL );
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &lbl_remaining_time_val, 4, 8, 0, 10, 1, 0 );
	lbl_remaining_time_val.applyTabStop();
	lbl_remaining_time_val.create();
	lbl_remaining_time_val.setText( L"infinite" );

	lbl_bufer_size_val.setParentAndId( this, LABEL_FILE_PROCESS_BUFFER_SIZE );
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &lbl_bufer_size_val, 0, 10, 0, 3, 1, 0 );
	lbl_bufer_size_val.applyTabStop();
	lbl_bufer_size_val.create();
	lbl_bufer_size_val.setText( L"Buffer size : " );

	edit_buffer_size.setParentAndId( this, EDIT_FILE_PROCESS_BUFFER_SIZE );
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &edit_buffer_size, 4, 10, 0, 10, 1, 0 );
	edit_buffer_size.create();
	edit_buffer_size.setText( to_tstring( 2 * MAX_LOADSTRING ) );

	btt_start_pause.setParentAndId( this, BUTTON_FILE_PROCESS_START_PAUSE );
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &btt_start_pause, 0, 13, 0, 10, 3, 0 );
	btt_start_pause.applyTabStop();
	btt_start_pause.create();

	if(meta_data->isFinished() ) btt_start_pause.setText( S_REDOWNLOAD.c_str() ); else btt_start_pause.setText( S_START.c_str() );

	btt_cancel.setParentAndId( this, BUTTON_FILE_PROCESS_CANCEL );
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &btt_cancel, 11, 13, 0, 10, 3, 0 );
	btt_cancel.applyTabStop();
	btt_cancel.create();
	btt_cancel.setText( L"cancel" );

	btt_start_pause.setOnClickListener( this );
	btt_cancel.setOnClickListener( this );
}

void FileProcessWindow::onClose() {
	hide();
}

void FileProcessWindow::onDestroy() {
	CustomWindow::onDestroy();
}

void FileProcessWindow::setMetaData( std::unique_ptr<MetaDataDownload> meta_data ) {
	this->meta_data = std::move( meta_data );
	this->meta_data->setUI( this );
}

MetaDataDownload* FileProcessWindow::getMetaData() {
	return meta_data.get();
}

void FileProcessWindow::setDataDownloaded( ULONGLONG data_downloaded ) {
	lbl_downloaded_val.setText( fileSizeToStr( data_downloaded )->c_str() );
}

void FileProcessWindow::setDownloadSpeed( ULONGLONG current_downloaded, time_t time ) {
	lbl_speed_val.setText( fileSizeToStr( current_downloaded / time )->c_str() );
}

void FileProcessWindow::setRemainingTime( ULONGLONG data_downloaded, ULONGLONG speed ) {
	if ( speed == 0 ) lbl_remaining_time_val.setText( L"infinite" );
	else lbl_remaining_time_val.setText( floorSeconds( ( meta_data->getSize() - data_downloaded ) / speed )->c_str() );
}

void FileProcessWindow::startMode() {
	edit_buffer_size.disable();
	btt_start_pause.setText( S_PAUSE.c_str() );
	lbl_status_val.setText( S_STATUS_CONNECT.c_str() );
}

void FileProcessWindow::pauseMode() {
	btt_start_pause.setText( S_START.c_str() );
	lbl_status_val.setText( S_STATUS_DISCONNECT.c_str() );
	edit_buffer_size.enable();
}

void FileProcessWindow::finishMode() {
	btt_start_pause.setText( S_REDOWNLOAD.c_str() );
	lbl_status_val.setText( S_STATUS_DISCONNECT.c_str() );
	edit_buffer_size.enable();
}

void FileProcessWindow::removeFile() {
	if ( meta_data.get() != nullptr ) {
		try {
			meta_data->remove();
		}
		catch ( SeraphException& e ) {
			MessageBox( getHandler(), e.getMessage()->c_str(), L"Error", MB_OK | MB_ICONERROR );
		}
	}
}

void FileProcessWindow::onClick( maschinen::event::OnClickEvent& e ) {
	if ( e.getSourceId() == BUTTON_FILE_PROCESS_START_PAUSE ) {
		if ( btt_start_pause.getText()->compare( S_START.c_str() ) == 0 ) {
			startMode();
			meta_data->startDownload( wcstoul( edit_buffer_size.getText()->c_str(), nullptr, 10 ) );
		}
		else if ( btt_start_pause.getText()->compare( S_REDOWNLOAD.c_str() ) == 0 ) {
			try {
				meta_data->deleteFiles();
			} catch ( SeraphException& e ) {
				MessageBox( getHandler(), e.getMessage()->c_str(), L"Error", MB_OK | MB_ICONERROR );
			} catch( maschinen::exception::Exception& e ) {
				MessageBox( getHandler(), e.getMessage()->c_str(), L"Error", MB_OK | MB_ICONERROR );
			} catch( std::exception& e ) {
				MessageBox( getHandler(), strToWstr( e.what() )->c_str(), L"Error", MB_OK | MB_ICONERROR );
			}
			startMode();
			meta_data->startDownload( wcstoul( edit_buffer_size.getText()->c_str(), nullptr, 10 ) );
		}
		else {
			meta_data->pause();
			pauseMode();
		}
	}
	else if ( e.getSourceId() == BUTTON_FILE_PROCESS_CANCEL ) {
		btt_start_pause.setText( S_START.c_str() );
		meta_data->cancel();
	}
}

void FileProcessWindow::onPaint( HDC hdc ) {
	HPEN drawer;
	HPEN hpen;
	COLORREF line_color;
	line_color = RGB( 255, 0, 0 );
	hpen = CreatePen( PS_SOLID, 1, line_color );
	drawer = ( HPEN ) SelectObject( hdc, hpen );
	for ( LONG i = 0; i < ( ( maschinen::layout::LayoutMatrix* )getLayout() )->getNumberOfCellPerHeight(); i++ ) {
		for ( LONG j = 0; j < ( ( maschinen::layout::LayoutMatrix* )getLayout() )->getNumberOfCellPerWidth(); j++ ) {
			std::unique_ptr<Position> pos = std::move( ( ( maschinen::layout::LayoutMatrix* )getLayout() )->getCoordinate( j, i, 0, 0, 0, 0 ) );
			MoveToEx( hdc, pos->x, pos->y, NULL );
			LineTo( hdc, pos->x + pos->width, pos->y );
			MoveToEx( hdc, pos->x + pos->width, pos->y, NULL );
			LineTo( hdc, pos->x + pos->width, pos->y + pos->height );
			MoveToEx( hdc, pos->x + pos->width, pos->y + pos->height, NULL );
			LineTo( hdc, pos->x, pos->y + pos->height );
			MoveToEx( hdc, pos->x, pos->y + pos->height, NULL );
			LineTo( hdc, pos->x, pos->y );
		}
	}


	SelectObject( hdc, drawer );
	DeleteObject( hpen );
}

void FileProcessWindow::setStatus( LPCTSTR status ) {
	lbl_status_val.setText( status );
}
void FileProcessWindow::setStatus( tstring& status ) {
	lbl_status_val.setText( status );
}
void FileProcessWindow::setStatus( tstring* status ) {
	lbl_status_val.setText( status->c_str() );
}

FileProcessWindow::~FileProcessWindow() {
}
