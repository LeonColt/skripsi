// Maschinen.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Maschinen.h"


// This is an example of an exported variable
MASCHINEN_API int nMaschinen=0;

QWORD window_class_name_counter = 0;

HINSTANCE instance = NULL;

tstring window_class_name_seed = TEXT( "window_class" );

// This is an example of an exported function.
MASCHINEN_API int fnMaschinen(void)
{
    return 42;
}

MASCHINEN_API std::unique_ptr<tstring> translateErrorCode( DWORD error_code ) {
	std::unique_ptr<tstring> result = std::make_unique<tstring>();
	LPTSTR buffer = nullptr;
	FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ARGUMENT_ARRAY | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, error_code, 0, (LPTSTR)&buffer, 0, NULL );
	if ( buffer != nullptr ) result->assign( buffer );
	return result;
}

MASCHINEN_API bool getLastErrorAsString(LPTSTR buffer, size_t size)
{
	wcscpy_s(buffer, size, L"");
	LPWSTR buff = NULL;
	DWORD error_id = GetLastError();
	if (error_id == 0) return false;
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ARGUMENT_ARRAY | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, error_id, 0, (LPWSTR)&buff, 0, NULL);

	//begin append error code
	TCHAR buffer_code[MAX_LOADSTRING];
	_ultow_s(error_id, buffer_code, 10);
	wcscat_s(buffer, size, L"code : ");
	wcscat_s(buffer, size, buffer_code);
	//end append error code
	if (buff != NULL) wcscat_s(buffer, size, buff);
	return true;
}

MASCHINEN_API bool getLastErrorAsString(LPTSTR buffer, size_t size, DWORD& error_code)
{
	wcscpy_s(buffer, size, L"");
	LPWSTR buff = NULL;
	DWORD error_id = GetLastError();
	if (error_id == 0) return false;
	error_code = error_id;
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ARGUMENT_ARRAY | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, error_id, 0, (LPWSTR)&buff, 0, NULL);

	//begin append error code
	TCHAR buffer_code[MAX_LOADSTRING];
	_ultow_s(error_id, buffer_code, 10);
	wcscat_s(buffer, size, L"code : ");
	wcscat_s(buffer, size, buffer_code);
	//end append error code
	if (buff != NULL) wcscat_s(buffer, size, buff);
	return true;
}

MASCHINEN_API bool sha512String(LPTSTR buffer, size_t size)
{
	HCRYPTPROV provider;
	HCRYPTHASH hasher;


	char buffer_mb[MAX_LOADSTRING];
	size_t size_wchar_buffer = wcslen(buffer);
	wcstombs_s(&size_wchar_buffer, buffer_mb, buffer, MAX_LOADSTRING);
	char* hex = "0123456789abcdef";

	if (!CryptAcquireContext(&provider, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT)) return false;
	BYTE hash[16];
	if (!CryptCreateHash(provider, CALG_SHA_512, 0, 0, &hasher))
	{
		CryptReleaseContext(provider, 0);
		return false;
	}
	if (!CryptHashData(hasher, (BYTE*)buffer_mb, (DWORD)strlen(buffer_mb), 0))
	{
		CryptReleaseContext(provider, 0);
		CryptDestroyHash(hasher);
		return false;
	}
	DWORD length = 16;
	if (!CryptGetHashParam(hasher, HP_HASHVAL, hash, &length, 0))
	{
		CryptReleaseContext(provider, 0);
		CryptDestroyHash(hasher);
		return false;
	}
	CryptDestroyHash(hasher);
	CryptReleaseContext(provider, 0);
	char temp[MAX_LOADSTRING];
	for (DWORD i = 0; i < length; i++)
	{
		temp[i * 2] = hex[hash[i] >> 4];
		temp[(i * 2) + 1] = hex[hash[i] & 0xF];
	}
	temp[length] = '\0';
	size_t size_char_mb = strlen(temp);
	mbstowcs_s(&size_char_mb, buffer, size, temp, size);
#ifdef _DEBUG
	OutputDebugStringW(L"hasil hash : ");
	OutputDebugStringW(buffer);
	OutputDebugStringW(L"\n");
#endif // _DEBUG
	return true;
}

MASCHINEN_API size_t strSplit( LPCSTR source, std::vector<std::string>* result, LPCSTR delimiter ) {
	if ( result == nullptr ) return 0;
	const size_t size_delimiter = strlen( delimiter );
	const size_t source_size = strlen( source );
	switch ( size_delimiter ) {
		case 0: for ( auto i = 0; i < source_size; i++ ) result->push_back( source[ i ] + "" ); break;
		case 1:
		{
			std::string joiner = ""; char out[ 2 ];
			for ( auto i = 0; i < source_size; i++ ) {
				if ( source[ i ] == delimiter[ 0 ] ) {
					result->push_back( joiner.c_str() );
					joiner.clear();
				}
				else {
					out[ 0 ] = source[ i ];
					out[ 1 ] = L'\0';
					joiner.append( out );
				}
			}
			result->push_back( joiner.c_str() );
		} break;

		case 2:
		{
			std::string joiner = "";
			char out[ 2 ];
			char previous = 0;
			for ( auto i = 0; i < source_size; i++ ) {
				if ( previous == delimiter[ 0 ] && source[ i ] == delimiter[ 1 ] ) {
					result->push_back( joiner.c_str() );
					joiner.clear();
				}
				else if ( previous == delimiter[ 0 ] && source[ i ] != delimiter[ 1 ] ) {
					out[ 0 ] = previous;
					out[ 1 ] = L'\0';
					joiner.append( out );
					out[ 0 ] = source[ i ];
					out[ 1 ] = L'\0';
					joiner.append( out );
				}
				else if ( source[ i ] != delimiter[ 0 ] ) {
					out[ 0 ] = source[ i ];
					out[ 1 ] = L'\0';
					joiner.append( out );
				}
				previous = source[ i ];
			}
			result->push_back( joiner.c_str() );
		} break;

		default:
		{
			char joiner[ MAX_LOADSTRING ] = "";
			char joiner2[ MAX_LOADSTRING ] = "";
			char out[ 2 ];
			LONGLONG delimiter_indexer = -1;
			for ( auto i = 0; i != source_size; i++ ) {
				if ( delimiter_indexer > -1 ) {
					if ( delimiter_indexer == ( LONGLONG ) size_delimiter ) {
						result->push_back( joiner2 );
						delimiter_indexer = -1;
					}
					else {
						if ( source[ i ] == delimiter[ delimiter_indexer ] ) {
							delimiter_indexer++;
							out[ 0 ] = source[ i ];
							out[ 1 ] = L'\0';
							strcat_s( joiner, out );
						}
						else {
							delimiter_indexer = -1;
							out[ 0 ] = source[ i ];
							out[ 1 ] = L'\0';
							strcat_s( joiner, out );
						}
					}
				}
				else {
					if ( source[ i ] == delimiter[ 0 ] ) {
						delimiter_indexer = 0;
						out[ 0 ] = source[ i ];
						out[ 1 ] = L'\0';
						strcat_s( joiner, out );
					}
					else {
						out[ 0 ] = source[ i ];
						out[ 1 ] = L'\0';
						strcat_s( joiner, out );
						strcat_s( joiner2, out );
					}
				}
			}
		}
	}
	return result->size();
}

MASCHINEN_API size_t strSplit( const std::string& source, std::vector<std::string>* result, LPCSTR delimiter ) {
	return strSplit( source.c_str(), result, delimiter );
}

MASCHINEN_API size_t wstrSplit( LPCTSTR source, std::vector<std::wstring>* result, LPCTSTR delimiter ) {
	if ( result == nullptr ) return 0;
	const size_t size_delimiter = wcslen( delimiter );
	const size_t source_size = wcslen( source );
	switch ( size_delimiter ) {
		case 0: for ( auto i = 0; i < source_size; i++ ) result->push_back( source[ i ] + TEXT( "" ) ); break;
		case 1:
		{
			std::wstring joiner = L""; WCHAR out[ 2 ];
			for ( auto i = 0; i < source_size; i++ ) {
				if ( source[ i ] == delimiter[ 0 ] ) {
					result->push_back( joiner.c_str() );
					joiner.clear();
				}
				else {
					out[ 0 ] = source[ i ];
					out[ 1 ] = L'\0';
					joiner.append( out );
				}
			}
			if( !joiner.empty() ) result->push_back( joiner.c_str() );
		} break;

		case 2:
		{
			std::wstring joiner;
			WCHAR out[ 2 ];
			WCHAR previous = 0;
			for ( auto i = 0; i < source_size; i++ ) {
				if ( previous == delimiter[ 0 ] && source[ i ] == delimiter[ 1 ] ) {
					result->push_back( joiner.c_str() );
					joiner.clear();
				}
				else if ( previous == delimiter[ 0 ] && source[ i ] != delimiter[ 1 ] ) {
					out[ 0 ] = previous;
					out[ 1 ] = L'\0';
					joiner.append( out );
					out[ 0 ] = source[ i ];
					out[ 1 ] = L'\0';
					joiner.append( out );
				}
				else if ( source[ i ] != delimiter[ 0 ] ) {
					out[ 0 ] = source[ i ];
					out[ 1 ] = L'\0';
					joiner.append( out );
				}
				previous = source[ i ];
			}
			if ( !joiner.empty() ) result->push_back( joiner.c_str() );
		} break;

		default:
		{
			TCHAR joiner[ MAX_LOADSTRING ] = L"";
			TCHAR joiner2[ MAX_LOADSTRING ] = L"";
			TCHAR out[ 2 ];
			LONGLONG delimiter_indexer = -1;
			for ( auto i = 0; i != source_size; i++ ) {
				if ( delimiter_indexer > -1 ) {
					if ( delimiter_indexer == ( LONGLONG ) size_delimiter ) {
						result->push_back( joiner2 );
						delimiter_indexer = -1;
					}
					else {
						if ( source[ i ] == delimiter[ delimiter_indexer ] ) {
							delimiter_indexer++;
							out[ 0 ] = source[ i ];
							out[ 1 ] = L'\0';
							wcscat_s( joiner, out );
						}
						else {
							delimiter_indexer = -1;
							out[ 0 ] = source[ i ];
							out[ 1 ] = L'\0';
							wcscat_s( joiner, out );
						}
					}
				}
				else {
					if ( source[ i ] == delimiter[ 0 ] ) {
						delimiter_indexer = 0;
						out[ 0 ] = source[ i ];
						out[ 1 ] = L'\0';
						wcscat_s( joiner, out );
					}
					else {
						out[ 0 ] = source[ i ];
						out[ 1 ] = L'\0';
						wcscat_s( joiner, out );
						wcscat_s( joiner2, out );
					}
				}
			}
		}
	}
	return result->size();
}

MASCHINEN_API size_t wstrSplit( std::wstring& source, std::vector<std::wstring>* result, LPCTSTR delimiter ) {
	return wstrSplit( source.c_str(), result, delimiter );
}

MASCHINEN_API void setInstance( HINSTANCE p_instance ) {
	instance = p_instance;
}

MASCHINEN_API HINSTANCE getInstance() {
	return instance;
}

MASCHINEN_API void setWindowClassNameSeed( LPCTSTR window_class_name ) {
	window_class_name_seed.assign( window_class_name );
}
MASCHINEN_API tstring& getWindowClassNameSeed() {
	return window_class_name_seed;
}

bool maschinen::Maschinen::process_started = false;

// This is the constructor of a class that has been exported.
// see Maschinen.h for the class definition
maschinen::Maschinen::Maschinen()
{
	filter_min = filter_max = 0;
}

int maschinen::Maschinen::getResultStatus()
{
	return (int)message.wParam;
}

void maschinen::Maschinen::onProcess()
{
	BOOL hasil;
	while ((hasil = GetMessage(&message, NULL, filter_min, filter_max)) != 0)
	{
		if (hasil == -1)
		{
			TCHAR buffer[MAX_LOADSTRING];
			getLastErrorAsString(buffer, MAX_LOADSTRING);
			MessageBox(NULL, buffer, L"ERROR", MB_OK | MB_ICONERROR);
			break;
		}
		else if (!TranslateAccelerator(message.hwnd, accelerator, &message))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
	}
}

void maschinen::Maschinen::beginProcess()
{
	if (!process_started)
	{
		process_started = true;
		onProcess();
	}
}

void maschinen::Maschinen::setFilterMin()
{
	this->filter_min = filter_min;
}

void maschinen::Maschinen::setFilterMax()
{
	this->filter_max = filter_max;
}

void maschinen::Maschinen::setAccelarator(HACCEL accelerator)
{
	this->accelerator = accelerator;
}

MASCHINEN_API std::unique_ptr<std::wstring> strToWstr( LPCSTR input ) {
	const size_t size_buffer = strlen( input ) + 1;
	std::unique_ptr<std::wstring> result = std::make_unique<std::wstring>();
	std::unique_ptr<TCHAR[]> buffer = std::make_unique<TCHAR[]>( size_buffer );
	size_t size = strlen( input );
	mbstowcs_s( &size, buffer.get(), size_buffer, input, size_buffer );
	result->assign( buffer.get() );
	return result;
}

MASCHINEN_API std::unique_ptr<std::wstring> strToWstr( std::string& input ) {
	const size_t size_buffer = strlen( input.c_str() ) + 1;
	std::unique_ptr<std::wstring> result = std::make_unique<std::wstring>();
	std::unique_ptr<TCHAR[]> buffer = std::make_unique<TCHAR[]>( size_buffer );
	size_t size = strlen( input.c_str() );
	mbstowcs_s( &size, buffer.get(), size_buffer, input.c_str(), size_buffer );
	result->assign( buffer.get() );
	return result;
}
MASCHINEN_API std::unique_ptr<std::wstring> strToWstr( std::string* input ) {
	if ( !input ) return std::make_unique<std::wstring>();
	const size_t size_buffer = strlen( input->c_str() ) + 1;
	std::unique_ptr<std::wstring> result = std::make_unique<std::wstring>();
	std::unique_ptr<TCHAR[]> buffer = std::make_unique<TCHAR[]>( size_buffer );
	size_t size = strlen( input->c_str() );
	mbstowcs_s( &size, buffer.get(), size_buffer, input->c_str(), size_buffer );
	result->assign( buffer.get() );
	return result;
}

MASCHINEN_API std::unique_ptr<std::wstring> strToWstr( const std::string& input ) {
	const size_t size_buffer = strlen( input.c_str() ) + 1;
	std::unique_ptr<std::wstring> result = std::make_unique<std::wstring>();
	std::unique_ptr<TCHAR[]> buffer = std::make_unique<TCHAR[]>( size_buffer );
	size_t size = strlen( input.c_str() );
	mbstowcs_s( &size, buffer.get(), size_buffer, input.c_str(), size_buffer );
	result->assign( buffer.get() );
	return result;
}
MASCHINEN_API std::unique_ptr<std::wstring> strToWstr( const std::string* const input ) {
	if ( !input ) return std::make_unique<std::wstring>();
	const size_t size_buffer = strlen( input->c_str() ) + 1;
	std::unique_ptr<std::wstring> result = std::make_unique<std::wstring>();
	std::unique_ptr<TCHAR[]> buffer = std::make_unique<TCHAR[]>( size_buffer );
	size_t size = strlen( input->c_str() );
	mbstowcs_s( &size, buffer.get(), size_buffer, input->c_str(), size_buffer );
	result->assign( buffer.get() );
	return result;
}

MASCHINEN_API std::unique_ptr<std::string> wstrToStr( LPCWSTR input ) {
	const size_t size_buffer = wcslen( input ) + 1;
	std::unique_ptr<std::string> result = std::make_unique<std::string>();
	std::unique_ptr<char[]> buffer = std::make_unique<char[]>( size_buffer );
	size_t size = wcslen( input );
	wcstombs_s( &size, buffer.get(), size_buffer, input, size_buffer );
	result->assign( buffer.get() );
	return result;
}

MASCHINEN_API std::unique_ptr<std::string> wstrToStr( std::wstring& input ) {
	const size_t size_buffer = wcslen( input.c_str() ) + 1;
	std::unique_ptr<std::string> result = std::make_unique<std::string>();
	std::unique_ptr<char[]> buffer = std::make_unique<char[]>( size_buffer );
	size_t size = wcslen( input.c_str() );
	wcstombs_s( &size, buffer.get(), size_buffer, input.c_str(), size_buffer );
	result->assign( buffer.get() );
	return result;
}

MASCHINEN_API std::unique_ptr<std::string> wstrToStr( std::wstring* input ) {
	if ( !input ) return std::make_unique<std::string>();
	const size_t size_buffer = wcslen( input->c_str() ) + 1;
	std::unique_ptr<std::string> result = std::make_unique<std::string>();
	std::unique_ptr<char[]> buffer = std::make_unique<char[]>( size_buffer );
	size_t size = wcslen( input->c_str() );
	wcstombs_s( &size, buffer.get(), size_buffer, input->c_str(), size_buffer );
	result->assign( buffer.get() );
	return result;
}

MASCHINEN_API std::unique_ptr<std::string> wstrToStr( const std::wstring& input ) {
	const size_t size_buffer = wcslen( input.c_str() ) + 1;
	std::unique_ptr<std::string> result = std::make_unique<std::string>();
	std::unique_ptr<char[]> buffer = std::make_unique<char[]>( size_buffer );
	size_t size = wcslen( input.c_str() );
	wcstombs_s( &size, buffer.get(), size_buffer, input.c_str(), size_buffer );
	result->assign( buffer.get() );
	return result;
}
MASCHINEN_API std::unique_ptr<std::string> wstrToStr( const std::wstring* const input ) {
	if ( !input ) return std::make_unique<std::string>();
	const size_t size_buffer = wcslen( input->c_str() ) + 1;
	std::unique_ptr<std::string> result = std::make_unique<std::string>();
	std::unique_ptr<char[]> buffer = std::make_unique<char[]>( size_buffer );
	size_t size = wcslen( input->c_str() );
	wcstombs_s( &size, buffer.get(), size_buffer, input->c_str(), size_buffer );
	result->assign( buffer.get() );
	return result;
}

MASCHINEN_API std::unique_ptr<std::string> strToLower( LPCSTR input ) {
	std::unique_ptr<std::string> temp = std::make_unique<std::string>(input);
	for ( size_t i = 0; i < temp->length(); i++ ) temp->operator[]( i ) = tolower( temp->operator[]( i ) );
	return temp;
}
MASCHINEN_API std::unique_ptr<std::wstring> wstrToLower( LPCWSTR input ) {
	std::unique_ptr<std::wstring> temp = std::make_unique<std::wstring>( input );
	for ( size_t i = 0; i < temp->length(); i++ ) temp->operator[]( i ) = towlower( temp->operator[]( i ) );
	return temp;
}
MASCHINEN_API std::unique_ptr<std::string> strToUpper( LPCSTR input ) {
	std::unique_ptr<std::string> temp = std::make_unique<std::string>( input );
	for ( size_t i = 0; i < temp->length(); i++ ) temp->operator[]( i ) = toupper( temp->operator[]( i ) );
	return temp;
}
MASCHINEN_API std::unique_ptr<std::wstring> wstrToUpper( LPCWSTR input ) {
	std::unique_ptr<std::wstring> temp = std::make_unique<std::wstring>( input );
	for ( size_t i = 0; i < temp->length(); i++ ) temp->operator[]( i ) = towupper( temp->operator[]( i ) );
	return temp;
}

MASCHINEN_API void copySystemTime( LPSYSTEMTIME source, LPSYSTEMTIME target ) noexcept {
	if ( source == nullptr || target == nullptr ) return;
	target->wDay = source->wDay;
	target->wDayOfWeek = source->wDayOfWeek;
	target->wHour = source->wHour;
	target->wMilliseconds = source->wMilliseconds;
	target->wMinute = source->wMinute;
	target->wMonth = source->wMonth;
	target->wSecond = source->wSecond;
	target->wYear = source->wYear;
}