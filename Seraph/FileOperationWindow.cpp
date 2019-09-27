#include "stdafx.h"
#include "FileOperationWindow.h"


FileOperationWindow::FileOperationWindow(){}

void FileOperationWindow::onCreate()
{
	lv = std::make_unique<maschinen::list_view::Table>();
	lv->setParentAndId(this, LIST_VIEW_FILE_DOWNLOAD_WINDOW);
	lv->setOnListViewSelectListener( this );
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( lv.get(), 0, 0, FILL_PARENT, FILL_PARENT, FILL_PARENT, FILL_PARENT );
	lv->setStyleAlignTop();
	((maschinen::list_view::Table*)lv.get())->applyFullRowSelect();
	((maschinen::list_view::Table*)lv.get())->applyHeaderDragDrop();
	((maschinen::list_view::Table*)lv.get())->applyShowGridLines();
	((maschinen::list_view::Table*)lv.get())->autoWidth();
	((maschinen::list_view::Table*)lv.get())->create();

	( ( maschinen::list_view::Table* )lv.get() )->addColumn( TEXT( "ID" ) );
	( ( maschinen::list_view::Table* )lv.get() )->addColumn( TEXT( "File ID" ) );
	( ( maschinen::list_view::Table* )lv.get() )->addColumn( TEXT( "Name" ) );
	( ( maschinen::list_view::Table* )lv.get() )->addColumn( TEXT( "Size" ) );
	( ( maschinen::list_view::Table* )lv.get() )->addColumn( TEXT( "Progress" ) );
	( ( maschinen::list_view::Table* )lv.get() )->addColumn( TEXT( "Speed" ) );
	( ( maschinen::list_view::Table* )lv.get() )->addColumn( TEXT( "Estimate" ) );
	( ( maschinen::list_view::Table* )lv.get() )->addColumn( TEXT( "State" ) );

	menu.create();
	menu.flagPopUpMenuLeftAlign();
	menu.flagPopUpMenuBottomAlign();
	menu.setParent( this );
	menu.positionbyCursor();
	menu.append( MENU_FILE_DOWNLOAD_WINDOW_START_DOWNLOAD, L"Begin Download" );
	menu.append( MENU_FILE_DOWNLOAD_WINDOW_OPEN_FILE_LOCATION, L"Open File Location" );
	menu.append( MENU_FILE_DOWNLOAD_WINDOW_OPEN_FILE_LOCATION_IN_EXPLORER, L"Open File Location In Explorer" );
	menu.append( MENU_FILE_DOWNLOAD_WINDOW_DELETE_FILE, L"Delete File" );
	menu.append( MENU_FILE_DOWNLOAD_WINDOW_DELETE, L"Delete" );
	menu.setOnClickListener( this );
}

void FileOperationWindow::onShow()
{
	//maschinen::Window::onShow();
	updateListView();
	lv->show();
}

void FileOperationWindow::onClose() {
	hide();
}

void FileOperationWindow::onDestroy() {
	for ( auto i = processors.begin(); i != processors.end(); ++i )
		( *i )->destroy();
	CustomWindow::onDestroy();
}

FileProcessWindow* FileOperationWindow::getFileProcessWindowByDownloadId( LPCTSTR id_download ) {
	for ( auto i = processors.begin(); i != processors.end(); ++i ) {
		if ( ( *i )->getMetaData()->getId()->compare( id_download ) == 0 ) return ( *i ).get();
	}
	return nullptr;
}

void FileOperationWindow::updateListView()
{
	lv->deleteAllItems();
	try {
		tstring path = getPath()->c_str(); path.append( TEXT( "\\*" ) );
		maschinen::file::DirectoryHandler dh( path );
		bool already_exists;
		for ( auto i = dh.begin(); i != dh.end(); i++ ) {
			if ( _tcscmp( i.file_name.c_str(), TEXT( "." ) ) == 0 || _tcscmp( i.file_name.c_str(), TEXT( ".." ) ) == 0 ) continue;
			already_exists = false;
			std::unique_ptr<MetaDataDownload> meta_data = std::make_unique<MetaDataDownload>();
			meta_data->setAkey( akey );
			meta_data->setSetting( setting );
			meta_data->setUserCredential( user_credential );
			OutputDebugStringW( i.file_name.c_str() );
			try {meta_data->read( i.file_name.c_str() );} catch ( MetaDataDoesNotMatchException& ) {continue;}
			for ( auto j = processors.begin(); j != processors.end(); ++j ) {
				if ( *( ( *j )->getMetaData() ) == *meta_data ) { already_exists = true; break; }
			}

			if ( !already_exists ) {
				std::unique_ptr<FileProcessWindow> process = std::make_unique<FileProcessWindow>();
				maschinen::layout::LayoutMatrix* layout = new maschinen::layout::LayoutMatrix();
				layout->setNumberOfCell( 50, 50 );
				process->setLayout( layout );
				process->setMetaData( std::move( meta_data ) );
				process->setStyle( CS_HREDRAW | CS_VREDRAW );
				process->setIcon( LoadIcon( getInstance(), MAKEINTRESOURCE( IDI_SERAPH ) ) );
				process->setCursor( LoadCursor( nullptr, IDC_ARROW ) );
				process->setBackground( ( HBRUSH ) (COLOR_WINDOW + 1) );
				process->setSmallIcon( LoadIcon( getInstance(), MAKEINTRESOURCE( IDI_SMALL ) ) );

				process->setAkey( akey );
				process->setSetting( setting );
				process->setUserCredential( user_credential );

				process->create();

				processors.push_back( std::move(process) );
			}
		}
	} catch ( SeraphException& e ) {
		MessageBox( NULL, e.getMessage()->c_str(), L"Error", MB_OK | MB_ICONERROR );
	} catch ( maschinen::exception::Exception& e ) {
		MessageBox( NULL, e.getMessage()->c_str(), L"Error", MB_OK | MB_ICONERROR );
		SeraphException se;
		se.write( string::file::name::ERROR_LOG_NAME.c_str(), e );
	} catch ( std::exception& e ) {
		MessageBox( NULL, strToWstr( e.what() )->c_str(), L"Error", MB_OK | MB_ICONERROR );
		SeraphException se;
		se.write( string::file::name::ERROR_LOG_NAME.c_str(), e );
	}
	int counter = 0;
	if ( maschinen::list_view::Table* lvd = dynamic_cast< maschinen::list_view::Table* >( lv.get() ) ) {
		for ( auto i = processors.begin(); i != processors.end(); ++i ) {
			for ( auto j = ( *i )->getMetaData()->getFiles()->begin(); j != ( *i )->getMetaData()->getFiles()->end(); ++j ) {
				lvd->addRow();
				//id
				lvd->setText( ( *i )->getMetaData()->getId()->c_str(), counter, 0 );
				//file_id
				lvd->setText( to_tstring( ( *j )->getId() ).c_str(), counter, 1 );
				//name
				lvd->setText( (*j)->getName()->c_str(), counter, 2 );
				//size
				lvd->setText( fileSizeToStr((*j)->getSize())->c_str(), counter, 3 );
				if ( (*j)->getSize() == 0 ) lvd->setText( L"0%", counter, 4 );
				else {
					double atas = ( double ) ( *i )->getMetaData()->getByteWritten( *j );
					double bawah = ( double ) ( *j )->getSize();
					double hasil = atas / bawah;
					tstring temp = to_tstring( hasil * 100 ).c_str();
					temp.append( L"%" );
					lvd->setText( ( LPWSTR ) temp.c_str(), counter, 4 );
				}

				lvd->setText( L"0", counter, 5 );
				lvd->setText( L"Infinite", counter, 6);
				lvd->setText( L"Disconnect", counter, 7 );
				counter++;
			}
		}
	}
}

std::unique_ptr<tstring> FileOperationWindow::getPath() {
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

void FileOperationWindow::onClick( maschinen::event::OnClickEvent& e ) {
	switch ( e.getSourceId() ) {
		case MENU_FILE_DOWNLOAD_WINDOW_START_DOWNLOAD:
		{
			if ( pop_up_selected == nullptr ) return; pop_up_selected->show();
		} break;

		case MENU_FILE_DOWNLOAD_WINDOW_OPEN_FILE_LOCATION_IN_EXPLORER:
		{
			if ( pop_up_selected == nullptr ) return;
			std::vector<tstring> paths;
			for ( auto i = pop_up_selected->getMetaData()->getFiles()->begin(); i != pop_up_selected->getMetaData()->getFiles()->end(); ++i ) {
				tstring path( pop_up_selected->getMetaData()->getTargetPath()->c_str() );
				path.append( TEXT( "\\" ) );
				path.append( ( *i )->getPath()->c_str() );
				if ( GetFileAttributes( path.c_str() ) != INVALID_FILE_ATTRIBUTES ) paths.push_back( path );
			}
			if ( paths.empty() ) return;
			maschinen::com::ComponentObjectModel com( COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE );
			com.openFolderAndSelectItems( pop_up_selected->getMetaData()->getTargetPath()->c_str(), &paths );
		} break;

		case MENU_FILE_DOWNLOAD_WINDOW_OPEN_FILE_LOCATION:
		{
		} break;

		case MENU_FILE_DOWNLOAD_WINDOW_DELETE_FILE:
		{
			if ( pop_up_selected == nullptr ) return;
			std::vector<tstring> paths;
			for ( auto i = pop_up_selected->getMetaData()->getFiles()->begin(); i != pop_up_selected->getMetaData()->getFiles()->end(); ++i ) {
				tstring path( pop_up_selected->getMetaData()->getTargetPath()->c_str() );
				path.append( TEXT( "\\" ) );
				path.append( ( *i )->getPath()->c_str() );
				if ( GetFileAttributes( path.c_str() ) != INVALID_FILE_ATTRIBUTES ) paths.push_back( path );
			}
			maschinen::com::ComponentObjectModel com( COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE );
			maschinen::com::FileOperation fo( &com );
			try {
				fo.deleteFile( &paths ); fo.commit();
			} catch ( maschinen::exception::FileNotDirectory& e ) {
				MessageBox( handler, e.getMessage()->c_str(), L"Error", MB_OK | MB_ICONERROR );
			} catch ( maschinen::exception::Exception& e ) {
				MessageBox( handler, e.getMessage()->c_str(), L"Error", MB_OK | MB_ICONERROR );
			}
		} break;

		case MENU_FILE_DOWNLOAD_WINDOW_DELETE:
		{
			if ( pop_up_selected == nullptr ) return;
			pop_up_selected->removeFile();
			pop_up_selected->destroy();
			processors.erase( std::remove_if( processors.begin(), processors.end(), [ & ]( std::unique_ptr<FileProcessWindow> const& p ) {
				return p.get() == pop_up_selected;
			} ), processors.end() );
			updateListView();
		} break;
	}
}

void FileOperationWindow::onListViewSelect( maschinen::event::OnListViewSelectEvent& e ) {
	switch (e.getMethod() ) {
		case NM_RCLICK:
		{
			if ( lv->getSelectedCount() == 0 ) return;
			pop_up_selected = getFileProcessWindowByDownloadId( ( ( maschinen::list_view::Table* )lv.get() )->getText( lv->getSelectedItemIndex()->front(), 0 )->c_str() );
			if ( pop_up_selected == nullptr ) return;
			std::vector<tstring> paths;
			for ( auto i = pop_up_selected->getMetaData()->getFiles()->begin(); i != pop_up_selected->getMetaData()->getFiles()->end(); ++i ) {
				tstring path( pop_up_selected->getMetaData()->getTargetPath()->c_str() );
				path.append( TEXT( "\\" ) );
				path.append( ( *i )->getPath()->c_str() );
				paths.push_back( path );
			}
			bool valid = false;
			for ( auto i = paths.cbegin(); i != paths.cend(); ++i ) {
				if ( GetFileAttributes( i->c_str() ) != INVALID_FILE_ATTRIBUTES ) {
					valid = true; break;
				}
			}
			if ( valid ) {
				menu.setVisible( MENU_FILE_DOWNLOAD_WINDOW_OPEN_FILE_LOCATION );
				menu.setVisible( MENU_FILE_DOWNLOAD_WINDOW_OPEN_FILE_LOCATION_IN_EXPLORER );
				menu.setVisible( MENU_FILE_DOWNLOAD_WINDOW_DELETE_FILE );
			}
			else {
				menu.setVisible( MENU_FILE_DOWNLOAD_WINDOW_OPEN_FILE_LOCATION, false );
				menu.setVisible( MENU_FILE_DOWNLOAD_WINDOW_OPEN_FILE_LOCATION_IN_EXPLORER, false );
				menu.setVisible( MENU_FILE_DOWNLOAD_WINDOW_DELETE_FILE, false );
			}
			menu.show();
		} break;
	}
}

FileOperationWindow::~FileOperationWindow()
{
}
