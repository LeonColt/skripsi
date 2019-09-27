#include "stdafx.h"
#include "HttpParams.h"


maschinen::wininet::HttpParams::HttpParams()
{
	post_params = std::make_unique<std::vector<std::unique_ptr<maschinen::wininet::HttpParam>>>();
	get_params = std::make_unique<std::vector<std::unique_ptr<maschinen::wininet::HttpParam>>>();
}

void maschinen::wininet::HttpParams::add(LPCTSTR variable, LPCTSTR value, const bool method) const {
	std::unique_ptr<HttpParam> hp = std::make_unique<HttpParam>();
	hp->setVariable(variable);
	hp->setValue(value);
	if(method == METHOD_POST) post_params->push_back(std::move(hp));
	else get_params->push_back(std::move(hp));
}

std::vector<std::unique_ptr<maschinen::wininet::HttpParam>>::iterator maschinen::wininet::HttpParams::begin(const bool method)
{
	method_iterator = method;
	if (method == METHOD_POST) return post_params->begin();
	else return get_params->begin();
}

std::vector<std::unique_ptr<maschinen::wininet::HttpParam>>::iterator maschinen::wininet::HttpParams::end()
{
	if (method_iterator == METHOD_POST) return post_params->end();
	else return get_params->end();
}

void maschinen::wininet::HttpParams::clear() const {
	post_params->clear();
	get_params->clear();
}

void maschinen::wininet::HttpParams::add( const tstring& variable, const tstring& value, const bool method ) const { add( variable.c_str(), value.c_str(), method ); }
void maschinen::wininet::HttpParams::add( const tstring* variable, const tstring* value, const bool method ) const { add( variable->c_str(), value->c_str(), method ); }
void maschinen::wininet::HttpParams::add( const tstring& variable, const tstring* value, const bool method ) const { add( variable.c_str(), value->c_str(), method ); }
void maschinen::wininet::HttpParams::add( const tstring* variable, const tstring& value, const bool method ) const { add( variable->c_str(), value.c_str(), method ); }
void maschinen::wininet::HttpParams::add( LPCTSTR variable, const tstring& value, const bool method ) const { add( variable, value.c_str(), method ); }
void maschinen::wininet::HttpParams::add( LPCTSTR variable, const tstring* value, const bool method ) const { add( variable, value->c_str(), method ); }
void maschinen::wininet::HttpParams::add( const tstring& variable, LPCTSTR value, const bool method ) const { add( variable.c_str(), value, method ); }
void maschinen::wininet::HttpParams::add( const tstring* variable, LPCTSTR value, const bool method ) const { add( variable->c_str(), value, method ); }

void maschinen::wininet::HttpParams::deleteParam( LPCTSTR variable, const bool method ) {
	if ( method == METHOD_GET ) {
		for ( auto i = get_params->begin(); i != get_params->end(); ++i ) {
		}
	}
	else {

	}
}
void maschinen::wininet::HttpParams::deleteParam( const tstring& variable, const bool method ) {

}
void maschinen::wininet::HttpParams::deleteParam( const tstring* variable, const bool method ) {

}

maschinen::wininet::HttpParams::~HttpParams()
{
}
