#pragma once
#include "Maschinen.h"
#include "Layout.h"
#include "VisibleController.h"
#include "ChildrenKeeper.h"

namespace maschinen {
	namespace layout {
		class MASCHINEN_API LayoutMatrix : public maschinen::layout::Layout {
			struct Cell {
				// koordinate cell yg diterapkan low = x, high = y
				//info koordinate cell
				//QWORD atas;
				struct DoublePosition {
					struct Koordinate {
						LONG x;
						LONG y;
					} kiri, kanan;
				} atas, bawah;
				// panjang cell = atas_kanan - atas_kiri || bawah_kanan - bawah_kiri
				//atas_kanan - atas_kiri == bawah_kanan - bawah_kiri
				//QWORD bawah;
				// lebar cell = atas_kiri - bawah_kiri || atas_kanan - bawah_kanan
				//atas_kiri - bawah_kiri == atas_kanan - bawah_kanan
				//diagonal jarang terpakai, gunakan phytagoras
			};
		private:
			LONG number_of_cell_per_width = 0, number_of_cell_per_height = 0;
			std::vector<std::vector<Cell>> cell2d;
			struct CellControlInfo {
				UINT id;
				UINT X;
				UINT Y;
				UINT consume_top;
				UINT consume_right;
				UINT consume_buttom;
				UINT consume_left;
			};
			std::vector<CellControlInfo*> ccis;
		public:
			LayoutMatrix();
			void calculate(LONG x, LONG y, LONG width, LONG height);
			void calculate(LPRECT rect);
			void calculate();
			std::unique_ptr<Position> getCoordinate( LONG x, LONG y, LONG consume_top, LONG consume_right, LONG consume_bottom, LONG consume_left ) const;
			std::unique_ptr<Position> getCoordinate( UINT id ) const;
			void addController(maschinen::VisibleController* control, UINT x, UINT y, UINT consume_top, UINT consume_right, UINT consume_bottom, UINT consume_left )  noexcept;
			void setNumberOfCell( LONG per_width, LONG per_height ) noexcept;
			LONG getNumberOfCellPerWidth() const;
			LONG getNumberOfCellPerHeight() const;
			~LayoutMatrix();
		};
	}
}

