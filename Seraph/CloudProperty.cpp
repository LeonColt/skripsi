#include "stdafx.h"
#include "CloudProperty.h"


CloudProperty::CloudProperty() {
}

void CloudProperty::onClick( maschinen::event::OnClickEvent& e ) {
	switch ( e.getSourceId() ) {
		case BUTTON_CLOUD_PROPERTY_ABOUT : {
			btt_about.disable();
			btt_seen_list.enable();
			btt_downloader_list.enable();
			btt_uploader_list.enable();
			about->show();
			seen_list->hide();
			downloader_list->hide();
			uploader_list->hide();

		} break;

		case BUTTON_CLOUD_PROPERTY_SEEN_LIST : {
			btt_about.enable();
			btt_seen_list.disable();
			btt_downloader_list.enable();
			btt_uploader_list.enable();
			about->hide();
			seen_list->show();
			downloader_list->hide();
			uploader_list->hide();
		} break;

		case BUTTON_CLOUD_PROPERTY_DOWNLOADER_LIST : {
			btt_about.enable();
			btt_seen_list.enable();
			btt_downloader_list.disable();
			btt_uploader_list.enable();
			about->hide();
			seen_list->hide();
			downloader_list->show();
			uploader_list->hide();
		} break;

		case BUTTON_CLOUD_PROPERTY_UPLOAD_LIST : {
			btt_about.enable();
			btt_seen_list.enable();
			btt_downloader_list.enable();
			btt_uploader_list.disable();
			about->hide();
			seen_list->hide();
			downloader_list->hide();
			uploader_list->show();
		} break;
	}
}

void CloudProperty::onCreate() {
	btt_about.setParentAndId( this, BUTTON_CLOUD_PROPERTY_ABOUT );
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &btt_about, 0, 0, 0, 5, 3, 0 );
	btt_about.create();
	btt_about.setText( TEXT( "About" ) );
	btt_about.setOnClickListener( this );

	btt_seen_list.setParentAndId( this, BUTTON_CLOUD_PROPERTY_SEEN_LIST );
	( ( maschinen::layout::LayoutMatrix* ) getLayout() )->addController( &btt_seen_list, 6, 0, 0, 5, 3, 0 );
	btt_seen_list.create();
	btt_seen_list.setText( TEXT( "View Seen List" ) );
	btt_seen_list.setOnClickListener( this );

	btt_downloader_list.setParentAndId( this, BUTTON_CLOUD_PROPERTY_DOWNLOADER_LIST );
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &btt_downloader_list, 12, 0, 0, 5, 3, 0 );
	btt_downloader_list.create();
	btt_downloader_list.setText( TEXT( "View Downloader List" ) );
	btt_downloader_list.setOnClickListener( this );

	btt_uploader_list.setParentAndId( this, BUTTON_CLOUD_PROPERTY_UPLOAD_LIST );
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &btt_uploader_list, 18, 0, 0, 5, 3, 0 );
	btt_uploader_list.create();
	btt_uploader_list.setText( TEXT( "View Uploader List" ) );
	btt_uploader_list.setOnClickListener( this );

	about = std::make_unique<CloudPropertyAbout>();
	about->setParentAndId( this, CLOUD_PROPERTY_ABOUT );
	about->setCloud( cloud );
	maschinen::layout::LayoutMatrix* layout_about = new maschinen::layout::LayoutMatrix();
	( ( maschinen::layout::LayoutMatrix* ) getLayout() )->addController( about.get(), 0, 4, 0, 49, 45, 0 );
	layout_about->setNumberOfCell( 50, 50 );
	about->setLayout( layout_about );
	about->setAkey( akey );
	about->setSetting( setting );
	about->setUserCredential( user_credential );
	about->create();
	btt_about.disable();

	seen_list = std::make_unique<CloudPropertyPrivilegeList>();
	seen_list->setParentAndId( this, CLOUD_PROPERTY_SEEN_LIST );
	seen_list->setCloud( cloud );
	maschinen::layout::LayoutMatrix* layout_seen_list = new maschinen::layout::LayoutMatrix();
	( ( maschinen::layout::LayoutMatrix* ) getLayout() )->addController( seen_list.get(), 0, 4, 0, 49, 45, 0 );
	layout_seen_list->setNumberOfCell( 50, 50 );
	seen_list->setLayout( layout_seen_list );
	seen_list->setAkey( akey );
	seen_list->setSetting( setting );
	seen_list->setUserCredential( user_credential );
	seen_list->setTypeSeen();
	seen_list->create();

	downloader_list = std::make_unique<CloudPropertyPrivilegeList>();
	downloader_list->setParentAndId( this, CLOUD_PROPERTY_DOWNLOAD_LIST );
	downloader_list->setCloud( cloud );
	maschinen::layout::LayoutMatrix* layout_downloader_list = new maschinen::layout::LayoutMatrix();
	( ( maschinen::layout::LayoutMatrix* ) getLayout() )->addController( downloader_list.get(), 0, 4, 0, 49, 45, 0 );
	layout_downloader_list->setNumberOfCell( 50, 50 );
	downloader_list->setLayout( layout_downloader_list );
	downloader_list->setAkey( akey );
	downloader_list->setSetting( setting );
	downloader_list->setUserCredential( user_credential );
	downloader_list->setTypeDownload();
	downloader_list->create();

	uploader_list = std::make_unique<CloudPropertyPrivilegeList>();
	uploader_list->setParentAndId( this, CLOUD_PROPERTY_UPLOADER_LIST );
	uploader_list->setCloud( cloud );
	maschinen::layout::LayoutMatrix* layout_uploader_list = new maschinen::layout::LayoutMatrix();
	( ( maschinen::layout::LayoutMatrix* ) getLayout() )->addController( uploader_list.get(), 0, 4, 0, 49, 45, 0 );
	layout_uploader_list->setNumberOfCell( 50, 50 );
	uploader_list->setLayout( layout_uploader_list );
	uploader_list->setAkey( akey );
	uploader_list->setSetting( setting );
	uploader_list->setUserCredential( user_credential );
	uploader_list->setTypeUploadOnDirectory();
	uploader_list->create();
}

void CloudProperty::onShow() {
	Property::onShow();
	about->show();
	seen_list->hide();
	downloader_list->hide();
	uploader_list->hide();
}

void CloudProperty::onClose() {
	Property::onClose();
	fc->deleteProperty( this );
}

void CloudProperty::setFileControl( FileControl* fc ) {
	this->fc = fc;
}

void CloudProperty::setCloud( Cloud* cloud ) {
	this->cloud = cloud;
}

CloudProperty::~CloudProperty() {
	destroy();
}
