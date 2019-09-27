#include "stdafx.h"
#include "JSON.h"


Poco::JSON::Object::Ptr deserialization( LPCTSTR str ) {
	Poco::JSON::Object::Ptr json;
	try {
		Poco::JSON::Parser parser;
		parser.parse( wstrToStr( str )->c_str() );
		Poco::DynamicAny result = parser.result();
		if ( result.type() == typeid( Poco::JSON::Object::Ptr ) )
			json = result.extract<Poco::JSON::Object::Ptr>();
	} catch ( Poco::JSON::JSONException& e ) {
		throw SeraphException( string::file::name::ERROR_LOG_NAME.c_str(), strToWstr( e.what() )->c_str(), 0 );
	} catch ( Poco::SyntaxException& e ) {
		throw SeraphException( string::file::name::ERROR_LOG_NAME.c_str(), strToWstr( e.what() )->c_str(), 0 );
	} catch ( std::exception& e ) {
		throw SeraphException( string::file::name::ERROR_LOG_NAME.c_str(), strToWstr( e.what() )->c_str(), 0 );
	}
	return json;
}

Poco::JSON::Object::Ptr deserialization( LPCSTR str ) {
	Poco::JSON::Object::Ptr json;
	try {
		Poco::JSON::Parser parser;
		parser.parse( str );
		Poco::DynamicAny result = parser.result();
		if ( result.type() == typeid( Poco::JSON::Object::Ptr ) )
			json = result.extract<Poco::JSON::Object::Ptr>();
	} catch ( Poco::JSON::JSONException& e ) {
		throw SeraphException( string::file::name::ERROR_LOG_NAME.c_str(), strToWstr( e.what() )->c_str(), 0 );
	} catch ( Poco::SyntaxException& e ) {
		throw SeraphException( string::file::name::ERROR_LOG_NAME.c_str(), strToWstr( e.what() )->c_str(), 0 );
	} catch ( std::exception& e ) {
		throw SeraphException( string::file::name::ERROR_LOG_NAME.c_str(), strToWstr( e.what() )->c_str(), 0 );
	}
	return json;
}

Poco::JSON::Object::Ptr deserialization( std::string& str ) {
	Poco::JSON::Object::Ptr json;
	try {
		Poco::JSON::Parser parser;
		parser.parse( str );
		Poco::DynamicAny result = parser.result();
		if ( result.type() == typeid( Poco::JSON::Object::Ptr ) )
			json = result.extract<Poco::JSON::Object::Ptr>();
	} catch ( Poco::JSON::JSONException& e ) {
		throw SeraphException( string::file::name::ERROR_LOG_NAME.c_str(), strToWstr( e.what() )->c_str(), 0 );
	} catch ( Poco::SyntaxException& e ) {
		throw SeraphException( string::file::name::ERROR_LOG_NAME.c_str(), strToWstr( e.what() )->c_str(), 0 );
	} catch ( std::exception& e ) {
		throw SeraphException( string::file::name::ERROR_LOG_NAME.c_str(), strToWstr( e.what() )->c_str(), 0 );
	}
	return json;
}

Poco::JSON::Object::Ptr deserialization( std::string* str ) {
	Poco::JSON::Object::Ptr json;
	try {
		Poco::JSON::Parser parser;
		parser.parse( str->c_str() );
		Poco::DynamicAny result = parser.result();
		if ( result.type() == typeid( Poco::JSON::Object::Ptr ) )
			json = result.extract<Poco::JSON::Object::Ptr>();
	} catch ( Poco::JSON::JSONException& e ) {
		throw SeraphException( string::file::name::ERROR_LOG_NAME.c_str(), strToWstr( e.what() )->c_str(), 0 );
	} catch ( Poco::SyntaxException& e ) {
		throw SeraphException( string::file::name::ERROR_LOG_NAME.c_str(), strToWstr( e.what() )->c_str(), 0 );
	} catch ( std::exception& e ) {
		throw SeraphException( string::file::name::ERROR_LOG_NAME.c_str(), strToWstr( e.what() )->c_str(), 0 );
	}
	return json;
}

std::unique_ptr<tstring> serialization( Poco::JSON::Object* json ) {
	std::unique_ptr<tstring> res = std::make_unique<tstring>();
	std::ostringstream oss;
	Poco::JSON::Stringifier::stringify( *json, oss );
	res->assign( strToWstr( oss.str().c_str() )->c_str() );
	return res;
}
