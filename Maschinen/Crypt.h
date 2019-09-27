#pragma once
#include<wincrypt.h>
#include "Maschinen.h"
#include "Exception.h"
namespace maschinen {
	namespace crypt {
		class MASCHINEN_API Crypt {
		private:
			HCRYPTPROV prov;
		public:
			const enum CryptType { RSA_FULL, RSA_AES, RSA_SIG, RSA_SCHANNEL, DDS, DDS_DH, DH_SCHANNEL, FORTEZZA, MS_EXCHANGE, SSL };
			Crypt( LPCTSTR container, LPCTSTR provider, CryptType prov_type, DWORD flag );
			Crypt( CryptType prov_type, DWORD flag );
			Crypt( CryptType prov_type );
			Crypt( DWORD flag );
			Crypt();
			std::unique_ptr<tstring> md5( LPCTSTR input );
			std::unique_ptr<tstring> md5( const tstring& input );
			std::unique_ptr<tstring> md5( const tstring* input );
			std::unique_ptr<tstring> randomAlphaNumeric( const DWORD length ) const;
			std::unique_ptr<tstring> randomString(const DWORD length) const;
			void randomByte(BYTE* buffer, size_t size) const;
			//std::unique_ptr<tstring> sha512(LPCTSTR input);
			bool isValid() const;
			~Crypt();
		};
	}
}

