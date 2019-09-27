#include "stdafx.h"
#include "CloudFileRenamer.h"


CloudFileRenamer::CloudFileRenamer( QWORD id, LPCTSTR new_name ) {
	this->id = id;
	this->new_name = new_name;
}

LPVOID CloudFileRenamer::doInBackground( LPVOID input ){
	maschinen::wininet::HttpParams params;
	params.add( CloudFileRenamer::ID, to_tstring( id ), params.METHOD_POST );
	params.add( CloudFileRenamer::NEW_NAME, new_name, params.METHOD_POST );
	ATRequestAndParamCustomInternetJson atr;
	atr.params = &params;
	atr.request = &internet::value::request::RENAME_FILE;
	return CustomInternetJSONAsyncTask::doInBackground( &atr );
}
void CloudFileRenamer::onMainPostExecute( Poco::JSON::Object::Ptr& json ){
	if ( json->has( VAR_JSON_MESSAGE ) ) {
		TCHAR buffer[ MAX_LOADSTRING ];
		size_t size = strlen( json->get( VAR_JSON_MESSAGE ).extract<std::string>().c_str() );
		mbstowcs( buffer, json->get( VAR_JSON_MESSAGE ).extract<std::string>().c_str(), MAX_LOADSTRING );
		MessageBox( parent->getParent()->getHandler(), buffer, error_marker, MB_ICONERROR | MB_OK );
	}
	( ( FileControl* ) parent )->restartRequest();
}


CloudFileRenamer::~CloudFileRenamer() {
}
