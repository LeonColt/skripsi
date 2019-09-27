#include "stdafx.h"
#include "UploadedFileTotalSizeAT.h"


UploadedFileTotalSizeAT::UploadedFileTotalSizeAT() {
}

void UploadedFileTotalSizeAT::onPreExecute() {

}

void UploadedFileTotalSizeAT::onMainPostExecute( Poco::JSON::Object::Ptr& json ) {
	if ( json->has( VAR_JSON_STATUS ) ) {
		if ( json->get( VAR_JSON_STATUS ).extract<int>() == 1 ) {
			if ( json->has( VAR_JSON_RESULT ) ) {
				( ( FileUploadProcessWindow* ) parent )->setTotalUploaded( _strtoui64( json->get( VAR_JSON_RESULT ).extract<std::string>().c_str(), nullptr, 10 ) );
				return;
			}
			else {
				if ( json->has( VAR_JSON_MESSAGE ) )
					MessageBox( parent->getHandler(), strToWstr( json->get( VAR_JSON_MESSAGE ).extract<std::string>() )->c_str(), error_marker, MB_OK | MB_ICONERROR );
				else MessageBox( parent->getHandler(), L"An unknown error occured", error_marker, MB_OK | MB_ICONERROR );
				( ( FileUploadProcessWindow* ) parent )->hide();
			}
		}
		else {
			if ( json->has( VAR_JSON_MESSAGE ) )
				MessageBox( parent->getHandler(), strToWstr( json->get(VAR_JSON_MESSAGE).extract<std::string>() )->c_str(), error_marker, MB_OK | MB_ICONERROR );
			else MessageBox( parent->getHandler(), L"An unknown error occured", error_marker, MB_OK | MB_ICONERROR );
			( ( FileUploadProcessWindow* ) parent )->hide();
		}
	}
	else {
		MessageBox( parent->getHandler(), L"Invalid Json", error_marker, MB_OK | MB_ICONERROR );
		( ( FileUploadProcessWindow* ) parent )->hide();
	}
}

void UploadedFileTotalSizeAT::onJsonStatusFail( Poco::JSON::Object::Ptr& json ) {
	if ( json->has( VAR_JSON_MESSAGE ) )
		MessageBox( parent->getHandler(), strToWstr( json->get( VAR_JSON_MESSAGE ).extract<std::string>() )->c_str(), error_marker, MB_OK | MB_ICONERROR );
	else MessageBox( parent->getHandler(), L"An unknown error occured", error_marker, MB_OK | MB_ICONERROR );
	( ( FileUploadProcessWindow* ) parent )->hide();
}

LPVOID UploadedFileTotalSizeAT::doInBackground( LPVOID input ) {
	maschinen::wininet::HttpParams params;
	getDefaultHttpParam( params, &uc->getPhpSessionId() );
	params.add( ID, id, params.METHOD_POST );
	if ( uc->isCredential() )
		params.add( LOGIN_ID, uc->getLoginId()->c_str(), params.METHOD_POST );
	ATRequestAndParamCustomInternetJson art;
	art.request = &internet::value::request::upload::GET_TOTAL_SIZE;
	art.params = &params;
	return CustomInternetJSONAsyncTask::doInBackground( &art );
}

void UploadedFileTotalSizeAT::setUploadId( LPCTSTR id ) noexcept {
	this->id.assign( id );
}

UploadedFileTotalSizeAT::~UploadedFileTotalSizeAT() {
}
