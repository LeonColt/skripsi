#include "stdafx.h"
#include "UploadFileRequestAT.h"

UploadFileRequestAT::UploadFileRequestAT() {}

LPVOID UploadFileRequestAT::doInBackground( LPVOID input ) {
	maschinen::wininet::HttpParams params;
	getDefaultHttpParam( params, &uc->getPhpSessionId() );
	Poco::JSON::Object json;
	Poco::JSON::Array arr;
	UINT cid = 0;
	for ( auto i = sources.cbegin(); i != sources.cend(); ++i )
		transformFilesToJson( arr, i->c_str(), cid );
	json.set( INPUT_FILES.c_str(), arr );
	tstring files_json( serialization( &json )->c_str() );

	params.add( FILES, files_json.c_str(), params.METHOD_POST );
	OutputDebugStringW( files_json.c_str() );
	OutputDebugStringW( TEXT( "\n" ) );
	if ( uc->getBenutzerID() > 0 ) params.add( LOGIN_ID, uc->getLoginId()->c_str(), params.METHOD_POST );
	params.add( PARENT, target.c_str(), params.METHOD_POST );
	ATRequestAndParamCustomInternetJson art;
	art.request = &internet::value::request::upload::BEGIN;
	art.params = &params;
	return CustomInternetJSONAsyncTask::doInBackground( &art );
}
void UploadFileRequestAT::onPrePostExecute() {}

void UploadFileRequestAT::onMainPostExecute( Poco::JSON::Object::Ptr& json ) {
	if ( json->has( VAR_JSON_RESULT ) ) {
		MetaDataUpload meta_data;
		meta_data.setId( strToWstr( json->get( VAR_JSON_RESULT ).extract<Poco::JSON::Object::Ptr>()->get( RETURN_ID ).extract<std::string>().c_str() )->c_str() );
		Poco::JSON::Array::Ptr files = json->get( VAR_JSON_RESULT ).extract<Poco::JSON::Object::Ptr>()->get( RETURN_FILES ).extract<Poco::JSON::Array::Ptr>();
		std::vector<MetaDataUploadDetail> mduds;
		for ( auto i = files->begin(); i != files->end(); ++i ) {
			Poco::JSON::Object::Ptr file = i->extract<Poco::JSON::Object::Ptr>();
			MetaDataUploadDetail mdud;
			mdud.setId( file->get( RETURN_FILES_ID ).convert<QWORD>() );
			mdud.setName( strToWstr( file->get( RETURN_FILES_NAME ).extract<std::string>() )->c_str() );
			for ( auto j = result_sources.cbegin(); j != result_sources.cend(); ++j ) {
				if ( j->cid == file->get( RETURN_FILES_CID ).extract<int>() ) {
					mdud.setPath( j->local->getPath()->c_str() );
					if ( DirectoryLocal* dl = dynamic_cast< DirectoryLocal* >( j->local ) )
						mdud.asDirectory();
					break;
				}
			}
			for ( auto j = result_sources.cbegin(); j != result_sources.cend(); ++j )
				delete j->local;
			result_sources.clear();
			mduds.push_back( mdud );
		}
		meta_data.setFiles( mduds );
		try {
			std::vector<MetaDataUploadDetail> files_real_path;
			meta_data.write(files_real_path);
			auto j = files_real_path.cbegin();
			for ( auto i = meta_data.begin(); i != meta_data.end(), j != files_real_path.cend(); ++i, ++j ) {
				if ( !i->isDirectory() )
					CopyFile( j->getPath()->c_str(), i->getPath()->c_str(), FALSE );
			}
			if ( setting->getShowWindowUploadAfterCreateNewUpload() ) {
				( ( MainWindow* ) parent->getParent() )->showWindowUpload();
			}
		} catch ( std::exception& e ) {
			TCHAR error[ MAX_LOADSTRING ];
			size_t size = strlen( e.what() );
			mbstowcs_s( &size, error, e.what(), MAX_LOADSTRING );
			MessageBox( parent->getParent()->getHandler(), error, error_marker, MB_ICONERROR | MB_OK );
		}
	}
	if ( json->has( VAR_JSON_MESSAGE ) ) {
		TCHAR buffer[ MAX_LOADSTRING ];
		size_t size = strlen( json->get( VAR_JSON_MESSAGE ).extract<std::string>().c_str() );
		mbstowcs( buffer, json->get( VAR_JSON_MESSAGE ).extract<std::string>().c_str(), MAX_LOADSTRING );
		MessageBox( parent->getParent()->getHandler(), buffer, error_marker, MB_ICONERROR | MB_OK );
	}
}

void UploadFileRequestAT::onPostPostExecute() {
	sources.clear();
}

UploadFileRequestAT::~UploadFileRequestAT() {}

void UploadFileRequestAT::transformFilesToJson( Poco::JSON::Array& json, LPCTSTR path, UINT& cid ) noexcept {
	std::unique_ptr<Local> local = loadLocalByPath( path );
	if ( DirectoryLocal* dl = dynamic_cast< DirectoryLocal* >( local.get() ) ) {
		Poco::JSON::Object obj_dir;
		obj_dir.set( INPUT_FILES_NAME.c_str(), wstrToStr( dl->getName()->c_str() )->c_str() );
		obj_dir.set( INPUT_FILES_IS_DIRECTORY, true );
		obj_dir.set( INPUT_FILES_SIZE.c_str(), "0" );
		obj_dir.set( INPUT_FILES_CID.c_str(), cid );
		SourcesPathReminder spr;
		spr.local = local.release();
		spr.cid = cid;
		result_sources.push_back( spr );
		cid++;
		Poco::JSON::Array arr;
		for ( auto i = dl->begin(); i != dl->end(); ++i )
			transformFilesToJson( arr, ( *i )->getPath()->c_str(), cid );
		obj_dir.set( INPUT_FILES_CHILDREN.c_str(), arr );
		json.add( obj_dir );
	}
	else if ( FileLocal* fl = dynamic_cast< FileLocal* > ( local.get() ) ) {
		Poco::JSON::Object obj_file;
		obj_file.set( INPUT_FILES_NAME.c_str(), wstrToStr( fl->getName()->c_str() )->c_str() );
		obj_file.set( INPUT_FILES_IS_DIRECTORY.c_str(), false );
		obj_file.set( INPUT_FILES_SIZE.c_str(), wstrToStr( to_tstring( fl->getSize() ).c_str() )->c_str() );
		obj_file.set( INPUT_FILES_CID.c_str(), cid );
		SourcesPathReminder spr;
		spr.cid = cid;
		spr.local = local.release();
		result_sources.push_back( spr );
		cid++;
		json.add( obj_file );
	}
}

void UploadFileRequestAT::setTarget( LPCTSTR target ) noexcept { this->target = target; }
void UploadFileRequestAT::setTarget( tstring& target ) noexcept { this->target = target.c_str(); }
void UploadFileRequestAT::setTarget( tstring* target ) noexcept { this->target = target->c_str(); }
void UploadFileRequestAT::addSources( LPCTSTR cloud ) noexcept { sources.push_back( cloud ); }
void UploadFileRequestAT::addSources( tstring& cloud ) noexcept { sources.push_back( cloud.c_str() ); }
void UploadFileRequestAT::addSources( tstring* cloud ) noexcept { sources.push_back( cloud->c_str() ); }
