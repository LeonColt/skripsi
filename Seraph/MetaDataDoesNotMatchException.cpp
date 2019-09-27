#include "stdafx.h"
#include "MetaDataDoesNotMatchException.h"


MetaDataDoesNotMatchException::MetaDataDoesNotMatchException() : SeraphException( L"Meta data does not match", dword::error_code::meta_data::META_DATA_DOES_NOT_MATCH ) {
}


MetaDataDoesNotMatchException::~MetaDataDoesNotMatchException() {
}
