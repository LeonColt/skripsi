#include "stdafx.h"
#include "HttpParam.h"


maschinen::wininet::HttpParam::HttpParam()
{
}
void maschinen::wininet::HttpParam::setVariable(LPCTSTR variable) noexcept {this->variable.assign( variable );}
void maschinen::wininet::HttpParam::setValue(LPCTSTR value) noexcept {this->value.assign( value );}
void maschinen::wininet::HttpParam::setVariable( tstring& variable ) noexcept { this->variable.assign( variable.c_str() ); }
void maschinen::wininet::HttpParam::setValue( tstring& value ) noexcept { this->value.assign( value.c_str() ); }
void maschinen::wininet::HttpParam::setVariable( tstring* variable ){if(variable) this->variable.assign( variable->c_str() );}
void maschinen::wininet::HttpParam::setValue( tstring* value ){if(value)this->value.assign( value->c_str() );}
const TCHAR* maschinen::wininet::HttpParam::getVariable() { return variable.c_str(); }
const TCHAR* maschinen::wininet::HttpParam::getValue() { return value.c_str(); }
maschinen::wininet::HttpParam::~HttpParam()
{
}
