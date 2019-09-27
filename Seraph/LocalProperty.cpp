#include "stdafx.h"
#include "LocalProperty.h"


LocalProperty::LocalProperty() {
}

void LocalProperty::onClick( maschinen::event::OnClickEvent& e ) {
	if( e.getSourceId() == btt_about.getId() ) {
		btt_about.hide();
		about->show();
	}
}

void LocalProperty::setFileControl( FileControl* fc ){this->fc = fc;}
void LocalProperty::setLocal( Local* local ) {this->local = local;}

void LocalProperty::onCreate() {
	btt_about.setParentAndId( this, BUTTON_LOCAL_PROPERTY_ABOUT );
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &btt_about, 0, 0, 0, 5, 3, 0 );
	btt_about.create();
	btt_about.setText( TEXT( "About" ) );
	btt_about.setOnClickListener( this );

	about = std::make_unique<LocalPropertyAbout>();
	about->setParentAndId( this, LOCAL_PROPERTY_ABOUT );
	about->setLocal( local );
	maschinen::layout::LayoutMatrix* layout_about = new maschinen::layout::LayoutMatrix();
	( ( maschinen::layout::LayoutMatrix* ) getLayout() )->addController( about.get(), 0, 4, 0, 49, 45, 0 );
	layout_about->setNumberOfCell( 50, 50 );
	about->setLayout( layout_about );
	about->setAkey( akey );
	about->setSetting( setting );
	about->setUserCredential( user_credential );
	about->create();
	btt_about.disable();
}

LocalProperty::~LocalProperty() {
}
