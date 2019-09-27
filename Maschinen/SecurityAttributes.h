#pragma once
#include "Maschinen.h"
namespace maschinen {
	namespace security {
		class MASCHINEN_API SecurityAttributes {
		private:
			SECURITY_ATTRIBUTES security_attributes;
		public:
			SecurityAttributes();
			void setInheritable( bool inheritable = true );
			void set( SECURITY_ATTRIBUTES& security_attributes );
			void set( LPSECURITY_ATTRIBUTES lp_security_attributes );
			SECURITY_ATTRIBUTES& get() noexcept;
			LPSECURITY_ATTRIBUTES getLP() noexcept;
			~SecurityAttributes();
		};
	}
}

