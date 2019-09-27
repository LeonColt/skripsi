#include "stdafx.h"
#include "LayoutMatrix.h"


maschinen::layout::LayoutMatrix::LayoutMatrix() {
}
void maschinen::layout::LayoutMatrix::calculate(LONG x, LONG y, LONG width, LONG height) {
	//should throw exception
	RECT rect;
	ZeroMemory( &rect, sizeof( RECT ) );
	rect.left = x;
	rect.top = y;
	rect.right = x + width;
	rect.bottom = y + height;
	calculate( &rect );
}

void maschinen::layout::LayoutMatrix::calculate( LPRECT rect) {
	//should throw exception
	cell2d.clear();
	LONG panjang_window = rect->right - rect->left;
	LONG lebar_window = rect->bottom - rect->top;
	if ( panjang_window == 0 || lebar_window == 0 || number_of_cell_per_width == 0 || number_of_cell_per_height == 0 ) return;
	LONG panjang_cell = panjang_window / number_of_cell_per_width;
	LONG lebar_cell = lebar_window / number_of_cell_per_height;
	LONG x_kiri = rect->left,
		x_kanan = x_kiri + panjang_cell,
		y_atas = rect->top,
		y_bawah = y_atas + lebar_cell;
	for ( LONG y = 0; y < number_of_cell_per_height; y++ ) {
		std::vector<Cell> cells;
		cells.reserve( number_of_cell_per_width );
		for ( LONG x = 0; x < number_of_cell_per_width; x++ ) {
			Cell cell;
			if ( x == number_of_cell_per_width - 1 && y == number_of_cell_per_height - 1 ) {
				cell.atas.kiri.x = x_kiri; cell.atas.kiri.y = y_atas; cell.atas.kanan.x = rect->right; cell.atas.kanan.y = y_atas;
				cell.bawah.kiri.x = x_kiri; cell.bawah.kiri.y = rect->bottom; cell.bawah.kanan.x = rect->right; cell.bawah.kanan.y = rect->bottom;
			}
			else if ( x == number_of_cell_per_width - 1 ) {
				cell.atas.kiri.x = x_kiri; cell.atas.kiri.y = y_atas; cell.atas.kanan.x = rect->right; cell.atas.kanan.y = y_atas;
				cell.bawah.kiri.x = x_kiri; cell.bawah.kiri.y = y_bawah; cell.bawah.kanan.x = rect->right; cell.bawah.kanan.y = y_bawah;
			}
			else if ( y == number_of_cell_per_height - 1 ) {
				cell.atas.kiri.x = x_kiri; cell.atas.kiri.y = y_atas; cell.atas.kanan.x = x_kanan; cell.atas.kanan.y = y_atas;
				cell.bawah.kiri.x = x_kiri; cell.bawah.kiri.y = rect->bottom; cell.bawah.kanan.x = x_kanan; cell.bawah.kanan.y = rect->bottom;
			}
			else {
				cell.atas.kiri.x = x_kiri; cell.atas.kiri.y = y_atas; cell.atas.kanan.x = x_kanan; cell.atas.kanan.y = y_atas;
				cell.bawah.kiri.x = x_kiri; cell.bawah.kiri.y = y_bawah; cell.bawah.kanan.x = x_kanan; cell.bawah.kanan.y = y_bawah;
			}
			cells.push_back( cell );
			x_kiri = x_kanan;
			x_kanan = x_kiri + panjang_cell;
		}
		cell2d.push_back( cells );
		y_atas = y_bawah;
		y_bawah = y_atas + lebar_cell;
		x_kiri = rect->left;
		x_kanan = x_kiri + panjang_cell;
	}
	for ( auto i = cell2d.begin(); i != cell2d.end(); i++ )
		if ( i->size() != number_of_cell_per_width ) throw maschinen::exception::Exception( L"control position violance", 0 );
}

void maschinen::layout::LayoutMatrix::calculate() {
	RECT rect;
	ZeroMemory( &rect, sizeof( RECT ) );
	GetClientRect( owner->getHandler(), &rect );
	calculate( &rect );
}

std::unique_ptr<Position> maschinen::layout::LayoutMatrix::getCoordinate( LONG x, LONG y, LONG consume_top, LONG consume_right, LONG consume_bottom, LONG consume_left ) const {
	std::unique_ptr<Position> pos = std::make_unique<Position>();
	ZeroMemory( pos.get(), sizeof( Position ) );
	LONG temp_x_kiri = ( consume_left == FILL_PARENT ) ? ( 0 ) : ( x - consume_left );
	LONG temp_x_kanan = ( consume_right == FILL_PARENT ) ? ( number_of_cell_per_width - 1 ) : ( x + consume_right );
	LONG temp_y_atas = ( consume_top == FILL_PARENT ) ? ( 0 ) : y - consume_top;
	LONG temp_y_bawah = ( consume_bottom == FILL_PARENT ) ? ( number_of_cell_per_height - 1 ) : ( y + consume_bottom );

	//if (temp_x_kiri < 0) temp_x_kiri = 0;
	//if (temp_x_kiri > number_of_cell_per_width - 1) temp_x_kiri = number_of_cell_per_width - 2;
	//if (temp_x_kanan > number_of_cell_per_width -1) temp_x_kanan = number_of_cell_per_width -1;
	//if (temp_y_atas < 0) temp_y_atas = 0;
	//if (temp_y_bawah > number_of_cell_per_height - 1) temp_y_bawah = number_of_cell_per_height -1;

	if ( temp_x_kiri < 0 || temp_x_kiri > number_of_cell_per_width - 1
		 || temp_x_kanan < 0 || temp_x_kiri > number_of_cell_per_width - 1
		 || temp_y_atas < 0 || temp_y_bawah > number_of_cell_per_height - 1
		 || temp_y_bawah < 0 || temp_y_bawah > number_of_cell_per_height - 1
		 || temp_x_kiri > temp_x_kanan || temp_y_atas > temp_y_bawah
		 || cell2d.size() != number_of_cell_per_height ) {
#ifdef _DEBUG 
		TCHAR buffer[ MAX_LOADSTRING ]; OutputDebugStringW( L"control position violation data : " ); OutputDebugStringW( L"  temp_x_kiri : " ); _itow_s( temp_x_kiri, buffer, 10 ); OutputDebugStringW( buffer ); OutputDebugStringW( L"  temp_x_kanan : " ); _itow_s( temp_x_kanan, buffer, 10 ); OutputDebugStringW( buffer ); OutputDebugStringW( L"  temp_y_atas : " ); _itow_s( temp_y_atas, buffer, 10 ); OutputDebugStringW( buffer ); OutputDebugStringW( L"  temp_y_bawah : " ); _itow_s( temp_y_bawah, buffer, 10 ); OutputDebugStringW( buffer ); OutputDebugStringW( L" jumlahcellperpanjang : " ); _itow_s( number_of_cell_per_width, buffer, 10 ); OutputDebugStringW( buffer ); OutputDebugStringW( L" perlebar : " ); _itow_s( number_of_cell_per_height, buffer, 10 ); OutputDebugStringW( buffer ); OutputDebugStringW( L" cell2d_length : " ); _itow_s( ( int ) cell2d.size(), buffer, 10 ); OutputDebugStringW( buffer ); OutputDebugStringW( L"\n" );
#endif // _DEBUG
		throw maschinen::exception::Exception( L"control position violance", 0 );
	}
	pos->x = cell2d[ temp_y_atas ][ temp_x_kiri ].atas.kiri.x;
	pos->y = cell2d[ temp_y_atas ][ temp_x_kiri ].atas.kiri.y;
	pos->width = cell2d[ temp_y_atas ][ temp_x_kanan ].atas.kanan.x - cell2d[ temp_y_atas ][ temp_x_kiri ].atas.kiri.x;
	pos->height = cell2d[ temp_y_bawah ][ temp_x_kiri ].bawah.kiri.y - cell2d[ temp_y_atas ][ temp_x_kiri ].atas.kiri.y;
	return pos;
}
std::unique_ptr<Position> maschinen::layout::LayoutMatrix::getCoordinate( UINT id ) const {
	for ( auto i = ccis.begin(); i != ccis.end(); i++ ) {
		if ( ( *i )->id == id )
			return std::move(getCoordinate((*i)->X, ( *i )->Y, ( *i )->consume_top, ( *i )->consume_right, ( *i )->consume_buttom, ( *i )->consume_left));
	}
	std::unique_ptr<Position> pos = std::make_unique<Position>();
	ZeroMemory( pos.get(), sizeof( Position ) );
	return pos;
}

void maschinen::layout::LayoutMatrix::addController( maschinen::VisibleController* control, UINT x, UINT y, UINT consume_top, UINT consume_right, UINT consume_bottom, UINT consume_left ) noexcept {
	bool exists = false;
	for ( auto i = ccis.begin(); i != ccis.end(); i++ ) {
		if ( control->getId() == ( *i )->id ) {
			exists = true;
			( *i )->X = x;
			( *i )->Y = y;
			( *i )->consume_top = consume_top;
			( *i )->consume_right = consume_right;
			( *i )->consume_buttom = consume_bottom;
			( *i )->consume_left = consume_left;
		}
	}
	if ( !exists ) {
		CellControlInfo* cci = new CellControlInfo();
		cci->id = control->getId();
		cci->X = x;
		cci->Y = y;
		cci->consume_top = consume_top;
		cci->consume_right = consume_right;
		cci->consume_buttom = consume_bottom;
		cci->consume_left = consume_left;
		ccis.push_back( std::move( cci ) );
	}
}
void maschinen::layout::LayoutMatrix::setNumberOfCell( LONG per_width, LONG per_height ) noexcept 
{ 
	cell2d.reserve( per_height );
	this->number_of_cell_per_width = per_width; this->number_of_cell_per_height = per_height; 
}
LONG maschinen::layout::LayoutMatrix::getNumberOfCellPerWidth() const { return number_of_cell_per_width; }
LONG maschinen::layout::LayoutMatrix::getNumberOfCellPerHeight() const { return number_of_cell_per_height; }

maschinen::layout::LayoutMatrix::~LayoutMatrix() {
	for ( auto i = ccis.begin(); i != ccis.end(); i++ ) {
		delete *i;
	}
	ccis.clear();
}
