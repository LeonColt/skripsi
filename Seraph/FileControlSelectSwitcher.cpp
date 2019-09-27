#include "stdafx.h"
#include "FileControlSelectSwitcher.h"


FileControlSelectSwitcher::FileControlSelectSwitcher( FileControl* fc ) {
	this->fc = fc;
}
void FileControlSelectSwitcher::retriveSelectedFile( maschinen::list_view::ListViewBasic* lv, maschinen::event::OnListViewSelectEvent& e ){
	selected.clear();
	if ( maschinen::list_view::Table* lvd = dynamic_cast< maschinen::list_view::Table* >( lv ) ) {
		for ( auto i = e.beginSelected(); i != e.endSelected(); i++ ) {
			if ( lvd->getText( *i, 0 )->empty() ) {
				tstring path = TEXT( "" );
				if ( fc->getPointer() != nullptr ) {
					if ( Local* local = dynamic_cast< Local* >( fc->getPointer() ) ) {
						if ( local->getPath()->compare( TEXT( "" ) ) != 0 ) {
							path.assign( ( fc->getPointer() )->getPath()->c_str() );
							path.append( TEXT( "\\" ) );
						}
					}
				}
				path.append( lvd->getText( *i, 1 )->c_str() );
				for( auto j = fc->begin(); j != fc->end(); ++j ) {
					if ( lvd->getText( *i, 1 )->compare( ( *j )->getName()->c_str() ) == 0 )
						selected.push_back( ( *j ).get() );
				}
			}
			else {
				for( auto j = fc->begin(); j != fc->end(); ++j ) {
					if( Cloud* c = dynamic_cast<Cloud*>( ( *j ).get() ) ) {
						if ( 
							lvd->getText( *i, 1 )->compare( ( *j )->getName()->c_str() ) == 0 //comparing name
							 && _tcstoui64( lvd->getText( *i, 0 )->c_str(), nullptr, 10 ) == c->getId() //comparing id
							)
							selected.push_back( c );
					}
					else if( CloudRecycleBin* crb = dynamic_cast<CloudRecycleBin*>( (*j ).get() ) ) {
						if (
							lvd->getText( *i, 1 )->compare( ( *j )->getName()->c_str() ) == 0 //comparing name
							)
							selected.push_back( crb );
					}
					else if( CloudRecycleBinObject* crbo = dynamic_cast<CloudRecycleBinObject*> ( (*j ).get() ) ) {
						if (
							lvd->getText( *i, 1 )->compare( ( *j )->getName()->c_str() ) == 0 //comparing name
							&& _tcstoui64( lvd->getText( *i, 0 )->c_str(), nullptr, 10 ) == crbo->getId() //comparing id
							)
							selected.push_back( crbo );
					}
				}
			}
		}
	}
}

void FileControlSelectSwitcher::addSelected( BaseFile* file ) {selected.push_back( file );}
bool FileControlSelectSwitcher::isSingleSelection() const noexcept {return selected.size() == 1;}
bool FileControlSelectSwitcher::isMultipleSelection() const noexcept {return selected.size() > 1;}
bool FileControlSelectSwitcher::isNoSelection() const noexcept { return selected.empty(); }
BaseFile* FileControlSelectSwitcher::get() const noexcept { if ( !isNoSelection() ) return selected.front(); else return nullptr; }
std::list<BaseFile*>::const_iterator FileControlSelectSwitcher::begin() const noexcept { return selected.cbegin(); }
std::list<BaseFile*>::const_iterator FileControlSelectSwitcher::end() const noexcept { return selected.cend(); }
FileControlSelectSwitcher::~FileControlSelectSwitcher() {
}
