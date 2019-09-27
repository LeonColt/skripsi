#pragma once
#ifndef POCO_NO_UNWINDOWS
#define POCO_NO_UNWINDOWS
#endif // !POCO_NO_UNWINDOWS

#include "Maschinen.h"
#include <Poco\JSON\JSON.h>
#include <Poco\JSON\JSONException.h>
#include <Poco\JSON\Parser.h>
#include <Poco\Dynamic\Var.h>
#include "SeraphException.h"
#include "SeraphCommon.h"

Poco::JSON::Object::Ptr deserialization( LPCTSTR str );
Poco::JSON::Object::Ptr deserialization( LPCSTR str );
Poco::JSON::Object::Ptr deserialization( std::string& str );
Poco::JSON::Object::Ptr deserialization( std::string* str );
std::unique_ptr<tstring> serialization( Poco::JSON::Object* json );

