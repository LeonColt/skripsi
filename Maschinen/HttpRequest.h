// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the CSCOMMUNICATION_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// CSCOMMUNICATION_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#pragma once
#include <WinInet.h>
#include <vector>
#include <memory>
#include <string>
#include <exception>
#include "Maschinen.h"
#include "HttpParam.h"
#include "HttpParams.h"
#include "HttpRequestReadingIterator.h"
#include "Exception.h"
#include "FileHandler.h"
#include "HttpHeaders.h"
// This class is exported from the CSCommunication.dll

namespace maschinen
{
	namespace wininet
	{
		class MASCHINEN_API HttpRequest {
			friend class HttpRequestReadingIterator;
		private:
			HINTERNET hinet, hconnect, hrequest;
			tstring accept_types;
			tstring header;
			tstring referer;
			tstring sending_data;
			tstring url;
			SYSTEMTIME local_time, system_time;
			INT status;

			std::unique_ptr<HttpRequestReadingIterator> ri;
			std::unique_ptr<HttpRequestReadingIterator> reading_finish;
		public:
			//begin InternetOpion
			const DWORD REQUEST_TIME_OUT_INFINITE;
			//end InternetOpeion

			//begin for InternetOpen
			const DWORD ACCESS_TYPE_DIRECT;
			const DWORD ACCESS_TYPE_PRE_CONFIG;
			const DWORD ACESS_TYPE_PRE_CONFIG_WITH_NO_AUTO_PROXY;

			const LPCTSTR VERSION_HTTP_10;
			const LPCTSTR VERSION_HTTP_11;

			HttpRequest( void );
			// TODO: add your methods here.

			void open(LPCTSTR user_agent, DWORD type_access);
			void openProxy( LPCTSTR user_agent, LPCTSTR proxy_name, LPCTSTR proxy_bypass );
			//end for InternetOpen

			//begin SendRequest
			INT sendRequest( LPCTSTR headers );
			INT sendRequest( tstring& headers );
			INT sendRequest( tstring* headers );
			INT sendRequest( maschinen::wininet::HttpHeaders& headers );
			void beginRequest( LPCTSTR header, DWORD header_length_total, LPVOID buffer, DWORD buffer_size, DWORD buffer_size_total ) const;
			void beginRequest( tstring& header, DWORD header_length_total, LPVOID buffer, DWORD buffer_size, DWORD buffer_size_total ) const;
			void beginRequest( tstring* header, DWORD header_length_total, LPVOID buffer, DWORD buffer_size, DWORD buffer_size_total ) const;
			void beginRequest( maschinen::wininet::HttpHeaders& header, DWORD header_length_total, LPVOID buffer, DWORD buffer_size, DWORD buffer_size_total ) const;
			void beginRequest( INTERNET_BUFFERS& internet_buffers ) const;
			void beginRequest( INTERNET_BUFFERS* internet_buffers ) const;
			void endRequest() const;
			//end SendRequest

			//begin HttpOpenRequest
			void openRequest(LPCTSTR url, LPCTSTR version, LPCTSTR referer, LPCTSTR* accept_types, DWORD flags, maschinen::wininet::HttpParams* params);
			//end HttpOpenRequest

			//begin for InternetConnect
			void connect(LPCTSTR server_name, LPCTSTR username = NULL, LPCTSTR password = NULL);
			//end for InternetConnect

			INT getStatusCode() const;

			maschinen::wininet::HttpRequestReadingIterator& beginRead(_Out_ LPVOID buffer, _In_ DWORD buffer_size, _Inout_ LPDWORD number_of_byte_read);
			maschinen::wininet::HttpRequestReadingIterator& endRead() const;
			void write(LPVOID buffer, DWORD buffer_size, LPDWORD number_of_byte_written) const;

			//begin set
			void setReferer( LPCTSTR referer ) noexcept;
			void setAcceptTypes( LPCTSTR accept_types ) noexcept;
			void setRequestTimeOut( DWORD miliseconds );
			void setSendTimeOut( DWORD miliseconds );
			void setReceiveTimeOut( DWORD miliseconds );
			//end set

			//begin get
			std::unique_ptr<tstring> getUrl() const noexcept;
			std::unique_ptr<tstring> getSendingData() const noexcept;
			void getLocalTimeExecuted( LPSYSTEMTIME local_time ) noexcept;
			void getSystemTimeExecuted( LPSYSTEMTIME system_time ) noexcept;
			//end get


			void closeRequest() const noexcept;
			void disconnect() const noexcept;
			void close() const noexcept;
			~HttpRequest();
		};
	}
}