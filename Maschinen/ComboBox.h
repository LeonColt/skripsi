#pragma once
#include "UncontainedController.h"
#include "Exception.h"
namespace maschinen {
	class MASCHINEN_API ComboBox : public maschinen::UncontainedController {
	public:
		ComboBox();
		void create();
		LONGLONG addText(LPCTSTR string);
		void setText( LONGLONG index, LPCTSTR string ) const;
		std::unique_ptr<tstring> getText( LONGLONG index ) const;
		LONGLONG setCurrentSelection( LONGLONG index ) const;
		LONGLONG setCurrentSelectionText(LPCTSTR string) const;
		void clearSelection() const;
		LONGLONG getCurrentSelection() const;
		std::unique_ptr<tstring> getCurrentSelectionText() const;
		LONGLONG getItemsCount() const;
		/*Automatically scrolls the text in an edit control to the right when the user types a character at the end of the line. If this style is not set, only text that fits within the rectangular boundary is allowed.*/
		void styleAutoHorizontalScroll( bool style = true );
		/*Shows a disabled vertical scroll bar in the list box when the box does not contain enough items to scroll. Without this style, the scroll bar is hidden when the list box does not contain enough items.*/
		void styleDisableNoScroll( bool style = true );
		/*Similar to CBS_SIMPLE, except that the list box is not displayed unless the user selects an icon next to the edit control.*/
		void styleDropDown( bool style = true );
		void styleDropDownList( bool style = true );
		void styleHasStrings( bool style = true );
		void styleLowerCase( bool style = true );
		void styleNoIntegralHeight( bool style = true );
		void styleOEMConvert( bool style = true );
		void styleOwnerDrawFixed( bool style = true );
		void styleSimple( bool style = true );
		void styleSort( bool style = true );
		void styleUpperCase( bool style = true );
		bool operator==( int type ) const noexcept;
		bool operator!=( int type ) const noexcept;
		bool compare( int type ) const noexcept;
		~ComboBox();
	};
}

