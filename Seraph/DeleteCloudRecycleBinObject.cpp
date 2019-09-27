#include "stdafx.h"
#include "DeleteCloudRecycleBinObject.h"


DeleteCloudRecycleBinObject::DeleteCloudRecycleBinObject() {
}

LPVOID DeleteCloudRecycleBinObject::doInBackground( LPVOID input ){
	maschinen::wininet::HttpParams params;
	tstring paths( TEXT( "" ) );
	for(auto i = sources.cbegin(); i != sources.cend(); ++i ) {
		if ( i != sources.cbegin() ) paths.append( TEXT( "|" ) );
		paths.append( i->c_str() );
	}
	params.add( PATHS, paths, params.METHOD_POST );
	params.add( LOGIN_ID, uc->getLoginId()->c_str(), params.METHOD_POST );
	ATRequestAndParamCustomInternetJson atp;
	atp.request = &internet::value::request::DELETE_DELETED_FILE;
	atp.params = &params;
	return CustomInternetJSONAsyncTask::doInBackground( &atp );
}
void DeleteCloudRecycleBinObject::onMainPostExecute( Poco::JSON::Object::Ptr& json ) {
	if ( json->has( VAR_JSON_MESSAGE ) )
		MessageBox( parent->getWindowHandler(), strToWstr( json->get( VAR_JSON_MESSAGE ).extract<std::string>() )->c_str(), error_marker, MB_ICONINFORMATION | MB_OK );
}

DeleteCloudRecycleBinObject::~DeleteCloudRecycleBinObject() {
}

void DeleteCloudRecycleBinObject::setSources( std::vector<tstring>& sources ) {
	this->sources.assign( sources.begin(), sources.end() );
}
