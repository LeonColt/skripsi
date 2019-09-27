#include "stdafx.h"
#include "CloudRecycleBinPropertyAbout.h"


CloudRecycleBinPropertyAbout::CloudRecycleBinPropertyAbout() {
}

void CloudRecycleBinPropertyAbout::onCreate() {
	/*
	label_name.setParentAndId( this, LABEL_LOCAL_PROPERTY_ABOUT_NAME );
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &label_name, 0, 0, 0, 5, 2, 0 );
	label_name.create();
	label_name.setText( TEXT( "Name : " ) );

	edit_name.setParentAndId( this, EDIT_LOCAL_PROPERTY_ABOUT_NAME );
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &edit_name, 6, 0, 0, 20, 2, 0 );
	edit_name.create();
	edit_name.setText( local->getName()->c_str() );

	label_owner_var.setParentAndId( this, LABEL_LOCAL_PROPERTY_ABOUT_OWNER_VAR );
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &label_owner_var, 0, 6, 0, 5, 2, 0 );
	label_owner_var.create();
	label_owner_var.setText( TEXT( "Owner : " ) );

	label_owner_val.setParentAndId( this, LABEL_LOCAL_PROPERTY_ABOUT_OWNER_VAL );
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &label_owner_val, 6, 6, 0, 5, 2, 0 );
	label_owner_val.create();
	label_owner_val.setText( local->getOwner()->c_str() );

	label_parent_var.setParentAndId( this, LABEL_CLOUD_PROPERTY_ABOUT_PARENT_VAR );
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &label_parent_var, 0, 18, 0, 5, 2, 0 );
	label_parent_var.create();
	label_parent_var.setText( TEXT( "Parent : " ) );

	label_parent_val.setParentAndId( this, LABEL_CLOUD_PROPERTY_ABOUT_PARENT_VAL );
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &label_parent_val, 6, 18, 0, 5, 2, 0 );
	label_parent_val.create();
	label_parent_val.setText( ( local->getParent() == nullptr ) ? TEXT( "No Parent" ) : ( local->getParent()->getName()->c_str() ) );
	*/
}

void CloudRecycleBinPropertyAbout::setCloudRecycleBinObject( CloudRecycleBinObject* crbo ) {this->crbo = crbo;}

CloudRecycleBinPropertyAbout::~CloudRecycleBinPropertyAbout() {
}
