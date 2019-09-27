#include "stdafx.h"
#include "LocalPropertyAbout.h"


LocalPropertyAbout::LocalPropertyAbout() {
}

void LocalPropertyAbout::onCreate() {
	label_name.setParentAndId( this, LABEL_LOCAL_PROPERTY_ABOUT_NAME );
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &label_name, 0, 0, 0, 5, 2, 0 );
	label_name.create();
	label_name.setText( TEXT( "Name : " ) );

	edit_name.setParentAndId( this, EDIT_LOCAL_PROPERTY_ABOUT_NAME );
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &edit_name, 6, 0, 0, 20, 2, 0 );
	edit_name.create();
	edit_name.setText( local->getName()->c_str() );

	label_owner_var.setParentAndId( this, LABEL_LOCAL_PROPERTY_ABOUT_OWNER_VAR );
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &label_owner_var, 0, 3, 0, 20, 2, 0 );
	label_owner_var.create();
	label_owner_var.setText( TEXT( "Owner : " ) );

	label_owner_val.setParentAndId( this, LABEL_LOCAL_PROPERTY_ABOUT_OWNER_VAL );
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &label_owner_val, 6, 3, 0, 20, 2, 0 );
	label_owner_val.create();
	label_owner_val.setText( local->getOwner()->c_str() );

	label_path_var.setParentAndId( this, LABEL_LOCAL_PROPERTY_ABOUT_PATH_VAR );
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &label_path_var, 0, 6, 0, 5, 2, 0 );
	label_path_var.create();
	label_path_var.setText( TEXT( "Path : " ) );

	label_path_val.setParentAndId( this, LABEL_LOCAL_PROPERTY_ABOUT_PATH_VAL );
	( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &label_path_val, 6, 6, 0, 20, 2, 0 );
	label_path_val.create();
	label_path_val.setText( local->getPath()->c_str() );

	if( FileLocal* fl = dynamic_cast<FileLocal*>( local ) ) {
		date_created_var.setParentAndId( this, LABEL_LOCAL_PROPERTY_ABOUT_DATE_CREATED_VAR );
		( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &date_created_var, 0, 9, 0, 5, 2, 0 );
		date_created_var.create();
		date_created_var.setText( TEXT( "Created : " ) );

		date_created_val.setParentAndId( this, LABEL_LOCAL_PROPERTY_ABOUT_DATE_CREATED_VAL );
		( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &date_created_val, 6, 9, 0, 20, 2, 0 );
		date_created_val.create();

		SYSTEMTIME create_time;
		FileTimeToSystemTime( fl->getCreatedTime().get(), &create_time );
		date_created_val.setText( extractTimes( &create_time )->c_str() );

		last_access_time_var.setParentAndId( this, LABEL_LOCAL_PROPERTY_ABOUT_LAST_ACCESS_TIME_VAR );
		( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &last_access_time_var, 0, 12, 0, 5, 2, 0 );
		last_access_time_var.create();
		last_access_time_var.setText( TEXT( "Last Access Time : " ) );

		last_access_time_val.setParentAndId( this, LABEL_LOCAL_PROPERTY_ABOUT_LAST_ACCESS_TIME_VAL );
		( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &last_access_time_val, 6, 12, 0, 20, 2, 0 );
		last_access_time_val.create();

		SYSTEMTIME last_access_time;
		FileTimeToSystemTime( fl->getLastAccessTime().get(), &last_access_time );
		last_access_time_val.setText( extractTimes( &last_access_time )->c_str() );

		last_write_time_var.setParentAndId( this, LABEL_LOCAL_PROPERTY_ABOUT_LAST_WRITE_TIME_VAR );
		( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &last_write_time_var, 0, 15, 0, 5, 2, 0 );
		last_write_time_var.create();
		last_write_time_var.setText( TEXT( "Last Write Time : " ) );

		last_write_time_val.setParentAndId( this, LABEL_LOCAL_PROPERTY_ABOUT_LAST_WRITE_TIME_VAL );
		( ( maschinen::layout::LayoutMatrix* )getLayout() )->addController( &last_write_time_val, 6, 15, 0, 20, 2, 0 );
		last_write_time_val.create();
		SYSTEMTIME last_write_time;
		FileTimeToSystemTime( fl->getLastWriteTime().get(), &last_write_time );
		last_write_time_val.setText( extractTimes( &last_write_time )->c_str() );
	}
}

std::unique_ptr<tstring> LocalPropertyAbout::extractTimes( LPSYSTEMTIME time ) {
	std::unique_ptr<tstring> waktu = std::make_unique<tstring>();
	waktu->append( to_tstring( time->wDayOfWeek ).c_str() );
	waktu->append( TEXT( " " ) );
	switch ( time->wMonth ) {
		case 1 : waktu->append( TEXT( "January" ) ); break;
		case 2 : waktu->append( TEXT( "February" ) ); break;
		case 3 : waktu->append( TEXT( "March" ) ); break;
		case 4 : waktu->append( TEXT( "April" ) ); break;
		case 5 : waktu->append( TEXT( "May" ) ); break;
		case 6 : waktu->append( TEXT( "June" ) ); break;
		case 7 : waktu->append( TEXT( "July" ) ); break;
		case 8 : waktu->append( TEXT( "August" ) ); break;
		case 9 : waktu->append( TEXT( "September" ) ); break;
		case 10: waktu->append( TEXT( "October" ) ); break;
		case 11: waktu->append( TEXT( "November" ) ); break;
		case 12: waktu->append( TEXT( "December" ) ); break;
	}
	waktu->append( TEXT( " " ) );
	waktu->append( to_tstring( time->wYear ).c_str() );
	waktu->append( TEXT( ", " ) );
	waktu->append( to_tstring( time->wHour ).c_str() );
	waktu->append( TEXT( ":" ) );
	waktu->append( to_tstring( time->wMinute ).c_str() );
	waktu->append( TEXT( ":" ) );
	waktu->append( to_tstring( time->wSecond ).c_str() );
	return waktu;
}

LocalPropertyAbout::~LocalPropertyAbout() {}

void LocalPropertyAbout::setLocal( Local* local ) {
	this->local = local;
}