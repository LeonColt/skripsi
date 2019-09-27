#include "stdafx.h"
#include "SecurityAttributes.h"


maschinen::security::SecurityAttributes::SecurityAttributes() {
	ZeroMemory( &security_attributes, sizeof( SECURITY_ATTRIBUTES ) );
	security_attributes.nLength = sizeof( SECURITY_ATTRIBUTES );
}

void maschinen::security::SecurityAttributes::setInheritable( bool inheritable ) {
	if ( inheritable ) security_attributes.bInheritHandle = TRUE; else security_attributes.bInheritHandle = FALSE;
}

void maschinen::security::SecurityAttributes::set( SECURITY_ATTRIBUTES& security_attributes ) {
	this->security_attributes.bInheritHandle = security_attributes.bInheritHandle;
	this->security_attributes.lpSecurityDescriptor = security_attributes.lpSecurityDescriptor;
}
void maschinen::security::SecurityAttributes::set( LPSECURITY_ATTRIBUTES lp_security_attributes ) {
	this->security_attributes.bInheritHandle = lp_security_attributes->bInheritHandle;
	this->security_attributes.lpSecurityDescriptor = lp_security_attributes->lpSecurityDescriptor;
}
SECURITY_ATTRIBUTES& maschinen::security::SecurityAttributes::get() noexcept {
	return security_attributes;
}
LPSECURITY_ATTRIBUTES maschinen::security::SecurityAttributes::getLP() noexcept {
	return &security_attributes;
}


maschinen::security::SecurityAttributes::~SecurityAttributes() {
}
