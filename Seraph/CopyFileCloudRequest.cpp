#include "stdafx.h"
#include "CopyFileCloudRequest.h"


CopyFileCloudRequest::CopyFileCloudRequest(){}

LPVOID CopyFileCloudRequest::doInBackground( LPVOID input ){
	maschinen::wininet::HttpParams params;
	tstring paths = TEXT( "" );
	for( auto i = sources.cbegin(); i != sources.cend(); ++i ) {
		if ( i != sources.cbegin() ) paths.append( TEXT( "|" ) );
		paths.append( i->c_str() );
	}
	params.add( SOURCES, paths, params.METHOD_POST );
	params.add( TARGET, target, params.METHOD_POST );
	params.add( LOGIN_ID, uc->getLoginId()->c_str(), params.METHOD_POST );
	ATRequestAndParamCustomInternetJson temp;
	temp.request = &internet::value::request::COPY_FILE_CLOUD;
	temp.params = &params;
	return CustomInternetJSONAsyncTask::doInBackground( &temp );
}

void CopyFileCloudRequest::onMainPostExecute( Poco::JSON::Object::Ptr& json ) {
	if ( json->has( VAR_JSON_MESSAGE ) ) {
		TCHAR buffer[ MAX_LOADSTRING ];
		size_t size = strlen( json->get( VAR_JSON_MESSAGE ).extract<std::string>().c_str() );
		mbstowcs( buffer, json->get( VAR_JSON_MESSAGE ).extract<std::string>().c_str(), MAX_LOADSTRING );
		MessageBox( parent->getParent()->getHandler(), buffer, error_marker, MB_ICONERROR | MB_OK );
	}
	( ( FileControl* ) parent )->restartRequest();
}

CopyFileCloudRequest::~CopyFileCloudRequest() {
}

void CopyFileCloudRequest::setTarget( LPCTSTR target ) noexcept { this->target = target; }
void CopyFileCloudRequest::setTarget( tstring& target ) noexcept { this->target = target.c_str(); }
void CopyFileCloudRequest::setTarget( tstring* target ) noexcept { this->target = target->c_str(); }
void CopyFileCloudRequest::addSources( LPCTSTR cloud ) noexcept { sources.push_back( cloud ); }
void CopyFileCloudRequest::addSources( tstring& cloud ) noexcept { sources.push_back( cloud.c_str() ); }
void CopyFileCloudRequest::addSources( tstring* cloud ) noexcept { sources.push_back( cloud->c_str() ); }
