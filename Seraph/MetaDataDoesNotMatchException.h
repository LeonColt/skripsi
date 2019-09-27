#pragma once
#include "SeraphException.h"
class MetaDataDoesNotMatchException : public SeraphException {
public:
	MetaDataDoesNotMatchException();
	~MetaDataDoesNotMatchException();
};

