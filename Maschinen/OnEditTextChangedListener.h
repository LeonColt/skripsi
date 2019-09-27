#pragma once
#include "Maschinen.h"
namespace maschinen {
	namespace event {
		namespace listener {
			class MASCHINEN_API OnEditTextChangedListener {
			public:
				OnEditTextChangedListener();
				virtual void beforeTextChanged() = 0;
				virtual void afterTextChanged() = 0;
				~OnEditTextChangedListener();
			};
		}
	}
}

