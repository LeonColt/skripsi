#include "stdafx.h"
#include "MetaDataDownloadDirectory.h"


MetaDataDownloadDirectory::MetaDataDownloadDirectory() {
}

void MetaDataDownloadDirectory::addChildren( MetaDataDownloadBaseFile* meta_data ) {
	if ( meta_data != nullptr ) children.push_back( meta_data );
}
std::vector<MetaDataDownloadBaseFile*>::const_iterator MetaDataDownloadDirectory::begin() const noexcept {
	return children.cbegin();
}
std::vector<MetaDataDownloadBaseFile*>::const_iterator MetaDataDownloadDirectory::end() const noexcept {
	return children.cend();
}

std::unique_ptr<tstring> MetaDataDownloadDirectory::getPath() noexcept {
	std::vector<tstring> paths;
	MetaDataDownloadBaseFile* bf = this;
	while ( bf != nullptr ) {
		paths.push_back( bf->getName()->c_str() );
		bf = bf->getParent();
	}
	std::unique_ptr<tstring> res = std::make_unique<tstring>( TEXT( "" ) );
	for ( auto i = paths.crbegin(); i != paths.crend(); i++ ) {
		res->append( i->c_str() );
		if ( i != paths.crend() - 1 && paths.size() != 1 ) res->append( TEXT( "\\" ) );
	}
	return res;
}

bool MetaDataDownloadDirectory::empty() const noexcept {return children.empty();}
void MetaDataDownloadDirectory::clear() noexcept {
	if ( children.empty() ) return;
	for ( auto i = children.begin(); i != children.end(); i++ ) {
		if ( MetaDataDownloadDirectory* dir = dynamic_cast< MetaDataDownloadDirectory* >( *i ) ) dir->clear();
		delete *i;
	}
	children.clear();
}


MetaDataDownloadDirectory::~MetaDataDownloadDirectory() {
	if ( !children.empty() ) clear();
}
