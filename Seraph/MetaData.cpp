#include "stdafx.h"
#include "MetaData.h"


MetaData::MetaData(){}

bool MetaData::exists() {DWORD result = GetFileAttributes( getPath()->c_str() );return result != INVALID_FILE_ATTRIBUTES && !(result & FILE_ATTRIBUTE_DIRECTORY);}

void MetaData::create() {
	if ( !exists() ) {
		if ( CreateDirectoryW( getPath()->c_str(), NULL ) == 0 ) {
			DWORD error_code;
			TCHAR error[ MAX_LOADSTRING ];
			if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) ) 
				throw SeraphException( string::file::name::ERROR_LOG_NAME.c_str(), error, error_code );
			else throw SeraphException();
		}
	}
}

std::unique_ptr<tstring> MetaData::getParentPath() const {
	std::unique_ptr<tstring> path = std::make_unique<tstring>();
	path->append( getDefaultAppDataPath( true )->c_str() );
	path->append( string::directory::name::FILE_META_DATA.c_str() );
	return path;
}

std::unique_ptr<tstring> MetaData::getPath() const {
	std::unique_ptr<tstring> path = std::make_unique<tstring>(getParentPath()->c_str());
	path->append( TEXT( "\\" ) );
	path->append( getMetaName()->c_str() );
	return path;
}

std::unique_ptr<tstring> MetaData::getMetaName() const {
	if ( id.length() > 0 ) {
		maschinen::crypt::Crypt crypt;
		return std::make_unique<tstring>( crypt.md5( id )->c_str() );
	}
	else return std::make_unique<tstring>( nullptr );
}

MetaData::~MetaData(){}

void MetaData::setAkey( Akey* akey ) {this->akey = akey;} void MetaData::setSetting( Setting* setting ) {this->setting = setting;} void MetaData::setUserCredential( UserCredential* user_credential ) {this->user_credential = user_credential;}
void MetaData::setId( LPCTSTR id ) noexcept { this->id.assign( id ); }
std::unique_ptr<tstring> MetaData::getId() const noexcept { return std::make_unique<tstring>( id.c_str() ); }
bool MetaData::operator==( MetaData& meta_data ) {return id.compare( meta_data.id ) == 0;}
bool MetaData::operator==( MetaData* meta_data ) {return id.compare( meta_data->id.c_str() ) == 0;}
void MetaData::read( tstring& meta_name ) {
	read( meta_name.c_str() );
}

void MetaData::read( tstring* meta_name ) {
	read( meta_name->c_str() );
}

void MetaData::read() {
	read( getMetaName()->c_str() );
}
