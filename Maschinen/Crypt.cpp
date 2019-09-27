#include "stdafx.h"
#include "Crypt.h"

maschinen::crypt::Crypt::Crypt( LPCTSTR container, LPCTSTR provider, CryptType prov_type, DWORD flag ) {
	if ( prov_type == RSA_FULL ) {
		if ( CryptAcquireContext( &prov, container, provider, PROV_RSA_FULL, flag ) == FALSE ) {
			TCHAR error[ MAX_LOADSTRING ];
			DWORD error_code;
			if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) )
				throw maschinen::exception::Exception( error, error_code );
			else throw maschinen::exception::Exception();
		}
	}
	else if ( prov_type == RSA_AES ) {
		if ( CryptAcquireContext( &prov, container, provider, PROV_RSA_AES, flag ) == FALSE ) {
			TCHAR error[ MAX_LOADSTRING ];
			DWORD error_code;
			if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) )
				throw maschinen::exception::Exception( error, error_code );
			else throw maschinen::exception::Exception();
		}
	}
}

maschinen::crypt::Crypt::Crypt( CryptType prov_type, DWORD flag ) {
	if ( CryptAcquireContext( &prov, NULL, NULL, prov_type, flag ) == FALSE ) {
		TCHAR error[ MAX_LOADSTRING ];
		DWORD error_code;
		if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) )
			throw maschinen::exception::Exception( error, error_code );
		else throw maschinen::exception::Exception();
	}
}

maschinen::crypt::Crypt::Crypt( CryptType prov_type ) {
	if ( CryptAcquireContext( &prov, NULL, NULL, prov_type, 0 ) == FALSE ) {
		TCHAR error[ MAX_LOADSTRING ];
		DWORD error_code;
		if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) )
			throw maschinen::exception::Exception( error, error_code );
		else throw maschinen::exception::Exception();
	}
}

maschinen::crypt::Crypt::Crypt( DWORD flag ) {
	if ( CryptAcquireContext( &prov, NULL, NULL, PROV_RSA_FULL, 0 ) == FALSE ) {
		TCHAR error[ MAX_LOADSTRING ];
		DWORD error_code;
		if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) )
			throw maschinen::exception::Exception( error, error_code );
		else throw maschinen::exception::Exception();
	}
}

maschinen::crypt::Crypt::Crypt::Crypt() {
	if ( CryptAcquireContext( &prov, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT ) == FALSE ) {
		TCHAR error[ MAX_LOADSTRING ];
		DWORD error_code;
		if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) )
			throw maschinen::exception::Exception( error, error_code );
		else throw maschinen::exception::Exception();
	}
}

std::unique_ptr<tstring> maschinen::crypt::Crypt::randomAlphaNumeric(const DWORD length) const {
	if ( isValid() ) {
		std::unique_ptr<BYTE[]> bytes = std::make_unique<BYTE[]>( length );
		if ( CryptGenRandom( prov, length, bytes.get() ) == FALSE ) {
			TCHAR error[ MAX_LOADSTRING ];
			DWORD error_code;
			if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) )
				throw maschinen::exception::Exception( error, error_code );
			else throw maschinen::exception::Exception();
		}
		std::unique_ptr<tstring> result = std::make_unique<tstring>();
		{
			const size_t buffer_size = length + 1;
			std::unique_ptr<char[]> buffer_mb = std::make_unique<char[]>( buffer_size );
			char* hex = "0123456789abcdef";
			for ( DWORD i = 0; i < length; i++ ) {
				buffer_mb[ i * 2 ] = hex[ bytes[ i ] >> 4 ];
				buffer_mb[ ( i * 2 ) + 1 ] = hex[ bytes[ i ] & 0xF ];
			}
			buffer_mb[ length ] = '\0';
#ifdef _UNICODE
			std::unique_ptr<TCHAR[]> buffer = std::make_unique<TCHAR[]>( buffer_size );
			size_t size = strlen( buffer_mb.get() );
			mbstowcs_s( &size, buffer.get(), buffer_size, buffer_mb.get(), buffer_size );
			result->assign( buffer.get() );
#else
			result->assign( buffer_mb.get() );
#endif // _UNICODE
		}
		return result;
	}
	return std::make_unique<tstring>();
}

std::unique_ptr<tstring> maschinen::crypt::Crypt::randomString( const DWORD length ) const {
	if ( isValid() ) {
		std::unique_ptr<BYTE[]> bytes = std::make_unique<BYTE[]>( length );
		if ( CryptGenRandom( prov, length, bytes.get() ) == FALSE ) {
			TCHAR error[ MAX_LOADSTRING ];
			DWORD error_code;
			if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) )
				throw maschinen::exception::Exception( error, error_code );
			else throw maschinen::exception::Exception();
		}
		size_t buffer_size = length + 1;
		std::unique_ptr<char[]> buffer_mb = std::make_unique<char[]>( buffer_size );
		for ( DWORD i = 0; i < length; i++ ) {
			if ( bytes[ i ] > 127 )  buffer_mb[ i ] = bytes[ i ] - 127 - 1 - 127 - 1;
			else buffer_mb[ i ] = bytes[ i ];
		}
		buffer_mb[ length ] = '\0';
		std::unique_ptr<tstring> result = std::make_unique<tstring>();
#ifdef _UNICODE
		std::unique_ptr<TCHAR[]> buffer = std::make_unique<TCHAR[]>( buffer_size );
		size_t size_buffer_mb = strlen( buffer_mb.get() );
		mbstowcs_s( &size_buffer_mb, buffer.get(), buffer_size, buffer_mb.get(), buffer_size );
		result->assign( buffer.get() );
#else
		result->assign( buffer_mb.get() );
#endif // _UNICODE
		return result;
	}
	return std::make_unique<tstring>();
}

void maschinen::crypt::Crypt::randomByte( BYTE* buffer, size_t size ) const {
	if ( isValid() ) {
		if ( CryptGenRandom( prov, ( DWORD ) size, buffer ) == FALSE ) {
			TCHAR error[ MAX_LOADSTRING ];
			DWORD error_code;
			if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) )
				throw maschinen::exception::Exception( error, error_code );
			else throw maschinen::exception::Exception();
		}
	}
}

std::unique_ptr<tstring> maschinen::crypt::Crypt::md5( LPCTSTR input ) {
	HCRYPTHASH hasher;
	if ( CryptCreateHash( prov, CALG_MD5, 0, 0, &hasher ) == FALSE ) {
		TCHAR error[ MAX_LOADSTRING ];
		DWORD error_code;
		if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) )
			throw maschinen::exception::Exception( error, error_code );
		else throw maschinen::exception::Exception();
	}

	{
#ifdef _UNICODE
		const size_t buffer_size = wcslen( input ) + 1;
		std::unique_ptr<char[]> buffer_mb = std::make_unique<char[]>( buffer_size );
		size_t size = wcslen( input );
		wcstombs_s( &size, buffer_mb.get(), buffer_size, input, buffer_size );
		if ( CryptHashData( hasher, ( BYTE* ) buffer_mb.get(), ( DWORD ) strlen( buffer_mb.get() ), 0 ) == FALSE ) {
			CryptDestroyHash( hasher );
			TCHAR error[ MAX_LOADSTRING ];
			DWORD error_code;
			if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) )
				throw maschinen::exception::Exception( error, error_code );
			else throw maschinen::exception::Exception();
		}
#else
		const size_t buffer_size = wcslen( input ) + 1;
		std::unique_ptr<char[]> buffer_mb = std::make_unique<char[]>( buffer_size );
		strcpy_s( buffer_mb.get(), buffer_size, input );
		if ( CryptHashData( hasher, ( BYTE* ) buffer_mb.get(), ( DWORD ) strlen( buffer_mb.get() ), 0 ) == FALSE ) {
			CryptDestroyHash( hasher );
			TCHAR error[ MAX_LOADSTRING ];
			DWORD error_code;
			if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) )
				throw maschinen::exception::Exception( error, error_code );
			else throw maschinen::exception::Exception();
		}
#endif // _UNICODE
	}
	const DWORD BUFFER_BYTE_LENGTH = 16;
	BYTE buffer_byte[ BUFFER_BYTE_LENGTH ];
	{
		DWORD temp_length = BUFFER_BYTE_LENGTH;
		if ( CryptGetHashParam( hasher, HP_HASHVAL, buffer_byte, &temp_length, 0 ) == FALSE ) {
			CryptDestroyHash( hasher );
			TCHAR error[ MAX_LOADSTRING ];
			DWORD error_code;
			if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) )
				throw maschinen::exception::Exception( error, error_code );
			else throw maschinen::exception::Exception();
		}
	}
	CryptDestroyHash( hasher );
	std::unique_ptr<tstring> result = std::make_unique<tstring>();
	{
		char* hex = "0123456789abcdef";
		char temp[ MAX_LOADSTRING ];
		for ( DWORD i = 0; i < BUFFER_BYTE_LENGTH; i++ ) {
			temp[ i * 2 ] = hex[ buffer_byte[ i ] >> 4 ];
			temp[ ( i * 2 ) + 1 ] = hex[ buffer_byte[ i ] & 0xF ];
		}
		temp[ BUFFER_BYTE_LENGTH ] = '\0';
#ifdef _UNICODE
		TCHAR buffer[ MAX_LOADSTRING ];
		size_t size = strlen( temp );
		mbstowcs_s( &size, buffer, temp, MAX_LOADSTRING );
		result->assign( buffer );
#else
		result->assign( temp );
#endif // _UNICODE
	}
	return result;
}

std::unique_ptr<tstring> maschinen::crypt::Crypt::md5( const tstring& input ) {
	HCRYPTHASH hasher;

	if ( CryptCreateHash( prov, CALG_MD5, 0, 0, &hasher ) == FALSE ) {
		TCHAR error[ MAX_LOADSTRING ];
		DWORD error_code;
		if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) )
			throw maschinen::exception::Exception( error, error_code );
		else throw maschinen::exception::Exception();
	}

	{
#ifdef _UNICODE
		char buffer_mb[ MAX_LOADSTRING ];
		size_t size = wcslen( input.c_str() );
		wcstombs_s( &size, buffer_mb, input.c_str(), MAX_LOADSTRING );
		if ( CryptHashData( hasher, ( BYTE* ) buffer_mb, ( DWORD ) strlen( buffer_mb ), 0 ) == FALSE ) {
			CryptDestroyHash( hasher );
			TCHAR error[ MAX_LOADSTRING ];
			DWORD error_code;
			if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) )
				throw maschinen::exception::Exception( error, error_code );
			else throw maschinen::exception::Exception();
		}
#else
		if ( CryptHashData( hasher, ( BYTE* ) input, ( DWORD ) strlen( input ), 0 ) == FALSE ) {
			CryptDestroyHash( hasher );
			TCHAR error[ MAX_LOADSTRING ];
			DWORD error_code;
			if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) )
				throw maschinen::exception::Exception( error, error_code );
			else throw maschinen::exception::Exception();
		}
#endif // _UNICODE
	}
	const DWORD BUFFER_BYTE_LENGTH = 16;
	BYTE buffer_byte[ BUFFER_BYTE_LENGTH ];
	{
		DWORD temp_length = BUFFER_BYTE_LENGTH;
		if ( CryptGetHashParam( hasher, HP_HASHVAL, buffer_byte, &temp_length, 0 ) == FALSE ) {
			CryptDestroyHash( hasher );
			TCHAR error[ MAX_LOADSTRING ];
			DWORD error_code;
			if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) )
				throw maschinen::exception::Exception( error, error_code );
			else throw maschinen::exception::Exception();
		}
	}
	CryptDestroyHash( hasher );
	std::unique_ptr<tstring> result = std::make_unique<tstring>();
	{
		char* hex = "0123456789abcdef";
		char temp[ MAX_LOADSTRING ];
		for ( DWORD i = 0; i < BUFFER_BYTE_LENGTH; i++ ) {
			temp[ i * 2 ] = hex[ buffer_byte[ i ] >> 4 ];
			temp[ ( i * 2 ) + 1 ] = hex[ buffer_byte[ i ] & 0xF ];
		}
		temp[ BUFFER_BYTE_LENGTH ] = '\0';
#ifdef _UNICODE
		TCHAR buffer[ MAX_LOADSTRING ];
		size_t size = strlen( temp );
		mbstowcs_s( &size, buffer, temp, MAX_LOADSTRING );
		result->assign( buffer );
#else
		result->assign( temp );
#endif // _UNICODE
	}
	return result;
}

std::unique_ptr<tstring> maschinen::crypt::Crypt::md5( const tstring* input ) {
	HCRYPTHASH hasher;

	if ( CryptCreateHash( prov, CALG_MD5, 0, 0, &hasher ) == FALSE ) {
		TCHAR error[ MAX_LOADSTRING ];
		DWORD error_code;
		if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) )
			throw maschinen::exception::Exception( error, error_code );
		else throw maschinen::exception::Exception();
	}

	{
#ifdef _UNICODE
		char buffer_mb[ MAX_LOADSTRING ];
		size_t size = wcslen( input->c_str() );
		wcstombs_s( &size, buffer_mb, input->c_str(), MAX_LOADSTRING );
		if ( CryptHashData( hasher, ( BYTE* ) buffer_mb, ( DWORD ) strlen( buffer_mb ), 0 ) == FALSE ) {
			CryptDestroyHash( hasher );
			TCHAR error[ MAX_LOADSTRING ];
			DWORD error_code;
			if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) )
				throw maschinen::exception::Exception( error, error_code );
			else throw maschinen::exception::Exception();
		}
#else
		if ( CryptHashData( hasher, ( BYTE* ) input, ( DWORD ) strlen( input ), 0 ) == FALSE ) {
			CryptDestroyHash( hasher );
			TCHAR error[ MAX_LOADSTRING ];
			DWORD error_code;
			if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) )
				throw maschinen::Exception( error, error_code );
			else throw maschinen::Exception();
		}
#endif // _UNICODE
	}
	const DWORD BUFFER_BYTE_LENGTH = 16;
	BYTE buffer_byte[ BUFFER_BYTE_LENGTH ];
	{
		DWORD temp_length = BUFFER_BYTE_LENGTH;
		if ( CryptGetHashParam( hasher, HP_HASHVAL, buffer_byte, &temp_length, 0 ) == FALSE ) {
			CryptDestroyHash( hasher );
			TCHAR error[ MAX_LOADSTRING ];
			DWORD error_code;
			if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) )
				throw maschinen::exception::Exception( error, error_code );
			else throw maschinen::exception::Exception();
		}
	}
	CryptDestroyHash( hasher );
	std::unique_ptr<tstring> result = std::make_unique<tstring>();
	{
		char* hex = "0123456789abcdef";
		char temp[ MAX_LOADSTRING ];
		for ( DWORD i = 0; i < BUFFER_BYTE_LENGTH; i++ ) {
			temp[ i * 2 ] = hex[ buffer_byte[ i ] >> 4 ];
			temp[ ( i * 2 ) + 1 ] = hex[ buffer_byte[ i ] & 0xF ];
		}
		temp[ BUFFER_BYTE_LENGTH ] = '\0';
#ifdef _UNICODE
		TCHAR buffer[ MAX_LOADSTRING ];
		size_t size = strlen( temp );
		mbstowcs_s( &size, buffer, temp, MAX_LOADSTRING );
		result->assign( buffer );
#else
		result->assign( temp );
#endif // _UNICODE
	}
	return result;
}

bool maschinen::crypt::Crypt::isValid() const {
	return prov != NULL;
}

maschinen::crypt::Crypt::~Crypt() {
	if ( prov != NULL ) CryptReleaseContext( prov, 0 );
}
