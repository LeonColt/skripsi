#include "stdafx.h"
#include "MetaDataDownloadFile.h"


MetaDataDownloadFile::MetaDataDownloadFile() {
}

std::unique_ptr<tstring> MetaDataDownloadFile::getPath() noexcept {
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

MetaDataDownloadFile::~MetaDataDownloadFile() {
}
