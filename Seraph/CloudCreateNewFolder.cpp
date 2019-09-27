#include "stdafx.h"
#include "CloudCreateNewFolder.h"


CloudCreateNewFolder::CloudCreateNewFolder() {
}

LPVOID CloudCreateNewFolder::doInBackground( LPVOID input ) {
	maschinen::wininet::HttpParams params;
	getDefaultHttpParam( &params, &uc->getPhpSessionId() );
	params.add( PARENT, to_tstring( parent_id ).c_str(), params.METHOD_POST );
	params.add( LOGIN_ID, uc->getLoginId()->c_str(), params.METHOD_POST );
	ATRequestAndParamCustomInternetJson temp;
	temp.request = &internet::value::request::CREATE_NEW_FOLDER;
	temp.params = &params;
	return CustomInternetJSONAsyncTask::doInBackground( ( LPVOID ) &temp );
}

void CloudCreateNewFolder::onMainPostExecute( Poco::JSON::Object::Ptr& json ) {
	if ( json->has( VAR_JSON_MESSAGE ) ) {
		TCHAR buffer[ MAX_LOADSTRING ];
		size_t size = strlen( json->get( VAR_JSON_MESSAGE ).extract<std::string>().c_str() );
		mbstowcs( buffer, json->get( VAR_JSON_MESSAGE ).extract<std::string>().c_str(), MAX_LOADSTRING );
		MessageBox( parent->getParent()->getHandler(), buffer, error_marker, MB_ICONERROR | MB_OK );
	}
	( ( FileControl* ) parent )->restartRequest();
}

void CloudCreateNewFolder::setParentId( QWORD parent_id ) {
	this->parent_id = parent_id;
}


CloudCreateNewFolder::~CloudCreateNewFolder() {
}
