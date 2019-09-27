#include "stdafx.h"
#include "DownloadRequest.h"


DownloadRequest::DownloadRequest(){
}

void DownloadRequest::onPreExecute()
{
	/*
	BROWSEINFO bi = { 0 };
	bi.lpszTitle = L"Browse For Target Save Location";
	bi.hwndOwner = parent->getParent()->getHandler();

	LPITEMIDLIST lpitemlist = SHBrowseForFolder(&bi);

	if (lpitemlist != NULL)
	{
		TCHAR temp[MAX_PATH] = L"\0";
		if (SHGetPathFromIDList(lpitemlist, temp) == TRUE)
			wcscpy_s(target_file_save, temp);
		else cancel(false);
		CoTaskMemFree(lpitemlist);
	}
	else cancel(false);
	*/
}

LPVOID DownloadRequest::doInBackground(LPVOID input){
	maschinen::wininet::HttpParams params;
	getDefaultHttpParam( &params, &uc->getPhpSessionId() );
	params.add( LOGIN_ID, uc->getLoginId()->c_str(), params.METHOD_POST );
	tstring paths = TEXT( "" );
	for ( auto i = sources.begin(); i != sources.end(); i++ ) {
		if ( i != sources.begin() ) paths.append( TEXT( "|" ) );
		paths.append( i->c_str() );
	}
	params.add( PATHS, paths.c_str(), params.METHOD_POST );
	params.add( AS_ZIP, L"false", params.METHOD_POST );
	ATRequestAndParamCustomInternetJson temp;
	temp.request = &internet::value::request::download::BEGIN;
	temp.params = &params;
	return CustomInternetJSONAsyncTask::doInBackground( &temp );
}

void DownloadRequest::onPrePostExecute(){}

void DownloadRequest::onMainPostExecute(Poco::JSON::Object::Ptr& json)
{
	try {
		Poco::JSON::Object::Ptr json_result = json->get( VAR_JSON_RESULT ).extract<Poco::JSON::Object::Ptr>();
		MetaDataDownload meta_data;
		meta_data.setId( strToWstr( json_result->get( KEMBALIAN_ID.c_str() ).extract<std::string>().c_str() )->c_str() );
		Poco::JSON::Array::Ptr json_array = json_result->get( KEMBALIAN_FILES.c_str() ).extract<Poco::JSON::Array::Ptr>();
		for ( auto i = json_array->begin(); i != json_array->end(); i++ ) {
			Poco::JSON::Object::Ptr json_file = i->extract<Poco::JSON::Object::Ptr>();
			meta_data.addFile(
				_strtoui64( json_file->get( KEMBALIAN_FILES_ID ).extract<std::string>().c_str(), nullptr, 10 ),
				strToWstr( json_file->get( KEMBALIAN_FILES_NAME ).extract<std::string>().c_str() )->c_str(),
				strtoull( json_file->get( KEMBALIAN_FILES_SIZE ).extract<std::string>().c_str(), nullptr, 10 ),
				json_file->get(KEMBALIAN_FILES_DIRECTORY).extract<bool>(),
				_strtoui64( json_file->get( KEMBALIAN_FILES_PARENT ).extract<std::string>().c_str(), nullptr, 10 ),
				json_file->get( KEMBALIAN_FILES_ORIGINAL ).extract<bool>()
			);
		}
		meta_data.setTargetPath( target.c_str() );
		meta_data.write();
		if ( setting->getShowWindowDownloadAfterCreateNewDownload() ) 
			( ( MainWindow* ) parent->getParent() )->showWindowDownload();
	} catch ( SeraphException& e ) {
		MessageBox( parent->getParent()->getHandler(), e.getMessage()->c_str(), L"Error Download Request", MB_ICONERROR | MB_OK );
	} catch ( maschinen::exception::Exception& e ) {
		MessageBox( parent->getParent()->getHandler(), e.getMessage()->c_str(), L"Error Download Request", MB_ICONERROR | MB_OK );
	} catch ( std::exception& e ) {
		MessageBox( parent->getParent()->getHandler(), strToWstr( e.what() )->c_str(), L"Error Download Request", MB_ICONERROR | MB_OK );
	}
}

void DownloadRequest::onPostPostExecute() { sources.clear(); }
void DownloadRequest::setTarget( LPCTSTR target ) noexcept  {this->target = target;}
void DownloadRequest::setTarget( tstring& target ) noexcept {this->target = target.c_str();}
void DownloadRequest::setTarget( tstring* target ) noexcept {this->target = target->c_str();}
void DownloadRequest::addSources( LPCTSTR cloud ) noexcept {sources.push_back( cloud );}
void DownloadRequest::addSources( tstring& cloud ) noexcept { sources.push_back( cloud.c_str() ); }
void DownloadRequest::addSources( tstring* cloud ) noexcept { sources.push_back( cloud->c_str() ); }
DownloadRequest::~DownloadRequest(){}
