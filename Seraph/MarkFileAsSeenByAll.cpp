#include "stdafx.h"
#include "MarkFileAsPrivilegeByAll.h"


MarkFileAsPrivilegeByAll::MarkFileAsPrivilegeByAll() : SEEN(1), DOWNLOAD(2), MODIFY(3) {
}

LPVOID MarkFileAsPrivilegeByAll::doInBackground( LPVOID input ){
	maschinen::wininet::HttpParams params;
	getDefaultHttpParam( &params, &uc->getPhpSessionId() );
	params.add( ID, to_tstring( cloud->getId() ).c_str(), params.METHOD_POST );
	if ( type == SEEN ) {
		params.add( FLAG, to_tstring( !cloud->isSeenByAll() ).c_str(), params.METHOD_POST );
		params.add( TYPE, TYPE_SEEN.c_str(), params.METHOD_POST );
	}
	else if ( type == DOWNLOAD ){
		params.add( FLAG, to_tstring( !cloud->isDownloadByAll() ).c_str(), params.METHOD_POST );
		params.add( TYPE, TYPE_DOWNLOAD.c_str(), params.METHOD_POST );
	}
	else if ( type == MODIFY ){
		params.add( FLAG, to_tstring( !cloud->isModifyByAll() ).c_str(), params.METHOD_POST );
		params.add( TYPE, TYPE_MODIFY, params.METHOD_POST );
	}
	if(uc != nullptr ) params.add( LOGIN_ID, uc->getLoginId()->c_str(), params.METHOD_POST );
	ATRequestAndParamCustomInternetJson temp;
	temp.request = &internet::value::request::MARK_FILES_AS_PRIVILEGE_BY_ALL;
	temp.params = &params;
	return CustomInternetJSONAsyncTask::doInBackground( &temp );
}

void MarkFileAsPrivilegeByAll::onMainPostExecute( Poco::JSON::Object::Ptr& json ) {
	if ( type == SEEN ) ( ( CloudPropertyAbout* ) parent )->setSeenByAllEnableDisable( true );
	else if ( type == DOWNLOAD ) ( ( CloudPropertyAbout* ) parent )->setDownloadByAllEnableDisable( true );
	else if ( type == MODIFY ) ( ( CloudPropertyAbout* ) parent )->setModifyByAllEnableDisable( true );
}

void MarkFileAsPrivilegeByAll::onJsonStatusFail( Poco::JSON::Object::Ptr& json ) {
	if ( json->has( VAR_JSON_MESSAGE ) ) {
		if ( type == SEEN ) ( ( CloudPropertyAbout* ) parent )->setSeenByAllCheckUncheck( cloud->isSeenByAll() );
		else if ( type == DOWNLOAD ) ( ( CloudPropertyAbout* ) parent )->setDownloadByAllCheckUncheck( cloud->isDownloadByAll() );
		else if ( type == MODIFY ) ( ( CloudPropertyAbout* ) parent )->setModifyByAllCheckUncheck( cloud->isModifyByAll() );
		MessageBox( parent->getWindowHandler(), strToWstr( json->get( VAR_JSON_MESSAGE ).extract<std::string>() )->c_str(), error_marker, MB_OK | MB_ICONERROR );
	}
	if ( type == SEEN ) ( ( CloudPropertyAbout* ) parent )->setSeenByAllEnableDisable( true );
	else if ( type == DOWNLOAD ) ( ( CloudPropertyAbout* ) parent )->setDownloadByAllEnableDisable( true );
	else if ( type == MODIFY ) ( ( CloudPropertyAbout* ) parent )->setModifyByAllEnableDisable( true );
}

void MarkFileAsPrivilegeByAll::setCloud( Cloud* cloud ) {
	this->cloud = cloud;
}
void MarkFileAsPrivilegeByAll::setTypeSeen(){
	type = SEEN;
}
void MarkFileAsPrivilegeByAll::setTypeDownload(){
	type = DOWNLOAD;
}
void MarkFileAsPrivilegeByAll::setTypeModify() {
	type = MODIFY;
}

MarkFileAsPrivilegeByAll::~MarkFileAsPrivilegeByAll() {
}
