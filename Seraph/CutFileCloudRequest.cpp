#include "stdafx.h"
#include "CutFileCloudRequest.h"


CutFileCloudRequest::CutFileCloudRequest(){}

LPVOID CutFileCloudRequest::doInBackground( LPVOID input ) {
	maschinen::wininet::HttpParams params;
	tstring paths = TEXT( "" );
	for ( auto i = sources.cbegin(); i != sources.cend(); ++i ) {
		if ( i != sources.cbegin() ) paths.append( TEXT( "|" ) );
		paths.append( i->c_str() );
	}
	params.add( FROMS, paths, params.METHOD_POST );
	params.add( TO, target.c_str(), params.METHOD_POST );
	params.add( LOGIN_ID, uc->getLoginId()->c_str(), params.METHOD_POST );
	ATRequestAndParamCustomInternetJson temp;
	temp.request = &internet::value::request::MOVE_FILE;
	temp.params = &params;
	return CustomInternetJSONAsyncTask::doInBackground( &temp );
}
void CutFileCloudRequest::onMainPostExecute( Poco::JSON::Object::Ptr& json ){
	if ( json->has( VAR_JSON_MESSAGE ) ) {
		TCHAR buffer[ MAX_LOADSTRING ];
		size_t size = strlen( json->get( VAR_JSON_MESSAGE ).extract<std::string>().c_str() );
		mbstowcs( buffer, json->get( VAR_JSON_MESSAGE ).extract<std::string>().c_str(), MAX_LOADSTRING );
		MessageBox( parent->getParent()->getHandler(), buffer, error_marker, MB_ICONERROR | MB_OK );
	}
	( ( FileControl* ) parent )->restartRequest();
}

CutFileCloudRequest::~CutFileCloudRequest() {
}

void CutFileCloudRequest::setTarget( LPCTSTR target ) noexcept { this->target = target; }
void CutFileCloudRequest::setTarget( tstring& target ) noexcept { this->target = target.c_str(); }
void CutFileCloudRequest::setTarget( tstring* target ) noexcept { this->target = target->c_str(); }
void CutFileCloudRequest::addSources( LPCTSTR cloud ) noexcept { sources.push_back( cloud ); }
void CutFileCloudRequest::addSources( tstring& cloud ) noexcept { sources.push_back( cloud.c_str() ); }
void CutFileCloudRequest::addSources( tstring* cloud ) noexcept { sources.push_back( cloud->c_str() ); }
