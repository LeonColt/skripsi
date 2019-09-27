#include "stdafx.h"
#include "ComponentObjectModel.h"


maschinen::com::ComponentObjectModel::ComponentObjectModel( const DWORD co_init ) {
	init_res = CoInitializeEx( NULL, co_init );
}

HRESULT maschinen::com::ComponentObjectModel::openFolderAndSelectItems( LPCTSTR path, const std::vector<tstring>* const files ) const {
	HRESULT hr = E_FAIL;
	if ( GetFileAttributes( path ) == INVALID_FILE_ATTRIBUTES ) throw maschinen::exception::Exception( L"path is not valid", 0 );
	ITEMIDLIST* pidl = ILCreateFromPath( path );
	ITEMIDLIST** pfiles = new ITEMIDLIST*[ files->size() ];
	size_t indexer = 0;
	for ( auto i = files->cbegin(); i != files->cend(); i++ ) {
		pfiles[ indexer ] = ILCreateFromPath( i->c_str() );
		indexer++;
	}
	hr = SHOpenFolderAndSelectItems( pidl, (UINT)files->size(), (LPCITEMIDLIST*)pfiles, 0 );
	ILFree( pidl );
	for ( size_t i = 0; i < files->size(); i++ ) {
		ILFree( pfiles[ indexer ] );
	}
	return E_FAIL;
}

HRESULT maschinen::com::ComponentObjectModel::openFolderAndSelectItem( LPCTSTR path ) const {
	HRESULT hr = E_FAIL;
	ITEMIDLIST* pidl = ILCreateFromPath( path );
	if ( pidl ) {
		hr = SHOpenFolderAndSelectItems( pidl, 0, 0, 0 );
		ILFree( pidl );
		return hr;
	}
	return E_FAIL;
}

maschinen::com::ComponentObjectModel::~ComponentObjectModel() {
	if ( SUCCEEDED( init_res ) ) CoUninitialize();
}
