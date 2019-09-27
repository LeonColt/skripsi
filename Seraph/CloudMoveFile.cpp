#include "stdafx.h"
#include "CloudMoveFile.h"


CloudMoveFile::CloudMoveFile() {
}

LPVOID CloudMoveFile::doInBackground( LPVOID input ) {
	maschinen::wininet::HttpParams params;
	getDefaultHttpParam( &params, &uc->getPhpSessionId() );
	params.add( TO, to_tstring( to ), params.METHOD_POST );
	tstring sfroms( TEXT( "" ) );
	for ( auto i = froms.begin(); i != froms.end(); i++ ) {
		if ( i != froms.begin() ) sfroms.append( TEXT( "," ) );
		sfroms.append( to_tstring( *i ).c_str() );
	}
	params.add( FROMS, sfroms, params.METHOD_POST );
	ATRequestAndParamCustomInternetJson temp;
	temp.request = &internet::value::request::MOVE_FILE;
	temp.params = &params;
	return CustomInternetJSONAsyncTask::doInBackground( (LPVOID) &temp );
}
void CloudMoveFile::onMainPostExecute( Poco::JSON::Object::Ptr& json ) {
	if ( !json->has( VAR_JSON_STATUS ) ) {
		if ( json->has( VAR_JSON_MESSAGE ) ) {
			TCHAR buffer[ MAX_LOADSTRING ];
			size_t size = strlen( json->get( VAR_JSON_MESSAGE ).extract<std::string>().c_str() );
			mbstowcs( buffer, json->get( VAR_JSON_MESSAGE ).extract<std::string>().c_str(), MAX_LOADSTRING );
			MessageBox( parent->getParent()->getHandler(), buffer, error_marker, MB_ICONERROR | MB_OK );
		}
		else MessageBox( parent->getParent()->getHandler(), L"Unknow error occurred", error_marker, MB_ICONERROR | MB_OK );
	}
	if ( json->get( VAR_JSON_STATUS ).extract<int>() == 1 ) {
		if ( json->has( VAR_JSON_MESSAGE ) ) {
			TCHAR buffer[ MAX_LOADSTRING ];
			size_t size = strlen( json->get( VAR_JSON_MESSAGE ).extract<std::string>().c_str() );
			mbstowcs( buffer, json->get( VAR_JSON_MESSAGE ).extract<std::string>().c_str(), MAX_LOADSTRING );
			MessageBox( parent->getParent()->getHandler(), buffer, error_marker, MB_ICONERROR | MB_OK );
		}
		//( ( FileControl* ) parent )->deleteCloudTreeFile( ids );
	}
	else {
		if ( json->has( VAR_JSON_MESSAGE ) ) {
			TCHAR buffer[ MAX_LOADSTRING ];
			size_t size = strlen( json->get( VAR_JSON_MESSAGE ).extract<std::string>().c_str() );
			mbstowcs( buffer, json->get( VAR_JSON_MESSAGE ).extract<std::string>().c_str(), MAX_LOADSTRING );
			MessageBox( parent->getParent()->getHandler(), buffer, error_marker, MB_ICONERROR | MB_OK );
		}
		else MessageBox( parent->getParent()->getHandler(), L"Unknow error occurred", error_marker, MB_ICONERROR | MB_OK );
	}
}


CloudMoveFile::~CloudMoveFile() {
}

void CloudMoveFile::setFroms( std::vector<QWORD> froms ) noexcept {
	this->froms = froms;
}
void CloudMoveFile::setTo( QWORD to ) noexcept {
	this->to = to;
}