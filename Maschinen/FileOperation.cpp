#include "stdafx.h"
#include "FileOperation.h"


maschinen::com::FileOperation::FileOperation( const ComponentObjectModel* com ) : COM( com ) {
	HRESULT hr = CoCreateInstance( CLSID_FileOperation, NULL, CLSCTX_ALL, IID_PPV_ARGS( &ifo ) );
	if ( hr == REGDB_E_CLASSNOTREG ) {

	}
	else if ( hr == CLASS_E_NOAGGREGATION ) {

	}
	else if ( hr == E_NOINTERFACE ) {

	}
	else if ( hr == E_POINTER ) {

	}
}

void maschinen::com::FileOperation::rename( LPCTSTR path, LPCTSTR new_name ) const {
	DWORD attributes = GetFileAttributes( path );
	if ( attributes == INVALID_FILE_ATTRIBUTES ) {
		TCHAR error[ MAX_LOADSTRING ];
		DWORD error_code;
		if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) )
			throw maschinen::exception::Exception( error, error_code );
		else throw maschinen::exception::Exception();
	}
	HRESULT hr;
	IShellItem* psi = NULL;
	hr = SHCreateItemFromParsingName( path, NULL, IID_PPV_ARGS( &psi ) );
	if ( FAILED( hr ) ) {
		OutputDebugStringW( to_tstring( hr ).c_str() );
		_com_error error( hr );
		throw maschinen::exception::Exception( error.ErrorMessage(), hr );
	}
	hr = ifo->RenameItem( psi, new_name, NULL );
	psi->Release();
	if ( FAILED( hr ) ) {
		OutputDebugStringW( to_tstring( hr ).c_str() );
		_com_error error( hr );
		throw maschinen::exception::Exception( error.ErrorMessage(), hr );
	}
}

void maschinen::com::FileOperation::move( LPCTSTR from, LPCTSTR to ) const {
	DWORD attributes_from, attributes_to;
	attributes_from = GetFileAttributes( from );
	if ( attributes_from == INVALID_FILE_ATTRIBUTES ) {
		TCHAR error[ MAX_LOADSTRING ];
		DWORD error_code;
		if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) )
			throw maschinen::exception::Exception( error, error_code );
		else throw maschinen::exception::Exception();
	}
	attributes_to = GetFileAttributes( to );
	if ( attributes_to == INVALID_FILE_ATTRIBUTES ) {
		TCHAR error[ MAX_LOADSTRING ];
		DWORD error_code;
		if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) )
			throw maschinen::exception::Exception( error, error_code );
		else throw maschinen::exception::Exception();
	}
	if ( !( attributes_to & FILE_ATTRIBUTE_DIRECTORY ) ) throw maschinen::exception::FileNotDirectory();
	IShellItem* psi_from = NULL;
	IShellItem* psi_to = NULL;
	HRESULT hr = SHCreateItemFromParsingName( from, NULL, IID_PPV_ARGS( &psi_from ) );
	if ( FAILED( hr ) ) {
		_com_error error( hr );
		throw maschinen::exception::Exception( error.ErrorMessage(), hr );
	}
	hr = SHCreateItemFromParsingName( to, NULL, IID_PPV_ARGS( &psi_to ) );
	if ( FAILED( hr ) ) {
		psi_from->Release();
		_com_error error( hr );
		throw maschinen::exception::Exception( error.ErrorMessage(), hr );
	}
	hr = ifo->MoveItem( psi_from, psi_to, NULL, NULL );
	psi_from->Release();
	psi_to->Release();
	if ( FAILED( hr ) ) {
		_com_error error( hr );
		throw maschinen::exception::Exception( error.ErrorMessage(), hr );
	}
}

void maschinen::com::FileOperation::copy( LPCTSTR from, LPCTSTR to ) const {
	DWORD attributes_from, attributes_to;
	attributes_from = GetFileAttributes( from );
	if ( attributes_from == INVALID_FILE_ATTRIBUTES ) {
		TCHAR error[ MAX_LOADSTRING ];
		DWORD error_code;
		if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) )
			throw maschinen::exception::Exception( error, error_code );
		else throw maschinen::exception::Exception();
	}
	attributes_to = GetFileAttributes( to );
	if ( attributes_to == INVALID_FILE_ATTRIBUTES ) {
		TCHAR error[ MAX_LOADSTRING ];
		DWORD error_code;
		if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) )
			throw maschinen::exception::Exception( error, error_code );
		else throw maschinen::exception::Exception();
	}
	if ( !( attributes_to & FILE_ATTRIBUTE_DIRECTORY ) ) throw maschinen::exception::FileNotDirectory();
	HRESULT hr;
	IShellItem* ishellitem_from = NULL;
	IShellItem* ishellitem_to = NULL;
	hr = SHCreateItemFromParsingName( from, NULL, IID_PPV_ARGS( &ishellitem_from ) );
	if ( FAILED( hr ) ) {
		_com_error error( hr );
		throw maschinen::exception::Exception( error.ErrorMessage(), hr );
	}
	hr = SHCreateItemFromParsingName( to, NULL, IID_PPV_ARGS( &ishellitem_to ) );
	if ( FAILED( hr ) ) {
		ishellitem_from->Release();
		_com_error error( hr );
		throw maschinen::exception::Exception( error.ErrorMessage(), hr );
	}
	hr = ifo->CopyItem( ishellitem_from, ishellitem_to, NULL, NULL );
	ishellitem_from->Release(); ishellitem_to->Release();
	if ( FAILED( hr ) ) {
		_com_error error( hr );
		throw maschinen::exception::Exception( error.ErrorMessage(), hr );
	}
}

void maschinen::com::FileOperation::deleteFile( LPCTSTR path ) const {
	DWORD attributes = GetFileAttributes( path );
	if ( attributes == INVALID_FILE_ATTRIBUTES ) {
		TCHAR error[ MAX_LOADSTRING ];
		DWORD error_code;
		if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) )
			throw maschinen::exception::Exception( error, error_code );
		else throw maschinen::exception::Exception();
	}
	HRESULT hr;
	IShellItem* psi = NULL;
	hr = SHCreateItemFromParsingName( path, NULL, IID_PPV_ARGS( &psi ) );
	if ( FAILED( hr ) ) {
		_com_error error( hr );
		throw maschinen::exception::Exception( error.ErrorMessage(), hr );
	}
	hr = ifo->DeleteItem( psi, NULL );
	psi->Release();
	if ( FAILED( hr ) ) {
		_com_error error( hr );
		throw maschinen::exception::Exception( error.ErrorMessage(), hr );
	}
}

void maschinen::com::FileOperation::deleteFile( const std::vector<tstring>* const files ) const {
	IShellItemArray* psia = NULL;
	PIDLIST_ABSOLUTE* rgpidl = new( std::nothrow ) PIDLIST_ABSOLUTE[ files->size() ];
	HRESULT hr = rgpidl ? S_OK : E_OUTOFMEMORY;
	if ( FAILED( hr ) ) {
		_com_error error( hr );
		throw maschinen::exception::Exception( error.ErrorMessage(), hr );
	}
	size_t counter = 0;
	for ( size_t i = 0; i < files->size(); i++ ) {
		if ( GetFileAttributes( ( *files )[ i ].c_str() ) != INVALID_FILE_ATTRIBUTES ) {
			hr = SHParseDisplayName( ( *files )[ i ].c_str(), nullptr, &rgpidl[ counter ], 0, nullptr );
			if ( FAILED( hr ) ) {
				for ( size_t j = 0; j < counter; j++ ) CoTaskMemFree( rgpidl[ i ] );
				delete[] rgpidl;
				_com_error error( hr );
				throw maschinen::exception::Exception( error.ErrorMessage(), hr );
			}
			counter++;
		}
	}
	hr = SHCreateShellItemArrayFromIDLists( (UINT)counter, ( LPCITEMIDLIST * )rgpidl, &psia );
	for ( size_t i = 0; i < files->size(); i++ ) CoTaskMemFree( rgpidl[ i ] );
	delete[] rgpidl;

	if ( FAILED( hr ) ) {
		_com_error error( hr );
		throw maschinen::exception::Exception( error.ErrorMessage(), hr );
	}

	hr = ifo->DeleteItems( psia );

	if ( FAILED( hr ) ) {
		_com_error error( hr );
		throw maschinen::exception::Exception( error.ErrorMessage(), hr );
	}
}

void maschinen::com::FileOperation::rename( const std::vector<tstring>* const old_names, LPCTSTR new_name ) const {
	IShellItemArray* psia = NULL;
	PIDLIST_ABSOLUTE* rgpidl = new ( std::nothrow )PIDLIST_ABSOLUTE[ old_names->size() ];
	HRESULT hr = rgpidl ? S_OK : E_OUTOFMEMORY;
	if( FAILED(hr) ) {
		_com_error error( hr );
		throw maschinen::exception::Exception( error.ErrorMessage(), hr );
	}
	size_t counter = 0;
	for ( ; counter < old_names->size(); ++counter ) {
		hr = SHParseDisplayName( ( *old_names )[ counter ].c_str(), nullptr, &rgpidl[ counter ], 0, nullptr );
		if( FAILED(hr) ) {
			for ( size_t i = 0; i < counter; ++i ) CoTaskMemFree( rgpidl[ i ] );
			delete[] rgpidl;
			_com_error error( hr );
			throw maschinen::exception::Exception( error.ErrorMessage(), hr );
		}
	}
	hr = SHCreateShellItemArrayFromIDLists( ( UINT ) counter, ( LPCITEMIDLIST* ) rgpidl, &psia );
	for( size_t i = 0; i < old_names->size(); i++ ) CoTaskMemFree( rgpidl[ i ] );
	delete[] rgpidl;
	if( FAILED(hr) ) {
		_com_error error( hr );
		throw maschinen::exception::Exception( error.ErrorMessage(), hr );
	}
	hr = ifo->RenameItems( psia, new_name );
	if( FAILED(hr) ) {
		_com_error error( hr );
		throw maschinen::exception::Exception( error.ErrorMessage(), hr );
	}
}

void maschinen::com::FileOperation::move( const std::vector<tstring>* const froms, LPCTSTR to ) const {
	IShellItemArray* psia = NULL;

	PIDLIST_ABSOLUTE* rgpidl = new ( std::nothrow )PIDLIST_ABSOLUTE[ froms->size() ];
	HRESULT hr = rgpidl ? S_OK : E_OUTOFMEMORY;

	if ( FAILED( hr ) ) {
		_com_error error( hr );
		throw maschinen::exception::Exception( error.ErrorMessage(), hr );
	}

	size_t counter = 0;
	for ( ; counter < froms->size(); counter++ ) {
		hr = SHParseDisplayName( ( *froms )[ counter ].c_str(), nullptr, &rgpidl[ counter ], 0, nullptr );
		if ( FAILED( hr ) ) {
			for ( size_t i = 0; i < counter; i++ ) CoTaskMemFree( rgpidl[ i ] );
			delete[] rgpidl;
			_com_error error( hr );
			throw maschinen::exception::Exception( error.ErrorMessage(), hr );
		}
	}
	hr = SHCreateShellItemArrayFromIDLists( ( UINT ) counter, ( LPCITEMIDLIST * ) rgpidl, &psia );
	for ( size_t i = 0; i < froms->size(); i++ ) CoTaskMemFree( rgpidl[ i ] );
	delete[] rgpidl;

	if ( FAILED( hr ) ) {
		_com_error error( hr );
		throw maschinen::exception::Exception( error.ErrorMessage(), hr );
	}

	DWORD attributes_to = GetFileAttributes( to );
	if ( attributes_to == INVALID_FILE_ATTRIBUTES ) {
		TCHAR error[ MAX_LOADSTRING ];
		DWORD error_code;
		if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) )
			throw maschinen::exception::Exception( error, error_code );
		else throw maschinen::exception::Exception();
	}

	IShellItem* pis = NULL;

	hr = SHCreateItemFromParsingName( to, NULL, IID_PPV_ARGS( &pis ) );

	if ( FAILED( hr ) ) {
		_com_error error( hr );
		throw maschinen::exception::Exception( error.ErrorMessage(), hr );
	}

	hr = ifo->MoveItems( psia, pis );
	
	if ( FAILED( hr ) ) {
		_com_error error( hr );
		throw maschinen::exception::Exception( error.ErrorMessage(), hr );
	}

}

void maschinen::com::FileOperation::copy( const std::vector<tstring>* const froms, LPCTSTR to ) const {
	IShellItemArray* psia = NULL;

	PIDLIST_ABSOLUTE* rgpidl = new( std::nothrow ) PIDLIST_ABSOLUTE[ froms->size() ];
	HRESULT hr = rgpidl ? S_OK : E_OUTOFMEMORY;

	if( FAILED(hr) ) {
		_com_error error( hr );
		throw maschinen::exception::Exception( error.ErrorMessage(), hr );
	}

	size_t counter = 0;
	for ( ; counter < froms->size(); counter++ ) {
		hr = SHParseDisplayName( ( *froms )[ counter ].c_str(), nullptr, &rgpidl[ counter ], 0, nullptr );
		if( FAILED(hr) ) {
			for ( size_t i = 0; i < counter; i++ ) CoTaskMemFree( rgpidl[ i ] );
			delete[] rgpidl;
			_com_error error( hr );
			throw maschinen::exception::Exception( error.ErrorMessage(), hr );
		}
	}
	hr = SHCreateShellItemArrayFromIDLists( ( UINT ) counter, ( LPCITEMIDLIST * ) rgpidl, &psia );
	for ( size_t i = 0; i < froms->size(); i++ ) {
		CoTaskMemFree( rgpidl[ i ] );
	}
	delete[] rgpidl;
	if ( FAILED( hr ) ) {
		_com_error error( hr );
		throw maschinen::exception::Exception( error.ErrorMessage(), hr );
	}
	DWORD attributes_to = GetFileAttributes( to );
	if ( attributes_to == INVALID_FILE_ATTRIBUTES ) {
		TCHAR error[ MAX_LOADSTRING ];
		DWORD error_code;
		if ( getLastErrorAsString( error, MAX_LOADSTRING, error_code ) )
			throw maschinen::exception::Exception( error, error_code );
		else throw maschinen::exception::Exception();
	}
	IShellItem* pis = NULL;
	hr = SHCreateItemFromParsingName( to, NULL, IID_PPV_ARGS( &pis ) );
	if ( FAILED( hr ) ) {
		_com_error error( hr );
		throw maschinen::exception::Exception( error.ErrorMessage(), hr );
	}
	hr = ifo->CopyItems( psia, pis );
	if ( FAILED( hr ) ) {
		_com_error error( hr );
		throw maschinen::exception::Exception( error.ErrorMessage(), hr );
	}
}

void maschinen::com::FileOperation::commit() const {
	if ( ifo != nullptr ) {
		HRESULT hr = ifo->PerformOperations();
		if(FAILED(hr)  ){
			OutputDebugStringW( to_tstring( hr ).c_str() );
			_com_error error( hr );
			throw maschinen::exception::Exception( error.ErrorMessage(), hr );
		}
	}
	else throw maschinen::exception::Exception( TEXT( "Operation Cannot Be Completed" ), ERROR_CAN_NOT_COMPLETE );
}

maschinen::com::FileOperation::~FileOperation() {
	if ( ifo != nullptr ) ifo->Release();
}
