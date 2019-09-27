#include "stdafx.h"
#include "CloudRestoreFile.h"


CloudRestoreFile::CloudRestoreFile() {
}

LPVOID CloudRestoreFile::doInBackground( LPVOID input ) {
	maschinen::wininet::HttpParams params;
	getDefaultHttpParam( &params, &uc->getPhpSessionId() );
	tstring paths = L"";
	for ( auto i = sources.cbegin(); i != sources.cend(); i++ ) {
		if ( i == sources.cbegin() ) paths.append( i->c_str() );
		else {
			paths.append( TEXT( "|" ) );
			paths.append( i->c_str() );
		}
	}
	params.add( PATHS, paths, params.METHOD_POST );
	params.add( LOGIN_ID, uc->getLoginId()->c_str(), params.METHOD_POST );
	ATRequestAndParamCustomInternetJson temp;
	temp.request = &internet::value::request::RESTORE_FILES;
	temp.params = &params;
	return CustomInternetJSONAsyncTask::doInBackground( ( LPVOID ) &temp );
}

void CloudRestoreFile::setSources( std::vector<tstring>& sources ) {
	this->sources.assign( sources.begin(), sources.end() );
}

void CloudRestoreFile::onMainPostExecute( Poco::JSON::Object::Ptr& json ) {
	if ( json->has( VAR_JSON_MESSAGE ) ) {
		TCHAR buffer[ MAX_LOADSTRING ];
		size_t size = strlen( json->get( VAR_JSON_MESSAGE ).extract<std::string>().c_str() );
		mbstowcs( buffer, json->get( VAR_JSON_MESSAGE ).extract<std::string>().c_str(), MAX_LOADSTRING );
		MessageBox( parent->getParent()->getHandler(), buffer, error_marker, MB_ICONERROR | MB_OK );
	}
	( ( FileControl* ) parent )->restartRequest();
}

CloudRestoreFile::~CloudRestoreFile() {
}
