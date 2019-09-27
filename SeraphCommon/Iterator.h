#pragma once
#include "Local.h"
class Iterator : public std::iterator<std::input_iterator_tag, Local, ptrdiff_t, Local*, Local&> {
private:
	Local* pointer;
public:
	Iterator();
	~Iterator();
};

