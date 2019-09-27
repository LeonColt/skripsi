#include "stdafx.h"
#include "LocalChangeDetector.h"


LocalChangeDetector::LocalChangeDetector() {
}
void LocalChangeDetector::onPreExecute() {
	flag_stop = false;
	flag_restart = false;
}
LPVOID LocalChangeDetector::doInBackground( LPVOID input ) {
	for ( ;; ) {
		if ( flag_stop ) break;
		if ( search_file_name.compare( TEXT( "" ) ) != 0 ) {
			std::list<Local*> temp;
			if ( Local* l = dynamic_cast< Local* >( ( ( FileControl* ) parent )->getPointer() ) )
				search( search_file_name.c_str(), temp, l );
			else {
				DriveLoader loader;
				for ( auto i = loader.begin(); i != loader.end(); ++i) {
					search( search_file_name.c_str(), temp, *i );
					delete *i;
				}
			}
			for ( auto i = temp.cbegin(); i != temp.cend() && !flag_stop; ++i )
				( ( FileControl* ) parent )->addFileToBuffer( *i );
			if ( flag_stop ) return nullptr;
			( ( FileControl* ) parent )->refreshListView();
			if ( flag_stop ) return nullptr;
			search_file_name.clear();
			( ( FileControl* ) parent )->setFlagSearchLocalFinish( true );
			( ( FileControl* ) parent )->endSearch();
		}
		else if ( ( ( FileControl* ) parent )->getPointer() == nullptr ) {
			watcher[ 1 ] = CreateEventW( NULL, TRUE, FALSE, NULL );
			if ( watcher[ 1 ] == NULL ) continue;
			DWORD wait_status = WaitForSingleObject( watcher[ 1 ], INFINITE );
			CloseHandle( watcher[ 1 ] );
			watcher[ 1 ] = NULL;
		}
		else if ( Local* l = dynamic_cast< Local* >( ( ( FileControl* ) parent )->getPointer() ) ) {
			tstring path( l->getPath()->c_str() );
			path.append( TEXT( "\\" ) );
			watcher[ 0 ] = FindFirstChangeNotification( path.c_str(), TRUE,
														FILE_NOTIFY_CHANGE_FILE_NAME |
														FILE_NOTIFY_CHANGE_DIR_NAME );
			if ( watcher[ 0 ] == INVALID_HANDLE_VALUE ) continue;
			watcher[ 1 ] = CreateEventW( NULL, TRUE, FALSE, NULL );
			if ( watcher[ 1 ] == NULL ) {
				FindCloseChangeNotification( watcher[ 0 ] );
				continue;
			}
			flag_restart = false;
			for ( ; !flag_restart; ) {
				DWORD wait_status = WaitForMultipleObjects( 2, watcher, FALSE, INFINITE );
				switch ( wait_status ) {
					case WAIT_OBJECT_0:
					{
						if ( Drive* d = dynamic_cast< Drive* >( l ) ) {
							while ( ( ( FileControl* ) parent )->isListViewUpdating() ) std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
							( ( FileControl* ) parent )->updateListView();
						}
						else if ( DirectoryLocal* dl = dynamic_cast< DirectoryLocal* >( l ) ) {
							while ( ( ( FileControl* ) parent )->isListViewUpdating() ) std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
							( ( FileControl* ) parent )->updateListView();
						}
						FindNextChangeNotification( watcher[ 0 ] );
					} break;

					case WAIT_OBJECT_0 + 1:
					{
						FindCloseChangeNotification( watcher[ 0 ] );
						CloseHandle( watcher[ 1 ] );
						watcher[ 0 ] = watcher[ 1 ] = NULL;
					} break;

					default:
					{
						flag_restart = true;
					} break;
				}
			}
		}
		else {
			watcher[ 1 ] = CreateEventW( NULL, TRUE, FALSE, NULL );
			if ( watcher[ 1 ] == NULL ) continue;
			DWORD wait_status = WaitForSingleObject( watcher[ 1 ], INFINITE );
			CloseHandle( watcher[ 1 ] );
			watcher[ 1 ] = NULL;
		}
	}
	return nullptr;
}

void LocalChangeDetector::onPostExecute( LPVOID result ) {}
void LocalChangeDetector::stop() {
	flag_stop = true;
	if ( watcher[ 1 ] != NULL ) SetEvent( watcher[ 1 ] );
}

bool LocalChangeDetector::isStopped() const noexcept {
	return flag_stop;
}

void LocalChangeDetector::restart() {
	if ( watcher[ 1 ] != NULL ) SetEvent( watcher[ 1 ] );
	flag_restart = true;
}

void LocalChangeDetector::search( LPCTSTR name, std::list<Local*>& buffer, Local* start_position ) {
	if ( flag_stop ) return;
	if ( tstrToLower( start_position->getName()->c_str() )->find( tstrToLower( name )->c_str() ) != tstring::npos ) {
		if ( DirectoryLocal* dir = dynamic_cast<DirectoryLocal*>( start_position ) ) buffer.push_back( start_position );
		else if ( FileLocal* file = dynamic_cast<FileLocal*>( start_position ) ) buffer.push_back( start_position );
	}
	if ( DriveFixed* df = dynamic_cast< DriveFixed* >( start_position ) ) {
		try {
			for ( auto i = df->begin(); i != df->end() && !flag_stop; ++i ) {
				if ( DirectoryLocal* dir = dynamic_cast< DirectoryLocal* >( *i ) )
					search( name, buffer, *i );
				else if ( FileLocal* file = dynamic_cast< FileLocal* >( *i ) ) {
					if ( tstrToLower( file->getName()->c_str() )->find( tstrToLower( name )->c_str() ) != tstring::npos )
						buffer.push_back( *i );
				}
			}
		} catch( SeraphException& e ) {
			SeraphException se;
			se.write( string::file::name::ERROR_LOG_NAME.c_str(), e );
		} catch(maschinen::exception::Exception& e ) {
			SeraphException se;
			se.write( string::file::name::ERROR_LOG_NAME.c_str(), e );
		} catch( std::exception& e ) {
			SeraphException se;
			se.write( string::file::name::ERROR_LOG_NAME.c_str(), e );
		}
	}
	else if ( DriveRemovable* dr = dynamic_cast< DriveRemovable* > ( start_position ) ) {
		try {
			for ( auto i = dr->begin(); i != dr->end() && !flag_stop; ++i ) {
				if ( DirectoryLocal* dir = dynamic_cast< DirectoryLocal* >( *i ) )
					search( name, buffer, *i );
				else if ( FileLocal* file = dynamic_cast< FileLocal* >( *i ) ) {
					if ( tstrToLower( file->getName()->c_str() )->find( tstrToLower( name )->c_str() ) != tstring::npos )
						buffer.push_back( *i );
				}
			}
		} catch ( SeraphException& e ) {
			SeraphException se;
			se.write( string::file::name::ERROR_LOG_NAME.c_str(), e );
		} catch ( maschinen::exception::Exception& e ) {
			SeraphException se;
			se.write( string::file::name::ERROR_LOG_NAME.c_str(), e );
		} catch ( std::exception& e ) {
			SeraphException se;
			se.write( string::file::name::ERROR_LOG_NAME.c_str(), e );
		}
	}
	else if ( DirectoryLocal* dl = dynamic_cast< DirectoryLocal* >( start_position ) ) {
		try {
			for ( auto i = dl->begin(); i != dl->end() && !flag_stop; ++i ) {
				if ( DirectoryLocal* dir = dynamic_cast< DirectoryLocal* >( *i ) )
					search( name, buffer, *i );
				else if ( FileLocal* file = dynamic_cast< FileLocal* >( *i ) ) {
					if ( tstrToLower( file->getName()->c_str() )->find( tstrToLower( name )->c_str() ) != tstring::npos )
						buffer.push_back( *i );
				}
			}
		} catch ( SeraphException& e ) {
		} catch ( maschinen::exception::Exception& e ) {
			SeraphException se;
			se.write( string::file::name::ERROR_LOG_NAME.c_str(), e );
		} catch ( std::exception& e ) {
			SeraphException se;
			se.write( string::file::name::ERROR_LOG_NAME.c_str(), e );
		}
	}
}

void LocalChangeDetector::setSearchName( LPCTSTR search_file_name ) {
	this->search_file_name.assign( search_file_name );
}
void LocalChangeDetector::setSearchName( tstring& search_file_name ) {
	this->search_file_name.assign( search_file_name );
}
void LocalChangeDetector::setSearchName( tstring* search_file_name ) {
	this->search_file_name.assign( search_file_name->c_str() );
}

LocalChangeDetector::~LocalChangeDetector() {
}
