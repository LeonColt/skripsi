#include "stdafx.h"
#include "FileControl.h"


FileControl::FileControl(){wcscpy_s(error_marker, L"File Control Error");}

void FileControl::onClick( maschinen::event::OnClickEvent& e ) {
	switch ( e.getSourceId() ) {
		case menu_file_control_open:
		{
			BaseFile* temp = switcher->get();
			opener();
		} break;

		case menu_file_control_copy:
		{
			file_operation.files.clear();
			if ( switcher->isNoSelection() ) return;
			file_operation.operation = file_operation.operation = file_operation.COPY;
			for ( auto i = switcher->begin(); i != switcher->end(); ++i ) 
				file_operation.files.push_back( ( *i )->getPath()->c_str() );
		} break;
		case menu_File_control_cut:
		{
			file_operation.files.clear();
			if ( switcher->isNoSelection() ) return;
			file_operation.operation = file_operation.operation = file_operation.CUT;
			for ( auto i = switcher->begin(); i != switcher->end(); ++i ) 
				file_operation.files.push_back( ( *i )->getPath()->c_str() );
		} break;
		case menu_file_control_rename:
		{
			Position pos = ( ( maschinen::list_view::Table* )lv.get() )->getCellCoordinate( ( ( maschinen::list_view::Table* )lv.get() )->getSelectedItemIndex()->front(), 1 );
			renamer.setPosition( pos.x, pos.y, pos.width, pos.height );
			renamer.show();
			renamer.setText( switcher->get()->getName()->c_str() );
			SetWindowPos( renamer.getHandler(), HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE );
			SetFocus( renamer.getHandler() );
		} break;

		case menu_file_control_paste:
		{
			if ( file_operation.operation == file_operation.NO_OPERATION ) return;
			for( auto i = switcher->begin(); i != switcher->end(); ++i ) {
				fe->cleanSource();
				fe->setTarget( ( *i )->getPath()->c_str() );
				for ( auto j = file_operation.files.begin(); j != file_operation.files.end(); j++ )
					fe->addSource( j->c_str() );
				if ( file_operation.operation == file_operation.COPY ) {
					fe->operationCopy(); fe->execute();
				}
				else if ( file_operation.operation == file_operation.CUT ) {
					fe->operationCut();
					fe->execute();
				}
			}
			file_operation.operation = file_operation.NO_OPERATION;
			file_operation.files.clear();
		} break;

		case menu_file_control_new_folder:
		{
			for ( auto i = switcher->begin(); i != switcher->end(); ++i ) {
				if ( DriveFixed* df = dynamic_cast< DriveFixed* >( *i ) ) df->createNewDirectory();
				else if ( DriveRemovable* dr = dynamic_cast< DriveRemovable* >( *i ) ) dr->createNewDirectory();
				else if ( DirectoryLocal* dl = dynamic_cast< DirectoryLocal* >( *i ) ) dl->createNewDirectory();
				else if ( DirectoryCloud* dc = dynamic_cast< DirectoryCloud* > ( *i ) )
					cloud->createNewFolder( dc );
			}
		} break;

		case menu_file_control_delete:
		{
			if ( switcher->isNoSelection() ) return;
			int result = MessageBox( handler, L"Are you sure wanna delete these files?", L"Delete Files", MB_ICONQUESTION | MB_YESNO );
			if ( result == IDNO ) return;
			fe->cleanSource();
			for ( auto i = switcher->begin(); i != switcher->end(); ++i )
				fe->addSource( ( *i )->getPath()->c_str() );
			fe->operationDelete();
			fe->execute();
		} break;

		case MENU_FILE_CONTROL_RESTORE : {
			if ( switcher->isNoSelection() ) return;
			int result = MessageBox( handler, L"Are you sure wanna restore these files?", L"Restore Files", MB_ICONQUESTION | MB_YESNO );
			if ( result == IDNO ) return;
			fe->cleanSource();
			for ( auto i = switcher->begin(); i != switcher->end(); ++i ) 
				fe->addSource( ( *i )->getPath()->c_str() );
			fe->operationRestore();
			fe->execute();
		} break;

		case MENU_FILE_CONTROL_PROPERTY : {
			try {
				if ( !switcher->isNoSelection() ) {
					if ( switcher->isSingleSelection() ) {
						if(Cloud* c = dynamic_cast<Cloud*>( switcher->get() ) ) {
							maschinen::layout::LayoutMatrix* layout = new maschinen::layout::LayoutMatrix();
							layout->setNumberOfCell( 50, 50 );

							std::unique_ptr<CloudProperty> dcp = std::make_unique<CloudProperty>();
							dcp->setLayout( layout );
							dcp->setStyle( CS_HREDRAW | CS_VREDRAW );
							dcp->setIcon( LoadIcon( getInstance(), MAKEINTRESOURCE( IDI_SERAPH ) ) );
							dcp->setCursor( LoadCursor( nullptr, IDC_ARROW ) );
							dcp->setBackground( ( HBRUSH ) ( COLOR_WINDOW + 1 ) );
							dcp->setSmallIcon( LoadIcon( getInstance(), MAKEINTRESOURCE( IDI_SMALL ) ) );
							dcp->setAkey( akey );
							dcp->setSetting( setting );
							dcp->setUserCredential( uc );
							dcp->setFileControl( this );
							dcp->setCloud( c );
							dcp->create();
							tstring title( c->getName()->c_str() );
							title.append( TEXT( " Properties" ) );
							dcp->setTitle( title );
							dcp->show();
							properties.push_back( std::move( dcp ) );
						}
						else if( Local* local = dynamic_cast<Local*>( switcher->get() ) ) {
							maschinen::layout::LayoutMatrix* layout = new maschinen::layout::LayoutMatrix();
							layout->setNumberOfCell( 50, 50 );

							std::unique_ptr<LocalProperty> lp = std::make_unique<LocalProperty>();
							lp->setLayout( layout );
							lp->setStyle( CS_HREDRAW | CS_VREDRAW );
							lp->setIcon( LoadIcon( getInstance(), MAKEINTRESOURCE( IDI_SERAPH ) ) );
							lp->setCursor( LoadCursor( nullptr, IDC_ARROW ) );
							lp->setBackground( ( HBRUSH ) ( COLOR_WINDOW + 1 ) );
							lp->setSmallIcon( LoadIcon( getInstance(), MAKEINTRESOURCE( IDI_SMALL ) ) );
							lp->setAkey( akey );
							lp->setSetting( setting );
							lp->setUserCredential( uc );
							lp->setFileControl( this );
							lp->setLocal( local );
							lp->create();
							tstring title( local->getName()->c_str() );
							title.append( TEXT( " Properties" ) );
							lp->setTitle( title );
							lp->show();
							properties.push_back( std::move( lp ) );
						}
					}
				}
				else if ( pointer ) {
					if( Cloud* c = dynamic_cast<Cloud*> ( pointer.get() ) ) {
						maschinen::layout::LayoutMatrix* layout = new maschinen::layout::LayoutMatrix();
						layout->setNumberOfCell( 50, 50 );

						std::unique_ptr<CloudProperty> dcp = std::make_unique<CloudProperty>();
						dcp->setLayout( layout );
						dcp->setStyle( CS_HREDRAW | CS_VREDRAW );
						dcp->setIcon( LoadIcon( getInstance(), MAKEINTRESOURCE( IDI_SERAPH ) ) );
						dcp->setCursor( LoadCursor( nullptr, IDC_ARROW ) );
						dcp->setBackground( ( HBRUSH ) ( COLOR_WINDOW + 1 ) );
						dcp->setSmallIcon( LoadIcon( getInstance(), MAKEINTRESOURCE( IDI_SMALL ) ) );
						dcp->setAkey( akey );
						dcp->setSetting( setting );
						dcp->setUserCredential( uc );
						dcp->setFileControl( this );
						dcp->setCloud( c );
						dcp->create();
						tstring title( c->getName()->c_str() );
						title.append( TEXT( " Properties" ) );
						dcp->setTitle( title );
						dcp->show();
						properties.push_back( std::move( dcp ) );
					}
				}
			} catch( SeraphException& e ) {
				MessageBox( getWindowHandler(), e.getMessage()->c_str(), error_marker, MB_ICONERROR | MB_OK );
			} catch( maschinen::exception::Exception& e ) {
				SeraphException se;
				se.write( string::file::name::ERROR_LOG_NAME.c_str(), e );
				MessageBox( getWindowHandler(), e.getMessage()->c_str(), error_marker, MB_ICONERROR | MB_OK );
			} catch( std::exception& e ) {
				SeraphException se;
				se.write( string::file::name::ERROR_LOG_NAME.c_str(), e );
				MessageBox( getWindowHandler(), strToWstr(e.what())->c_str(), error_marker, MB_ICONERROR | MB_OK );
			}
		} break;

		case button_file_control_back: {
			setFileControLoadDisable();
			std::vector<tstring> paths;
			size_t size = tstrSplit( url.getText()->c_str(), &paths, TEXT( "\\" ) );
			tstring new_path;
			if( paths.size() > 1 ) {
				for ( auto i = paths.cbegin(); i != paths.cend() - 1; ++i ) {
					if ( i != paths.cbegin() ) new_path.append( TEXT( "\\" ) );
					new_path.append( i->c_str() );
				}
				new_path.append( TEXT( "\\" ) );
			}
			url.setText( new_path.c_str() );
			updateListView();
		} break;
	}
}

void FileControl::onListViewSelect( maschinen::event::OnListViewSelectEvent& e ) {
	switch ( e.getMethod() ) {
		case NM_RCLICK:
		{
			try {
				switcher->retriveSelectedFile( lv.get(), e );
				if ( !switcher->isNoSelection() ) {
					{
						bool show_open = true;
						for( auto i = switcher->begin(); i != switcher->end(); ++i ) {
							if( Cloud* c = dynamic_cast<Cloud*>( *i ) ) {
								if ( c->getName()->compare( setting->getCloudRootName()->c_str() ) == 0 ) continue;
								if ( !c->isDownloadable() ) { show_open = false; break; }
							}
							else if( Local* l = dynamic_cast<Local*>( *i ) ) {
								if ( !l->isOpenable() ) { show_open = false; break; }
							}
							else if( CloudRecycleBinObject* crbo = dynamic_cast<CloudRecycleBinObject*>( *i ) ) {
								show_open = false; break;
							}
						}
						pop_up_menu.setVisible( menu_file_control_open, show_open );
					}
					{
						bool show_cut = true;
						for ( auto i = switcher->begin(); i != switcher->end(); ++i ) {
							if ( Cloud* c = dynamic_cast<Cloud*>( *i ) ) {
								if ( c->getName()->compare( setting->getCloudRootName()->c_str() ) == 0 ) {show_cut = false; break;}
							}
							else if ( Local* l = dynamic_cast<Local*>( *i ) ) {
								if ( !l->isMovable() ) { show_cut = false; break; }
							} else if(CloudRecycleBin* crb = dynamic_cast<CloudRecycleBin*>( *i ) ) {show_cut = false; break;}
							else if( CloudRecycleBinObject* crbo = dynamic_cast<CloudRecycleBinObject*>( *i )) {show_cut = false; break;}
						}
						pop_up_menu.setVisible( menu_File_control_cut, show_cut );
					}
					{
						bool show_copy = true;
						for ( auto i = switcher->begin(); i != switcher->end(); ++i ) {
							if ( Cloud* c = dynamic_cast<Cloud*>( *i ) ) {
								if ( c->getName()->compare( setting->getCloudRootName()->c_str() ) == 0 ) { show_copy = false; break; }
							}
							else if ( Local* l = dynamic_cast<Local*>( *i ) ) {
								if ( !l->isDuplicatable() ) { show_copy = false; break; }
							}
							else if ( CloudRecycleBin* crb = dynamic_cast<CloudRecycleBin*>( *i ) ) {show_copy = false; break;}
							else if ( CloudRecycleBinObject* crbo = dynamic_cast<CloudRecycleBinObject*>( *i ) ) { show_copy = false; break; }
						}
						pop_up_menu.setVisible( menu_file_control_copy, show_copy );
					}
					{
						bool show_paste = true;
						if ( file_operation.operation == file_operation.NO_OPERATION ) show_paste = false;
						for ( auto i = switcher->begin(); i != switcher->end() && show_paste; ++i ) {
							if ( FileCloud* fc = dynamic_cast<FileCloud*>( *i ) ) {show_paste = false; break;}
							if ( FileLocal* fl = dynamic_cast<FileLocal*>( *i ) ) {show_paste = false; break;}
							if ( CloudRecycleBin* crb = dynamic_cast<CloudRecycleBin*>( *i ) ) {show_paste = false; break;}
							else if ( CloudRecycleBinObject* crbo = dynamic_cast<CloudRecycleBinObject*>( *i ) ) { show_paste = false; break; }
						}
						pop_up_menu.setVisible( menu_file_control_paste, show_paste );
					}
					{
						bool show_rename = true;
						for ( auto i = switcher->begin(); i != switcher->end(); ++i ) {
							if ( Drive* d = dynamic_cast< Drive* >( *i ) ) { show_rename = false; break; }
							else if ( Cloud* c = dynamic_cast<Cloud*>( *i ) ) {
								if ( c->getName()->compare( setting->getCloudRootName()->c_str() ) == 0 ) { show_rename = false; break; }
							}
							else if ( CloudRecycleBin* crb = dynamic_cast<CloudRecycleBin*>( *i ) ) { show_rename = false; break; }
							else if ( CloudRecycleBinObject* crbo = dynamic_cast<CloudRecycleBinObject*>( *i ) ) { show_rename = false; break; }
						}
						pop_up_menu.setVisible( menu_file_control_rename, show_rename );
					}
					{
						bool show_delete = true;
						for ( auto i = switcher->begin(); i != switcher->end(); ++i ) {
							if ( Cloud* c = dynamic_cast<Cloud*>( *i ) ) {
								if ( c->getName()->compare( TEXT( "Cloud" ) ) == 0 ) { show_delete = false; break; }
								if ( !c->isDeletable() ) { show_delete = false; break; }
							}
							else if ( Local* l = dynamic_cast<Local*>( *i ) ) {
								if ( !l->isDeletable() ) { show_delete = false; break; }
							}
							else if ( CloudRecycleBin* crb = dynamic_cast<CloudRecycleBin*>( *i ) ) { show_delete = false; break; }
						}
						pop_up_menu.setVisible( menu_file_control_delete, show_delete );
					}
					{
						bool show_new_folder = true;
						for ( auto i = switcher->begin(); i != switcher->end(); ++i ) {
							if ( CloudRecycleBin* crb = dynamic_cast<CloudRecycleBin*>( *i ) ) { show_new_folder = false; break; }
							else if ( CloudRecycleBinObject* crbo = dynamic_cast<CloudRecycleBinObject*>( *i ) ) { show_new_folder = false; break; }
						}
						pop_up_menu.setVisible( menu_file_control_new_folder, show_new_folder );
					}
					{
						bool show_restore = true;
						for( auto i = switcher->begin(); i != switcher->end(); ++i ) {
							if ( CloudRecycleBinObject* crbo = dynamic_cast< CloudRecycleBinObject* >( *i ) ) continue;
							else {
								show_restore = false;
								break;
							}
						}
						pop_up_menu.setVisible( MENU_FILE_CONTROL_RESTORE, show_restore );
					}
					pop_up_menu.show();
				}
				else {
					if ( ( pointer.get() != local->get() || pointer.get() != cloud->get() ) && !url.isTextEmpty() ) {
						if ( Cloud* c = dynamic_cast< Cloud* >( pointer.get() ) ) {
							pop_up_menu.setVisibleVisible( { menu_file_control_new_folder } );
							if ( file_operation.operation != file_operation.NO_OPERATION )
								pop_up_menu.setVisible( menu_file_control_paste, file_operation.operation != file_operation.NO_OPERATION );
							pop_up_menu.setVisible( MENU_FILE_CONTROL_PROPERTY );
							switcher->addSelected( c );
						}
						else if ( Local* l = dynamic_cast< Local* >( pointer.get() ) ) {
							pop_up_menu.setVisibleVisible( { menu_file_control_new_folder } );
							if ( file_operation.operation != file_operation.NO_OPERATION )
								pop_up_menu.setVisible( menu_file_control_paste, file_operation.operation != file_operation.NO_OPERATION );
							pop_up_menu.setVisible( MENU_FILE_CONTROL_PROPERTY );
							switcher->addSelected( l );
						}
						pop_up_menu.show();
					}
				}
			} catch ( maschinen::exception::Exception& e ) {
				MessageBox( NULL, e.getMessage()->c_str(), L"Error", MB_OK | MB_ICONERROR );
			}
		} break;

		case NM_DBLCLK:
		{
			switcher->retriveSelectedFile( lv.get(), e );
			opener();
		} break;
	}
}

void FileControl::opener() {
	BaseFile* temp = switcher->get();
	if ( temp != nullptr ) {
		if ( DriveFixed* df = dynamic_cast< DriveFixed* >( temp ) ) {
			url.setText( df->getPath()->c_str() );
			setFileControLoadDisable();
			lv->deleteAllItems();
			updateListView();
		}
		else if ( DirectoryLocal* dl = dynamic_cast< DirectoryLocal* >( temp ) ) {
			url.setText( dl->getPath()->c_str() );
			setFileControLoadDisable();
			lv->deleteAllItems();
			updateListView();
		}
		else if ( DirectoryCloud* dc = dynamic_cast< DirectoryCloud* >( temp ) ) {
			url.setText( dc->getPath()->c_str() );
			setFileControLoadDisable();
			lv->deleteAllItems();
			updateListView();
		}
		else if ( FileLocal* fl = dynamic_cast< FileLocal* >( temp ) )
			ShellExecute( getWindowHandler(), L"open", fl->getPath()->c_str(), NULL, NULL, SW_SHOW );
		else if ( FileCloud* fc = dynamic_cast< FileCloud* >( temp ) ) {
			int result = MessageBox( parent->getHandler(), L"you want to download this file?", L"Download", MB_ICONQUESTION | MB_YESNO );
			//if ( result == IDYES ) downloader->execute( fc );
		}
		else if ( CloudRecycleBin* crb = dynamic_cast<CloudRecycleBin*>( temp ) ) {
			url.setText( crb->getPath()->c_str() );
			setFileControLoadDisable();
			lv->deleteAllItems();
			updateListView();
		}
	}
}

void FileControl::onKeyUp( maschinen::Controller* control, WPARAM key ) {
	if( control->getId() == search.getId() ) {
		if ( key == VK_RETURN ) {
			url.setText( TEXT( "" ) );
			status->setText( TEXT( "Searching" ) );
			cleanBuffer();
			refreshListView();
			local->setSearchName( search.getText()->c_str() );
			if ( local->isStopped() ) local->execute(); else local->restart();
			cloud->setSearchName( search.getText()->c_str() );
			if ( cloud->isStopped() ) cloud->execute(); else cloud->restart();
			flag_search_cloud_finish = flag_search_local_finish = false;
			pb.styleMarquee();
			pb.show();
			pb.setMarqueeSpeed( true, 1 );
		}
	}
}

void FileControl::onKeyDown( maschinen::Controller* control, WPARAM key ) {}
void FileControl::onFocus( maschinen::Controller* control, bool focus ) {
	if(control->getId() == renamer.getId() ) {
		if( focus == false ) {
			renamer.hide();
			if ( switcher->isSingleSelection() ) {
				if ( Local* l = dynamic_cast< Local* > ( switcher->get() ) ) {
					std::vector<tstring> names;
					tstrSplit( l->getPath()->c_str(), &names, TEXT( "\\" ) );
					tstring path;
					for( auto i = names.cbegin(); i != names.cend() - 1; ++i ){
						path.append( i->c_str() );
						path.append( TEXT( "\\" ) );
					}
					path.append( renamer.getText()->c_str() );
					l->rename( path.c_str() );
				}
				else if ( Cloud* c = dynamic_cast< Cloud* > ( switcher->get() ) )
					cloud->renameFile( c->getId(), renamer.getText()->c_str() );
			}
		}
	}
}

void FileControl::onShow()
{
	maschinen::Container::onShow();
	updateListView();
	pb.hide();
}

void FileControl::onHide()
{
	maschinen::Container::onHide();
	if ( !local->isStopped() )local->stop();
	if ( !cloud->isStopped() ) cloud->stop();
	while ( local->isAlive() || cloud->isAlive() ) std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
}

void FileControl::onDestroy() {
	pointer = nullptr;
	if ( !local->isStopped() ) local->stop();
	if ( !cloud->isStopped() ) cloud->stop();
	for ( auto i = properties.cbegin(); i != properties.cend(); ++i ) 
		( *i )->destroy();
	properties.clear();
	DWORD exit_code = 0;
	while ( local->isAlive() || cloud->isAlive() ) std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
	CustomContainer::onDestroy();
}

void FileControl::onCreate()
{
	btt_back.setParentAndId(this, button_file_control_back);
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &btt_back, 0, 0, 0, 3, 3, 0 );
	btt_back.create();
	btt_back.setText(L"back");
	btt_back.setOnClickListener( this, false );

	pb.setParentAndId(this, progress_bar_file_control_pb);
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &pb, 4, 0, 0, 10, 3, 0 );
	pb.styleMarquee();
	pb.create();

	url.setParentAndId(this, edit_file_control_url);
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &url, 15, 0, 0, 50, 3, 0 );
	url.create();

	renamer.setParentAndId( this, edit_file_control_file_rename );
	renamer.setPosition( 0, 0, 1, 1 );
	renamer.setStyleClipSibling();
	renamer.create();
	renamer.setOnFocusListener( this );


	search.setParentAndId(this, button_file_control_refresh);
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &search, 66, 0, 0, 3, 3, 0 );
	search.create();
	search.setOnKeyPressedChangeListener( this );

	lv = std::make_unique<maschinen::list_view::Table>();
	lv->setParentAndId(this, list_view_file_control_file_shower);
	lv->setOnListViewSelectListener( this );
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( lv.get(), 0, 4, 0, FILL_PARENT, FILL_PARENT, 0 );
	if ( maschinen::list_view::Table* table = dynamic_cast< maschinen::list_view::Table* >( lv.get() ) ) {
		table->applyFullRowSelect();
		table->applyShowGridLines();
		table->applyHeaderDragDrop();
		table->autoWidth();
		table->create();
		table->addColumn( L"ID" );
		table->addColumn( L"Name" );
		table->addColumn( L"Type" );
		table->addColumn( L"Owner" );
		table->addColumn( L"Size" );
	} 
	else if ( maschinen::list_view::ListView* list_view = dynamic_cast< maschinen::list_view::ListView* >( lv.get() ) ) {
	}


	pointer = nullptr;
	local = std::make_unique<LocalChangeDetector>();
	local->setParent( this );

	cloud = std::make_unique<CloudFileOperator>();
	cloud->setParent( this );
	cloud->setAkey( akey );
	cloud->setSetting( setting );
	cloud->setUserCredential( uc );
	cloud->setErrorMarker( error_marker );

	switcher = std::make_unique<FileControlSelectSwitcher>( this );

	pop_up_menu.create();
	pop_up_menu.flagPopUpMenuLeftAlign();
	pop_up_menu.flagPopUpMenuBottomAlign();
	pop_up_menu.setParent( (maschinen::Window*)getParent() );
	pop_up_menu.positionbyCursor();

	pop_up_menu.append( menu_file_control_open, L"Open" );
	pop_up_menu.append( menu_File_control_cut, L"Cut" );
	pop_up_menu.append( menu_file_control_copy, L"Copy" );
	pop_up_menu.append( menu_file_control_paste, L"Paste" );
	pop_up_menu.append( menu_file_control_rename, L"Rename" );
	pop_up_menu.append( menu_file_control_delete, L"Delete" );
	pop_up_menu.append( menu_file_control_new_folder, L"Create New Folder" );
	pop_up_menu.append( MENU_FILE_CONTROL_RESTORE, TEXT( "Restore" ) );
	pop_up_menu.append( MENU_FILE_CONTROL_PROPERTY, TEXT( "Property" ) );
	pop_up_menu.setOnClickListener( this );

	fe = std::make_unique<FileExecutor>( this, error_marker, akey, setting, uc );

	flag_search_cloud_finish = flag_search_local_finish = true;

	mapper.read();
}
void FileControl::setFileControLoadDisable()
{
	pb.styleMarquee();
	pb.show();
	pb.setMarqueeSpeed(true, 1);
	lv->disable();
	btt_back.disable();
	search.disable();
}

void FileControl::setFileControlLoadEnable()
{
	pb.styleMarquee(false);
	pb.hide();
	lv->enable();
	btt_back.enable();
	search.enable();
}
void FileControl::updateListView()
{
	URLParser* parser = new URLParser();
	parser->setParent( this );
	parser->setAutoDelete();
	parser->setAkey( akey );
	parser->setSetting( setting );
	parser->setUserCredential( uc );
	parser->setErrorMarker( error_marker );
	tstring* path = new tstring( url.getText()->c_str() );
	parser->execute( (LPVOID)path );
}

void FileControl::prosesListView() {
	if( previous_ptr != nullptr && pointer != nullptr ) {
		if ( *previous_ptr.get() != *pointer.get() )
			file_history.push_back( std::move( previous_ptr ) );
	}
	file_buffer.clear();
	if ( pointer == nullptr ) {
		if ( !cloud->isStopped() ) cloud->stop();
		if ( !local->isStopped() ) local->stop();
		DriveLoader loader;
		for ( auto i = loader.begin(); i != loader.end(); ++i )
			file_buffer.push_back( std::unique_ptr<BaseFile>( *i ) );
		DirectoryCloud* dc = new DirectoryCloud();
		dc->setId( 0 );
		dc->setName( setting->getCloudRootName()->c_str() );
		tstring path = setting->getCloudRootName()->c_str();
		path.append( TEXT( "\\" ) );
		dc->setPath( path.c_str() );
		dc->setModifyByAll( true );
		dc->setDownloadable( false );
		dc->setDownloadByAll( false );
		dc->setSeenByAll( true );
		if( uc->isCredential() ) 
			dc->setOwner( uc->getUserName()->c_str() );
		else {
			TCHAR buffer[ MAX_LOADSTRING ];
			DWORD size = MAX_LOADSTRING;
			GetUserName( buffer, &size );
			dc->setOwner( buffer );
		}
		file_buffer.push_back( std::unique_ptr<BaseFile>( dc ) );

		if( uc->isCredential() ) {
			std::unique_ptr<CloudRecycleBin> crb = std::make_unique<CloudRecycleBin>();
			crb->setName( setting->getCloudRecycleBinRootName()->c_str() );
			tstring path = setting->getCloudRecycleBinRootName()->c_str();
			path.append( TEXT( "\\" ) );
			crb->setPath( path.c_str() );
			file_buffer.push_back( std::move( crb ) );
		}
	}
	else {
		if ( DriveFixed* df = dynamic_cast<DriveFixed*>( pointer.get() ) ) {
			for ( auto i = df->begin(); i != df->end(); ++i )
				file_buffer.push_back( std::unique_ptr<BaseFile>( *i ) );
			if ( !cloud->isStopped() ) cloud->stop();
			if ( local->isStopped() ) local->execute(); else local->restart();
		}
		else if ( DriveRemovable* dr = dynamic_cast<DriveRemovable*>( pointer.get() ) ) {
			for ( auto i = dr->begin(); i != dr->end(); ++i )
				file_buffer.push_back( std::unique_ptr<BaseFile>( *i ) );
			if ( !cloud->isStopped() ) cloud->stop();
			if ( local->isStopped() ) local->execute(); else local->restart();
		}
		else if ( DirectoryLocal* dl = dynamic_cast<DirectoryLocal*>( pointer.get() ) ) {
			for ( auto i = dl->begin(); i != dl->end(); ++i )
				file_buffer.push_back( std::unique_ptr<BaseFile>( *i ) );
			if ( !cloud->isStopped() ) cloud->stop();
			if ( local->isStopped() ) local->execute(); else local->restart();
		}
		else if ( DirectoryCloud* dc = dynamic_cast<DirectoryCloud*>( pointer.get() ) ) {
			for ( auto i = dc->begin(); i != dc->end(); i++ ){
				if( DirectoryCloud* child = dynamic_cast<DirectoryCloud*>( *i ) ) {
					std::unique_ptr<BaseFile> temp = std::make_unique<DirectoryCloud>();
					temp->setName( child->getName()->c_str() );
					temp->setPath( child->getPath()->c_str() );
					( ( DirectoryCloud* ) temp.get() )->setId( child->getId() );
					( ( DirectoryCloud* ) temp.get() )->setOwner( child->getOwner()->c_str() );
					( ( DirectoryCloud* ) temp.get() )->setParentId( child->getParentId() );
					( ( DirectoryCloud* ) temp.get() )->setParentId( child->getParentId() );
					( ( DirectoryCloud* ) temp.get() )->setDeletable( child->isDeletable() );
					( ( DirectoryCloud* ) temp.get() )->setDownloadable( child->isDownloadable() );
					( ( DirectoryCloud* ) temp.get() )->setDownloadByAll( child->isDownloadByAll() );
					( ( DirectoryCloud* ) temp.get() )->setModifyByAll( child->isModifyByAll() );
					( ( DirectoryCloud* ) temp.get() )->setSeenByAll( child->isSeenByAll() );
					( ( DirectoryCloud* ) temp.get() )->setSize( child->getSize() );
					file_buffer.push_back( std::move( temp ) );
				}
				else if( FileCloud* child = dynamic_cast<FileCloud*>( *i ) ) {
					std::unique_ptr<BaseFile> temp = std::make_unique<FileCloud>();
					temp->setName( child->getName()->c_str() );
					temp->setPath( child->getPath()->c_str() );
					( ( FileCloud* ) temp.get() )->setId( child->getId() );
					( ( FileCloud* ) temp.get() )->setOwner( child->getOwner()->c_str() );
					( ( FileCloud* ) temp.get() )->setParentId( child->getParentId() );
					( ( FileCloud* ) temp.get() )->setParentId( child->getParentId() );
					( ( FileCloud* ) temp.get() )->setDeletable( child->isDeletable() );
					( ( FileCloud* ) temp.get() )->setDownloadable( child->isDownloadable() );
					( ( FileCloud* ) temp.get() )->setDownloadByAll( child->isDownloadByAll() );
					( ( FileCloud* ) temp.get() )->setModifyByAll( child->isModifyByAll() );
					( ( FileCloud* ) temp.get() )->setSeenByAll( child->isSeenByAll() );
					( ( FileCloud* ) temp.get() )->setSize( child->getSize() );
					file_buffer.push_back( std::move( temp ) );
				}
			}
			if ( !local->isStopped() ) local->stop();
			if ( cloud->isStopped() ) cloud->execute();
			else cloud->restart();
		}
		else if( CloudRecycleBin* crb = dynamic_cast<CloudRecycleBin*>( pointer.get() ) ) {
			for ( auto i = crb->begin(); i != crb->end(); ++i ) {
				std::unique_ptr<BaseFile> crbo = std::make_unique<CloudRecycleBinObject>();
				( ( CloudRecycleBinObject* ) crbo.get() )->setAsDirectory( ( ( CloudRecycleBinObject* ) ( *i ) )->isDirectory() );
				( ( CloudRecycleBinObject* ) crbo.get() )->setDateDeleted( ( ( CloudRecycleBinObject* ) ( *i ) )->getDateDeleted()->c_str() );
				( ( CloudRecycleBinObject* ) crbo.get() )->setId( ( ( CloudRecycleBinObject* ) ( *i ) )->getId() );
				( ( CloudRecycleBinObject* ) crbo.get() )->setName( ( ( CloudRecycleBinObject* ) ( *i ) )->getName()->c_str() );
				( ( CloudRecycleBinObject* ) crbo.get() )->setOwner( ( ( CloudRecycleBinObject* ) ( *i ) )->getOwner()->c_str() );
				( ( CloudRecycleBinObject* ) crbo.get() )->setPath( ( ( CloudRecycleBinObject* ) ( *i ) )->getPath()->c_str() );
				file_buffer.push_back( std::move( crbo ) );
			}
			if ( !local->isStopped() ) local->stop();
			if ( cloud->isStopped() ) cloud->execute();
			else cloud->restart();
		}
	}
	refreshListView();
}

void FileControl::refreshListView() {
	flag_updating_list_view = true;
	DWORD error_code = 0;
	bool exists = false;
	std::list<tstring> existsing;
	if ( maschinen::list_view::Table* table = dynamic_cast< maschinen::list_view::Table* >( lv.get() ) ) {
		for ( auto i = file_buffer.cbegin(); i != file_buffer.cend(); ++i ) {
			BaseFile* bf = i->get();
			if ( DriveFixed* df = dynamic_cast< DriveFixed* >( bf ) ) {
				exists = false;
				for ( auto j = 0; j < ( int ) table->getRowCount() && !exists; j++ ) {
					if( table->getText( j, 1)->compare( df->getName()->c_str() ) == 0 ) {
						table->setText( df->getName()->c_str(), j, 1 );
						{
							tstring temp = df->getFileSystemName()->c_str();
							temp.append( TEXT( " Fixed" ) );
							table->setText( temp.c_str(), j, 2 );
						}
						existsing.push_back( df->getName()->c_str() );
						exists = true;
					}
				}
				if ( !exists ) {
					table->addRow();
					table->setText( df->getName()->c_str(), table->getRowCount() - 1, 1 );
					{
						tstring temp = df->getFileSystemName()->c_str();
						temp.append( TEXT( " Fixed" ) );
						table->setText( temp.c_str(), table->getRowCount() - 1, 2 );
					}
					table->setText( df->getOwner()->c_str(), table->getRowCount() - 1, 3 );
					existsing.push_back( df->getName()->c_str() );
				}
			}
			else if ( DriveRemovable* dr = dynamic_cast< DriveRemovable* >( bf ) ) {
				exists = false;
				for ( auto j = 0; j < ( int ) table->getRowCount() && !exists; j++ ) {
					if ( table->getText( j, 1 )->compare( dr->getName()->c_str() ) == 0 ) {
						table->setText( dr->getName()->c_str(), j, 1 );
						{
							tstring temp = dr->getFileSystemName()->c_str();
							temp.append( TEXT( " Removable" ) );
							table->setText( temp.c_str(), j, 2 );
						}
						existsing.push_back( dr->getName()->c_str() );
						exists = true;
					}
				}
				if ( !exists ) {
					table->addRow();
					table->setText( dr->getName()->c_str(), table->getRowCount() - 1, 1 );
					{
						tstring temp = dr->getFileSystemName()->c_str();
						temp.append( TEXT( " Removable" ) );
						table->setText( temp.c_str(), table->getRowCount() - 1, 2 );
					}
					table->setText( dr->getOwner()->c_str(), table->getRowCount() - 1, 3 );
					existsing.push_back( dr->getName()->c_str() );
				}
			}
			else if ( DriveCDRom* dcdr = dynamic_cast< DriveCDRom* >( bf ) ) {
				exists = false;
				for ( auto j = 0; j < ( int ) table->getRowCount() && !exists; j++ ) {
					if ( table->getText( j, 1 )->compare( dcdr->getName()->c_str() ) == 0 ) {
						table->setText( dcdr->getName()->c_str(), j, 1 );
						{
							tstring temp = dcdr->getFileSystemName()->c_str();
							temp.append( TEXT( " CDRom" ) );
							table->setText( temp.c_str(), j, 2 );
						}
						table->setText( uc->getUserName()->c_str(), j, 4 );
						existsing.push_back( dcdr->getName()->c_str() );
						exists = true;
					}
				}
				if ( !exists ) {
					table->addRow();
					table->setText( dcdr->getName()->c_str(), table->getRowCount() - 1, 1 );
					{
						tstring temp = dcdr->getFileSystemName()->c_str();
						temp.append( TEXT( " CDRom" ) );
						table->setText( temp.c_str(), table->getRowCount() - 1, 2 );
					}
					table->setText( dcdr->getOwner()->c_str(), table->getRowCount() - 1, 3 );
					existsing.push_back( dcdr->getName()->c_str() );
				}
			}
			else if ( DirectoryLocal* dl = dynamic_cast< DirectoryLocal* >( bf ) ) {
				exists = false;
				for ( auto j = 0; j < ( int ) table->getRowCount() && !exists; j++ ) {
					if ( table->getText( j, 1 )->compare( dl->getName()->c_str() ) == 0 ) {
						table->setText( dl->getName()->c_str(), j, 1 );
						table->setText( TEXT( "Directory" ), j, 2 );
						table->setText( dl->getOwner()->c_str(), j, 3 ); //owner

						table->setText( uc->getUserName()->c_str(), j, 4 );
						existsing.push_back( dl->getName()->c_str() );
						exists = true;
					}
				}
				if ( !exists ) {
					table->addRow();
					table->setText( dl->getName()->c_str(), table->getRowCount() - 1, 1 );
					table->setText( TEXT( "Directory" ), table->getRowCount() - 1, 2 );
					try {
						table->setText( dl->getOwner()->c_str(), table->getRowCount() - 1, 3 ); //owner
					} catch ( maschinen::exception::Exception& e ) {
						TCHAR error[ MAX_LOADSTRING ];
						if ( e.getErrorCode() != error_code ) {
							e.getMessage( error, MAX_LOADSTRING );
							MessageBox( parent->getHandler(), error, L"Error File Control", MB_OK | MB_ICONERROR );
							table->setText( L"?", table->getRowCount() - 1, 3 );
							error_code = e.getErrorCode();
						}
						else table->setText( L"?", table->getRowCount() - 1, 3 );
					}
					existsing.push_back( dl->getName()->c_str() );
				}
			}
			else if ( FileLocal* fl = dynamic_cast< FileLocal* >( bf ) ) {
				exists = false;
				for ( auto j = 0; j < ( int ) table->getRowCount() && !exists; j++ ) {
					if ( table->getText( j, 1 )->compare( fl->getName()->c_str() ) == 0 ) {
						table->setText( fl->getName()->c_str(), j, 1 );
						table->setText( fl->getType()->c_str(), j, 2 );
						try {
							table->setText( fl->getOwner()->c_str(), j, 3 ); //owner
						} catch ( maschinen::exception::Exception& e ) {
							TCHAR error[ MAX_LOADSTRING ];
							if ( e.getErrorCode() != error_code ) {
								e.getMessage( error, MAX_LOADSTRING );
								MessageBox( parent->getHandler(), error, L"Error File Control", MB_OK | MB_ICONERROR );
								table->setText( L"?", j, 3 );
								error_code = e.getErrorCode();
							}
							else table->setText( L"?", j, 3 );
						}
						try {
							table->setText( to_tstring( fl->getSize() ).c_str(), j, 4 );
						} catch ( maschinen::exception::Exception& e ) {
							TCHAR error[ MAX_LOADSTRING ];
							if ( e.getErrorCode() != error_code ) {
								e.getMessage( error, MAX_LOADSTRING );
								MessageBox( parent->getHandler(), error, L"Error File Control", MB_OK | MB_ICONERROR );
								table->setText( L"?", j, 4 );
								error_code = e.getErrorCode();
							}
							else table->setText( L"?", j, 4 );
						}
						existsing.push_back( fl->getName()->c_str() );
						exists = true;
					}
				}
				if ( !exists ) {
					table->addRow();
					table->setText( fl->getName()->c_str(), table->getRowCount() - 1, 1 );
					table->setText( fl->getType()->c_str(), table->getRowCount() - 1, 2 );
					try {
						table->setText( fl->getOwner()->c_str(), table->getRowCount() - 1, 3 ); //owner
					} catch ( maschinen::exception::Exception& e ) {
						TCHAR error[ MAX_LOADSTRING ];
						if ( e.getErrorCode() != error_code ) {
							e.getMessage( error, MAX_LOADSTRING );
							MessageBox( parent->getHandler(), error, L"Error File Control", MB_OK | MB_ICONERROR );
							table->setText( L"?", table->getRowCount() - 1, 3 );
							error_code = e.getErrorCode();
						}
						else table->setText( L"?", table->getRowCount() - 1, 3 );
					}
					try {
						table->setText( to_tstring( fl->getSize() ).c_str(), table->getRowCount() - 1, 4 );
					} catch ( maschinen::exception::Exception& e ) {
						TCHAR error[ MAX_LOADSTRING ];
						if ( e.getErrorCode() != error_code ) {
							e.getMessage( error, MAX_LOADSTRING );
							MessageBox( parent->getHandler(), error, L"Error File Control", MB_OK | MB_ICONERROR );
							table->setText( L"?", table->getRowCount() - 1, 4 );
							error_code = e.getErrorCode();
						}
						else table->setText( L"?", table->getRowCount() - 1, 4 );
					}
					existsing.push_back( fl->getName()->c_str() );
				}
			}
			else if ( DirectoryCloud* dc = dynamic_cast< DirectoryCloud* >( bf ) ) {
				exists = false;
				for ( auto j = 0; j < ( int ) table->getRowCount() && !exists; j++ ) {
					if ( table->getText( j, 1 )->compare( dc->getName()->c_str() ) == 0 ) {
						table->setText( dc->getName()->c_str(), j, 1 );
						table->setText( TEXT( "Directory" ), j, 2 );
						table->setText( dc->getOwner()->c_str(), j, 3 );
						existsing.push_back( dc->getName()->c_str() );
						exists = true;
					}
				}
				if ( !exists ) {
					table->addRow();
					table->setText( dc->getName()->c_str(), table->getRowCount() - 1, 1 );
					table->setText( TEXT("Directory"), table->getRowCount() - 1, 2 );
					table->setText( dc->getOwner()->c_str(), table->getRowCount() - 1, 3 ); //owner
					existsing.push_back( dc->getName()->c_str() );
				}
			}
			else if ( FileCloud* fc = dynamic_cast< FileCloud* >( bf ) ) {
				exists = false;
				for ( auto j = 0; j < ( int ) table->getRowCount() && !exists; j++ ) {
					if ( table->getText( j, 1 )->compare( fc->getName()->c_str() ) == 0 ) {
						table->setText( fc->getName()->c_str(), j, 1 );
						table->setText( fc->getType()->c_str(), j, 2 );
						table->setText( fc->getOwner()->c_str(), j, 3 );
						table->setText( to_tstring( fc->getSize() ).c_str(), j, 4 );
						existsing.push_back( fc->getName()->c_str() );
						exists = true;
					}
				}
				if ( !exists ) {
					table->addRow();
					table->setText( fc->getName()->c_str(), table->getRowCount() - 1, 1 );
					table->setText( fc->getType()->c_str(), table->getRowCount() - 1, 2 );
					table->setText( fc->getOwner()->c_str(), table->getRowCount() - 1, 3 ); //owner
					table->setText( to_tstring( fc->getSize() ).c_str(), table->getRowCount() - 1, 4 );
					existsing.push_back( fc->getName()->c_str() );
				}
			}
			else if( CloudRecycleBin* crb = dynamic_cast<CloudRecycleBin*>( bf ) ) {
				exists = false;
				for ( auto j = 0; j < ( int ) table->getRowCount() && !exists; j++ ) {
					if ( table->getText( j, 1 )->compare( crb->getName()->c_str() ) == 0 ) {
						table->setText( crb->getName()->c_str(), j, 1 );
						table->setText( TEXT( "Recyble Bin" ), j, 2 );
						table->setText( uc->getUserName()->c_str(), j, 3 );
						existsing.push_back( crb->getName()->c_str() );
						exists = true;
					}
				}
				if ( !exists ) {
					table->addRow();
					table->setText( TEXT( "0" ), table->getRowCount() - 1, 0 );
					table->setText( crb->getName()->c_str(), table->getRowCount() - 1, 1 );
					table->setText( TEXT( "Recyble Bin" ), table->getRowCount() - 1, 2 );
					table->setText( uc->getUserName()->c_str(), table->getRowCount() - 1, 3 );
					existsing.push_back( crb->getName()->c_str() );
				}
			}
			else if( CloudRecycleBinObject* crbo = dynamic_cast<CloudRecycleBinObject*>( bf ) ) {
				bool exists = false;
				for ( auto j = 0; j < (int)table->getRowCount(); j++ ) {
					if ( _tcstoui64( table->getText( j, 0 )->c_str(), nullptr, 10 ) == crbo->getId() ) {
						table->setText( to_tstring( crbo->getId() ).c_str(), j, 1 );
						table->setText( crbo->getName()->c_str(), j, 2 );
						table->setText( crbo->getType()->c_str(), j, 3 );
						table->setText( uc->getUserName()->c_str(), j, 4 );
						existsing.push_back( crbo->getName()->c_str() );
						bool exists = true;
					}
				}
				if ( !exists ) {
					table->addRow();
					table->setText( to_tstring( crbo->getId() ).c_str(), table->getRowCount() - 1, 0 );
					table->setText( crbo->getName()->c_str(), table->getRowCount() - 1, 1 );
					table->setText( crbo->getType()->c_str(), table->getRowCount() - 1, 2 );
					table->setText( uc->getUserName()->c_str(), table->getRowCount() - 1, 3 );
					existsing.push_back( crbo->getName()->c_str() );
				}
			}
		}
		for ( size_t i = 0; i < table->getRowCount(); ) {
			exists = false;
			for ( auto j = existsing.cbegin(); j != existsing.end(); ++j ) {
				if ( table->getText( (int)i, 1 )->compare( j->c_str() ) == 0 ) {
					exists = true;
					break;
				}
			}
			if ( !exists ) ( ( maschinen::list_view::Table* )lv.get() )->deleteRow( ( int ) i );
			else ++i;
		}
	}
	else if ( maschinen::list_view::ListView* list_view = dynamic_cast< maschinen::list_view::ListView* >( lv.get() ) ) {
	}
	flag_updating_list_view = false;
	setFileControlLoadEnable();
}

void FileControl::restartRequest(){if(!cloud->isStopped()) cloud->restart();}
void FileControl::stopRefresher(){cloud->cancel(true);}

CloudFileOperator* FileControl::getCloud() const noexcept { return cloud.get(); }
void FileControl::resetBuffer( std::list<BaseFile*> buffer ) {
	cleanBuffer();
	//file_buffer.swap( buffer );
	refreshListView();
}
void FileControl::addFileToBuffer( BaseFile* file ) {
	if ( file ) file_buffer.push_back( std::unique_ptr<BaseFile>( file ) );
}

void FileControl::cleanBuffer() {
	file_buffer.clear();
}

void FileControl::deleteProperty( Property* window_property ) {
	for ( auto i = properties.begin(); i != properties.end(); ++i ) {
		if( (*i )->getId() == window_property->getId() ) {
			( *i )->destroy();
			properties.erase( std::remove( properties.begin(), properties.end(), *i ), properties.end() );
			break;
		}
	}
}

void FileControl::setStatusBar( maschinen::StatusBar* status ) {this->status = status;}
std::list<std::unique_ptr<BaseFile>>::const_iterator FileControl::begin(){return file_buffer.cbegin();}
std::list<std::unique_ptr<BaseFile>>::const_iterator FileControl::end(){return file_buffer.cend();}
BaseFile* FileControl::getPointer() {return pointer.get();}
void FileControl::setPointer( BaseFile* pointer ){
	previous_ptr.reset( this->pointer.release() );
	this->pointer.reset( pointer );
}
bool FileControl::isListViewUpdating() const noexcept {return flag_updating_list_view;}

void FileControl::setFlagSearchCloudFinish( bool flag_search_cloud_finish ) {
	this->flag_search_cloud_finish = flag_search_cloud_finish;
}
void FileControl::setFlagSearchLocalFinish( bool flag_search_local_finish ){
	this->flag_search_local_finish = flag_search_local_finish;
}
void FileControl::endSearch() {
	if( flag_search_cloud_finish && flag_search_local_finish ) {
		pb.styleMarquee( false );
		pb.hide();
	}
}
FileControl::~FileControl()
{
}
