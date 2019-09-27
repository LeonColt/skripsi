#pragma once
#include <exception>
#include "Maschinen.h"
namespace maschinen {
	namespace exception {
		class MASCHINEN_API Exception : public std::exception {
		private:
			DWORD error_code;
		public:
			Exception() throw( ) : std::exception() {
				error_code = 0;
			}
			Exception(wchar_t const* _Message) throw() : std::exception( wideToMultiByte( _Message )->c_str() ) {
				error_code = 0;
			}
			Exception( char const* const _Message, DWORD error_code ) throw( ) : std::exception(_Message) {
				this->error_code = error_code;
			}
			Exception( wchar_t const* const _Message, DWORD error_code ) throw ( ) : std::exception(wideToMultiByte(_Message)->c_str()) {
				this->error_code = error_code;
			}
			virtual void getMessage( LPTSTR buffer, size_t size ) const throw( ) {
				size_t size_temp = strlen( what() );
				mbstowcs_s( &size_temp, buffer, size, what(), size );
			}
			virtual std::unique_ptr<tstring> getMessage() const throw( ) {
				return std::make_unique<tstring>( multiByteToWide(what())->c_str() );
			}
			virtual DWORD getErrorCode() const throw( ) {
				return error_code;
			}
			std::unique_ptr<std::string> wideToMultiByte( wchar_t const* const _Message ) const {
				size_t size = wcslen( _Message );
				char* temp = new char[ size + 1 ];
				wcstombs_s( &size, temp, size + 1, _Message, size + 1 );
				std::unique_ptr<std::string> p_temp = std::make_unique<std::string>( temp );
				delete[] temp;
				return p_temp;
			}
			std::unique_ptr<std::wstring> multiByteToWide( char const* const _Message ) const {
				size_t size = strlen( _Message );
				wchar_t* temp = new wchar_t[ size + 1 ];
				mbstowcs_s( &size, temp, size + 1, _Message, size + 1 );
				std::unique_ptr<std::wstring> p_temp = std::make_unique<std::wstring>( temp );
				delete[] temp;
				return p_temp;
			}
		};
	}
}

