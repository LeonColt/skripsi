#include "stdafx.h"
#include "CloudPropertyAbout.h"


CloudPropertyAbout::CloudPropertyAbout() {
}

void CloudPropertyAbout::onCheckedChanged( maschinen::CheckBox* check, bool in_check ) {
	MarkFileAsPrivilegeByAll* mfapba = new MarkFileAsPrivilegeByAll();
	mfapba->setCloud( cloud );
	mfapba->setAutoDelete( true );
	mfapba->setErrorMarker( error_marker );
	mfapba->setAkey( akey );
	mfapba->setSetting( setting );
	mfapba->setUserCredential( uc );
	switch ( check->getId() ) {
		case CHECK_CLOUD_PROPERTY_ABOUT_SEEN_BY_ALL : {
			check_seen_by_all.disable();
			mfapba->setTypeSeen();
		} break;

		case CHECK_CLOUD_PROPERTY_ABOUT_DOWNLOAD_BY_ALL: {
			check_download_by_all.disable();
			mfapba->setTypeDownload();
		} break;

		case CHECK_CLOUD_PROPERTY_ABOUT_MODIFY_BY_ALL: {
			check_modify_by_all.disable();
			mfapba->setTypeModify();
		} break;
	}
	mfapba->setParent( this );
	mfapba->execute();
}

void CloudPropertyAbout::onCreate() {
	label_id_var.setParentAndId( this, LABEL_CLOUD_PROPERTY_ABOUT_ID_VAR );
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &label_id_var, 0, 0, 0, 5, 2, 0 );
	label_id_var.create();
	label_id_var.setText( TEXT( "Id : " ) );

	label_id_val.setParentAndId( this, LABEL_CLOUD_PROPERTY_ABOUT_ID_VAL );
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &label_id_val, 6, 0, 0, 20, 2, 0 );
	label_id_val.create();
	label_id_val.setText( to_tstring(cloud->getId()).c_str() );

	label_name.setParentAndId( this, LABEL_CLOUD_PROPERTY_ABOUT_NAME );
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &label_name, 0, 3, 0, 5, 2, 0 );
	label_name.create();
	label_name.setText( TEXT( "Name : " ) );

	edit_name.setParentAndId( this, EDIT_CLOUD_PROPERTY_ABOUT_NAME );
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &edit_name, 6, 3, 0, 20, 2, 0 );
	edit_name.create();
	edit_name.setText( cloud->getName()->c_str() );

	label_seen_by_all.setParentAndId( this, LABEL_CLOUD_PROPERTY_ABOUT_SEEN_BY_ALL );
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &label_seen_by_all, 0, 6, 0, 5, 2, 0 );
	label_seen_by_all.create();
	label_seen_by_all.setText( TEXT( "Seen By All : " ) );

	check_seen_by_all.setParentAndId( this, CHECK_CLOUD_PROPERTY_ABOUT_SEEN_BY_ALL );
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &check_seen_by_all, 6, 6, 0, 5, 2, 0 );
	check_seen_by_all.create();
	check_seen_by_all.setText( TEXT( "Seen By All" ) );
	if ( cloud->isSeenByAll() ) check_seen_by_all.setChecked();
	check_seen_by_all.setOnCheckedChangeListener( this );

	label_download_by_all.setParentAndId( this, LABEL_CLOUD_PROPERTY_ABOUT_DOWNLOAD_BY_ALL );
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &label_download_by_all, 0, 9, 0, 5, 2, 0 );
	label_download_by_all.create();
	label_download_by_all.setText( TEXT( "Download By All : " ) );

	check_download_by_all.setParentAndId( this, CHECK_CLOUD_PROPERTY_ABOUT_DOWNLOAD_BY_ALL );
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &check_download_by_all, 6, 9, 0, 5, 2, 0 );
	check_download_by_all.create();
	check_download_by_all.setText( TEXT( "Download By All" ) );
	if ( cloud->isDownloadByAll() ) check_download_by_all.setChecked();
	check_download_by_all.setOnCheckedChangeListener( this );

	label_modify_by_all.setParentAndId( this, LABEL_CLOUD_PROPERTY_ABOUT_MODIFY_BY_ALL );
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &label_modify_by_all, 0, 12, 0, 5, 2, 0 );
	label_modify_by_all.create();
	label_modify_by_all.setText( TEXT( "Modify By All : " ) );

	check_modify_by_all.setParentAndId( this, CHECK_CLOUD_PROPERTY_ABOUT_MODIFY_BY_ALL );
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &check_modify_by_all, 6, 12, 0, 5, 2, 0 );
	check_modify_by_all.create();
	check_modify_by_all.setText( TEXT( "Modify By All" ) );
	if ( cloud->isModifyByAll() ) check_modify_by_all.setChecked();
	check_modify_by_all.setOnCheckedChangeListener( this );

	label_owner_var.setParentAndId( this, LABEL_CLOUD_PROPERTY_ABOUT_OWNER_VAR );
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &label_owner_var, 0, 15, 0, 5, 2, 0 );
	label_owner_var.create();
	label_owner_var.setText( TEXT( "Owner : " ) );

	label_owner_val.setParentAndId( this, LABEL_CLOUD_PROPERTY_ABOUT_OWNER_VAL );
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &label_owner_val, 6, 15, 0, 5, 2, 0 );
	label_owner_val.create();
	label_owner_val.setText( cloud->getOwner()->c_str() );

	label_parent_var.setParentAndId( this, LABEL_CLOUD_PROPERTY_ABOUT_PARENT_VAR );
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &label_parent_var, 0, 18, 0, 5, 2, 0 );
	label_parent_var.create();
	label_parent_var.setText( TEXT( "Parent : " ) );

	label_parent_val.setParentAndId( this, LABEL_CLOUD_PROPERTY_ABOUT_PARENT_VAL );
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &label_parent_val, 6, 18, 0, 5, 2, 0 );
	label_parent_val.create();
	//label_parent_val.setText( (cloud->getParent() == nullptr) ? TEXT("No Parent") : (cloud->getParent()->getName()->c_str()) );

}

void CloudPropertyAbout::setCloud( Cloud* cloud ) {
	this->cloud = cloud;
	wcscpy_s( error_marker, cloud->getName()->c_str() );
	wcscat_s( error_marker, TEXT( " Properites About" ) );
}

void CloudPropertyAbout::setSeenByAllEnableDisable( bool flag ){
	if ( flag ) check_seen_by_all.enable(); else check_seen_by_all.disable();
}
void CloudPropertyAbout::setDownloadByAllEnableDisable( bool flag ) {
	if ( flag ) check_download_by_all.enable(); else check_download_by_all.disable();
}
void CloudPropertyAbout::setModifyByAllEnableDisable( bool flag ) {
	if ( flag ) check_modify_by_all.enable(); else check_modify_by_all.disable();
}

void CloudPropertyAbout::setSeenByAllCheckUncheck( bool checked ){
	OutputDebugStringW( L"check uncheck seen by all" );
	check_seen_by_all.setChecked( checked );
}
void CloudPropertyAbout::setDownloadByAllCheckUncheck( bool checked ){
	check_download_by_all.setChecked( checked );
}
void CloudPropertyAbout::setModifyByAllCheckUncheck( bool checked ) {
	check_modify_by_all.setChecked( checked );
}


CloudPropertyAbout::~CloudPropertyAbout() {
}
