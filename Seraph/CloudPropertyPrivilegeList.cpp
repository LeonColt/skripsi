#include "stdafx.h"
#include "CloudPropertyPrivilegeList.h"


CloudPropertyPrivilegeList::CloudPropertyPrivilegeList() : TYPE_SEEN(1), TYPE_DOWNLOAD(2), TYPE_UPLOAD_ON_DIRECTORY(3) {
	cloud = nullptr;
	type = TYPE_SEEN;
}

void CloudPropertyPrivilegeList::onCreate() {
	label_id_username_email_added.setParentAndId( this, LABEL_CLOUD_PROPERTY_UPLOADER_LIST_NAME_OR_ID_ADDED );
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &label_id_username_email_added, 0, 0, 0, 10, 2, 0 );
	label_id_username_email_added.create();
	label_id_username_email_added.setText( TEXT( "User Id or Username or Email to be added : " ) );

	edit_id_username_email_added.setParentAndId( this, EDIT_CLOUD_PROPERTY_UPLOADER_LIST_NAME_OR_ID_ADDEDD );
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &edit_id_username_email_added, 11, 0, 0, 20, 2, 0 );
	edit_id_username_email_added.create();

	btt_add_user_privilege.setParentAndId( this, BUTTON_CLOUD_PROPERTY_PRIVILEGE_LIST_ADD_USER );
	( ( maschinen::layout::LayoutMatrix* ) getLayout() )->addController( &btt_add_user_privilege, 33, 0, 0, 8, 2, 0 );
	btt_add_user_privilege.create();
	btt_add_user_privilege.setText( TEXT( "Add User" ) );
	btt_add_user_privilege.setOnClickListener( this );

	lv = std::make_unique<maschinen::list_view::Table>();
	lv->setParentAndId( this, LIST_VIEW_CLOUD_PROPERTY_DOWNLOAD_LIST );
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( lv.get(), 0, 20, 0, 49, 29, 0 );
	if ( maschinen::list_view::Table* table = dynamic_cast<maschinen::list_view::Table*>( lv.get() ) ) {
		table->applyFullRowSelect();
		table->applyShowGridLines();
		table->applyHeaderDragDrop();
		table->autoWidth();
		lv->create();
		table->addColumn( TEXT( "ID" ) );
		table->addColumn( TEXT( "Username" ) );
	}

	lister = std::make_unique<GetPrivilegeList>();
	lister->setAkey( akey );
	lister->setSetting( setting );
	lister->setUserCredential( uc );
	lister->setErrorMarker( error_marker );
	lister->setParent( this );
	lister->setCloud( cloud );
	if ( type == TYPE_DOWNLOAD ) lister->setTypeDownload();
	else if ( type == TYPE_UPLOAD_ON_DIRECTORY ) lister->setTypeUploadOnDirectory();
	else lister->setTypeSeen();
	lister->execute();
}

void CloudPropertyPrivilegeList::onDestroy() {
	lister->stop();
	while ( lister->isAlive() ) std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
	CustomContainer::onDestroy();
}

void CloudPropertyPrivilegeList::onClick( maschinen::event::OnClickEvent& e ) {
	SetPrivilegeUserOnFile* spuof = new SetPrivilegeUserOnFile();
	spuof->setParent( this );
	spuof->setAutoDelete();
	spuof->setAkey( akey );
	spuof->setSetting( setting );
	spuof->setUserCredential( uc );
	spuof->setCloud( cloud );
	spuof->setUserIdUsernameEmail( edit_id_username_email_added.getText()->c_str() );
	spuof->setErrorMarker( error_marker );
	if ( type == TYPE_DOWNLOAD ) spuof->setTypeDownload();
	else if ( type == TYPE_UPLOAD_ON_DIRECTORY ) setTypeUploadOnDirectory();
	else spuof->setTypeSeen();
	spuof->execute();
}

void CloudPropertyPrivilegeList::setCloud( Cloud* cloud ) {
	this->cloud = cloud;
}

void CloudPropertyPrivilegeList::setUsers( std::vector<std::unique_ptr<User>>::const_iterator& begin, std::vector<std::unique_ptr<User>>::const_iterator& end ) {
	if ( maschinen::list_view::Table* table = dynamic_cast<maschinen::list_view::Table*>( lv.get() ) ) {
		std::vector<QWORD> existing;
		existing.reserve( table->getRowCount() );
		int row_counter = ( int ) ( ( maschinen::list_view::Table* )lv.get() )->getRowCount();
		for ( auto i = begin; i != end; ++i ) {
			int row_exists = -1;
			for ( auto j = 0; j < ( int ) table->getRowCount(); j++ ) {
				if ( _tcstoui64( table->getText( j, 0 )->c_str(), nullptr, 10 ) == ( *i )->getId() ) {
					row_exists = j; break;
				}
			}
			if ( row_counter > -1 ) {
				table->addRow();
				table->setText( to_tstring( ( *i )->getId() ).c_str(), row_counter, 0 );
				table->setText( ( *i )->getUsername()->c_str(), row_counter, 1 );
				existing.push_back( ( *i )->getId() );
				row_counter++;
			}
			else {
				table->setText( to_tstring( ( *i )->getId() ).c_str(), row_exists, 0 );
				table->setText( ( *i )->getUsername()->c_str(), row_exists, 1 );
			}
		}
	}
}


CloudPropertyPrivilegeList::~CloudPropertyPrivilegeList() {
}

void CloudPropertyPrivilegeList::setTypeSeen() {
	type = TYPE_SEEN;
}
void CloudPropertyPrivilegeList::setTypeDownload(){
	type = TYPE_DOWNLOAD;
}
void CloudPropertyPrivilegeList::setTypeUploadOnDirectory(){
	type = TYPE_UPLOAD_ON_DIRECTORY;
}