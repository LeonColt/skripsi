#include "stdafx.h"
#include "FileUploadOperationWindow.h"


FileUploadOperationWindow::FileUploadOperationWindow() {
}

void FileUploadOperationWindow::onClick( maschinen::event::OnClickEvent& e ) {
	if ( pop_up_selected != nullptr ) {
		switch ( e.getSourceId() ) {
			case MENU_FILE_UPLOAD_OPERATION_WINDOW_START_UPLOAD : {
				pop_up_selected->show();
			} break;

			case MENU_FILE_UPLOAD_OPERATION_WINDOW_OPEN_FILE_LOCATION: {
			} break;

			case MENU_FILE_UPLOAD_OPERATION_WINDOW_DELETE_FILE: {
			} break;

			case MENU_FILE_DOWNLOAD_WINDOW_DELETE: {
				pop_up_selected->getMetaData()->deleteMetaDataFile();
				pop_up_selected->destroy();
				processors.erase(std::remove_if(processors.begin(), processors.end(), [&](std::unique_ptr<FileUploadProcessWindow> const& p) {
					return p.get() == pop_up_selected;
				}
				), processors.end() );
				updateListView();
			} break;
		}
	}
}

void FileUploadOperationWindow::onListViewSelect( maschinen::event::OnListViewSelectEvent& e ) {
	switch ( e.getMethod() ) {
		case NM_RCLICK:
		{
			if ( lv->getSelectedCount() == 0 ) return;
			pop_up_selected = getProcessWindowById( ( ( maschinen::list_view::Table* )lv.get() )->getText( lv->getSelectedItemIndex()->front(), 0 )->c_str() );
			if ( pop_up_selected != nullptr ) menu.show();
		} break;
	}
}

FileUploadProcessWindow* FileUploadOperationWindow::getProcessWindowById( LPCTSTR id ) {
	for ( auto i = processors.begin(); i != processors.end(); ++i ) {
		if ( ( *i )->getMetaData()->getId()->compare( id ) == 0 ) return ( *i ).get();
	}
	return nullptr;
}

void FileUploadOperationWindow::onCreate() {
	lv = std::make_unique< maschinen::list_view::Table>();
	lv->setParentAndId(this, LISTVIEW_FILE_UPLOAD_OPERATION_WINDOW );
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( lv.get(), 0, 0, FILL_PARENT, FILL_PARENT, FILL_PARENT, FILL_PARENT );

	if ( maschinen::list_view::Table* table = dynamic_cast< maschinen::list_view::Table* > ( lv.get() ) ) {
		table->applyFullRowSelect();
		table->applyHeaderDragDrop();
		table->applyShowGridLines();
		table->autoWidth();
		table->create();

		table->setOnListViewSelectListener( this );

		table->addColumn( TEXT( "ID" ) );
		table->addColumn( TEXT( "File Id" ) );
		table->addColumn( TEXT( "Name" ) );
		table->addColumn( TEXT( "Size" ) );
	}

	menu.create();
	menu.flagPopUpMenuLeftAlign();
	menu.flagPopUpMenuBottomAlign();
	menu.positionbyCursor();
	menu.setParent( this );
	menu.append( MENU_FILE_UPLOAD_OPERATION_WINDOW_START_UPLOAD, TEXT( "Begin Upload" ) );
	menu.append( MENU_FILE_UPLOAD_OPERATION_WINDOW_OPEN_FILE_LOCATION, TEXT( "Open File Location" ) );
	menu.append( MENU_FILE_UPLOAD_OPERATION_WINDOW_DELETE_FILE, TEXT( "Delete Files" ) );
	menu.append( MENU_FILE_UPLOAD_OPERATION_WINDOW_DELETE, TEXT( "Delete" ) );
	menu.setOnClickListener( this );
}

void FileUploadOperationWindow::onShow() {
	CustomWindow::onShow();
	updateListView();
}

void FileUploadOperationWindow::onClose(){
	hide();
}

void FileUploadOperationWindow::onDestroy() {
	for ( auto i = processors.begin(); i != processors.end(); ++i )
		( *i )->destroy();
	CustomWindow::onDestroy();
}

void FileUploadOperationWindow::eraseProcessor( FileUploadProcessWindow* processor ) {
	processors.erase( std::remove_if( processors.begin(), processors.end(), [ & ]( std::unique_ptr<FileUploadProcessWindow> const& p ) {
		return p.get() == processor;
	}
	), processors.end() );
	processor->destroy();
}

void FileUploadOperationWindow::updateListView() {
	OutputDebugStringW( TEXT( "Update Upload List View\n" ) );
	lv->deleteAllItems();
	try {
		TCHAR path[ MAX_PATH ] = TEXT( "" );
		wcscat_s( path, getPath()->c_str() );
		wcscat_s( path, L"\\*" );
		maschinen::file::DirectoryHandler dh( path );
		bool already_exists;
		for ( auto i = dh.begin(); i != dh.end(); i++ ) {
			if ( _tcscmp( i.file_name.c_str(), TEXT( "." ) ) == 0 || _tcscmp( i.file_name.c_str(), TEXT( ".." ) ) == 0 ) continue;
			already_exists = false;
			std::unique_ptr<MetaDataUpload> meta_data = std::make_unique<MetaDataUpload>();
			try { meta_data->read( i.file_name.c_str() ); } catch ( MetaDataDoesNotMatchException& ) { continue; }
			meta_data->setAkey( akey );
			meta_data->setSetting( setting );
			meta_data->setUserCredential( user_credential );
			for ( auto j = processors.cbegin(); j != processors.cend(); ++j ) {
				if ( *( ( *j )->getMetaData() ) == *meta_data ) already_exists = true;
			}
			if ( !already_exists ) {
				std::unique_ptr<FileUploadProcessWindow> process = std::make_unique<FileUploadProcessWindow>( this );
				maschinen::layout::LayoutMatrix* layout = new maschinen::layout::LayoutMatrix();
				layout->setNumberOfCell( 50, 50 );
				meta_data->setUI( process.get() );
				process->setMetaData( std::move( meta_data ) );
				process->setStyle( CS_HREDRAW | CS_VREDRAW );
				process->setIcon( LoadIcon( getInstance(), MAKEINTRESOURCE( IDI_SERAPH ) ) );
				process->setCursor( LoadCursor( nullptr, IDC_ARROW ) );
				process->setBackground( ( HBRUSH ) ( COLOR_WINDOW + 1 ) );
				process->setSmallIcon( LoadIcon( getInstance(), MAKEINTRESOURCE( IDI_SMALL ) ) );
				process->setLayout( layout );
				process->setAkey( akey );
				process->setSetting( setting );
				process->setUserCredential( user_credential );
				process->create();
				processors.push_back( std::move( process ) );
			}
		}
	} 
	catch ( SeraphException& e ) {
		if ( e.getErrorCode() != dword::error_code::meta_data::META_DATA_DOES_NOT_MATCH ) {
			TCHAR buffer[ MAX_LOADSTRING ];
			e.getMessage( buffer, MAX_LOADSTRING );
			MessageBox( NULL, buffer, L"Error", MB_OK | MB_ICONERROR );
		}
	} catch ( maschinen::exception::Exception& e ) {
		if ( e.getErrorCode() != dword::error_code::meta_data::META_DATA_DOES_NOT_MATCH ) {
			TCHAR buffer[ MAX_LOADSTRING ];
			e.getMessage( buffer, MAX_LOADSTRING );
			MessageBox( NULL, buffer, L"Error", MB_OK | MB_ICONERROR );
			SeraphException se;
			se.write( string::file::name::ERROR_LOG_NAME.c_str(), e );
		}
	} catch ( std::exception& e ) {
		TCHAR buffer[ MAX_LOADSTRING ];
		size_t size = strlen( e.what() );
		mbstowcs_s( &size, buffer, e.what(), MAX_LOADSTRING );
		MessageBox( NULL, buffer, L"Error", MB_OK | MB_ICONERROR );
		SeraphException se;
		se.write( string::file::name::ERROR_LOG_NAME.c_str(), e );
	}
	if ( maschinen::list_view::Table* table = dynamic_cast< maschinen::list_view::Table* > ( lv.get() ) ) {
		int counter = 0;
		for ( auto i = processors.cbegin(); i != processors.cend(); ++i ) {
			for ( auto j = ( *i )->getMetaData()->begin(); j != ( *i )->getMetaData()->end(); ++j ) {
				table->addRow();
				table->setText( ( *i )->getMetaData()->getId()->c_str(), counter, 0 );
				table->setText( to_tstring( j->getId() ), counter, 1 );
				table->setText( j->getName()->c_str(), counter, 2 );
				table->setText( to_tstring( j->getSize() ), counter, 3 );
				counter++;
			}
		}
	}
}

std::unique_ptr<tstring> FileUploadOperationWindow::getPath() {
	std::unique_ptr<tstring> path = std::make_unique<tstring>();
	path->append( getDefaultAppDataPath( true )->c_str() );
	path->append( string::directory::name::FILE_META_DATA.c_str() );
	DWORD attributes = GetFileAttributes( path->c_str() );
	if ( attributes == INVALID_FILE_ATTRIBUTES ) {
		if ( CreateDirectoryW( path->c_str(), NULL ) == 0 ) {
			DWORD error_code;
			TCHAR error[ MAX_LOADSTRING ];
			if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) )
				throw SeraphException( string::file::name::ERROR_LOG_NAME.c_str(), error, error_code );
			else throw SeraphException( string::file::name::ERROR_LOG_NAME.c_str() );
		}
	}
	return path;
}

FileUploadOperationWindow::~FileUploadOperationWindow() {
}
