#include "stdafx.h"
#include "CloudRecycleBinProperty.h"


CloudRecycleBinProperty::CloudRecycleBinProperty() {
}

void CloudRecycleBinProperty::onClick( maschinen::event::OnClickEvent& e ) {
	if ( e.getSourceId() == btt_about.getId() ) {
		btt_about.hide();
		about->show();
	}
}

void CloudRecycleBinProperty::onCreate() {
	btt_about.setParentAndId( this, BUTTON_CLOUD_RECYCLE_BIN_PROPERTY_ABOUT );
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &btt_about, 0, 0, 0, 5, 3, 0 );
	btt_about.create();
	btt_about.setText( TEXT( "About" ) );
	btt_about.setOnClickListener( this );

	about = std::make_unique<CloudRecycleBinPropertyAbout>();
	about->setParentAndId( this, CLOUD_RECYCLE_BIN_PROPERTY_ABOUT );
	about->setCloudRecycleBinObject( crbo );
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

CloudRecycleBinProperty::~CloudRecycleBinProperty() {
}

void CloudRecycleBinProperty::setCloudRecycleBinObject( CloudRecycleBinObject* crbo ) {
	this->crbo = crbo;
}
