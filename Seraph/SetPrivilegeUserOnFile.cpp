#include "stdafx.h"
#include "SetPrivilegeUserOnFile.h"


SetPrivilegeUserOnFile::SetPrivilegeUserOnFile() {
	cloud = nullptr;
}

LPVOID SetPrivilegeUserOnFile::doInBackground( LPVOID input ) {
	maschinen::wininet::HttpParams params;
	getDefaultHttpParam( &params, &uc->getPhpSessionId() );
	params.add( FILE_ID, to_tstring( cloud->getId() ), params.METHOD_POST );
	params.add( USER_ID_USERNAME_EMAIL, user_id_username_email, params.METHOD_POST );
	params.add( TYPE, type, params.METHOD_POST );
	params.add( LOGIN_ID, uc->getLoginId()->c_str(), params.METHOD_POST );
	ATRequestAndParamCustomInternetJson atp;
	atp.request = &internet::value::request::ADD_USER_LIST_ON_FILE_PRIVILEGE;
	atp.params = &params;
	return CustomInternetJSONAsyncTask::doInBackground( &atp );
}
void SetPrivilegeUserOnFile::onMainPostExecute( Poco::JSON::Object::Ptr& json ){
	if ( json->has( VAR_JSON_MESSAGE ) )
		MessageBox( parent->getHandler(), strToWstr( json->get( VAR_JSON_MESSAGE ).extract<std::string>() )->c_str(), TEXT( "Privilege" ), MB_ICONINFORMATION | MB_OK );
}

void SetPrivilegeUserOnFile::setCloud( Cloud* cloud ){
	this->cloud = cloud;
}
void SetPrivilegeUserOnFile::setUserIdUsernameEmail( LPCTSTR user_id_username_email ) {
	this->user_id_username_email.assign( user_id_username_email );
}

void SetPrivilegeUserOnFile::setTypeSeen(){
	this->type.assign( TYPE_SEEN.c_str() );
}
void SetPrivilegeUserOnFile::setTypeDownload(){
	this->type.assign( TYPE_DOWNLOAD.c_str() );
}
void SetPrivilegeUserOnFile::setTypeUploadOnDirectory(){
	this->type.assign( TYPE_UPLOAD_ON_DIRECTORY.c_str() );
}

SetPrivilegeUserOnFile::~SetPrivilegeUserOnFile() {
}
