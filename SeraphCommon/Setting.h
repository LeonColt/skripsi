#pragma once
#include <string>
#include "SeraphCommon.h"
#include "SeraphException.h"
#include "FileHandler.h"
#include "GlobalType.h"

class SERAPHCOMMON_API Setting {
private:
	int jumlah_cell_per_panjang, jumlah_cell_per_lebar;
	int log_time_type;//2 for utc and local, 1 for local, 0 for utc, default sama dengan 2, menulis waktu penulisan
	bool log_url, log_post, log_result, show_window_download_after_create_new_download,
		show_window_upload_after_create_new_upload;
	QWORD max_size_uploaded_file_chunked;
	tstring server_name, url;
	tstring cloud_root_name;
	tstring cloud_bin_root_name;
public:
	Setting();
	void read();
	void write();

	bool exists();
	void create();

	//set
	inline void setJumlahCellPerPanjang(int jumlah_cell_per_panjang ) { this->jumlah_cell_per_panjang = jumlah_cell_per_panjang; }
	inline void setJumlahCellPerLebar( int jumlah_cell_per_lebar ) { this->jumlah_cell_per_lebar = jumlah_cell_per_lebar; }
	inline void setLogResult( bool log_result ) { this->log_result = log_result; }
	inline void setLogPost( bool log_post ) { this->log_post = log_post; }
	inline void setLogTimeType( int type ) { this->log_time_type = type; }
	inline void setShowWindowDownloadAfterCreateNewDownload( bool show_window_download_after_create_new_download ) { this->show_window_download_after_create_new_download = show_window_download_after_create_new_download; }
	inline void setShowWindowUploadAfterCreateNewUpload( bool show_window_upload_after_create_new_upload ) { this->show_window_upload_after_create_new_upload = show_window_upload_after_create_new_upload; }
	inline void setMaxFileSizeUploadChunked( QWORD max_size_uploaded_file_chunked ) {this->max_size_uploaded_file_chunked = max_size_uploaded_file_chunked;}
	void setServerName( LPCTSTR server_name );
	void setServerName( tstring& server_name );
	void setServerName( tstring* server_name );
	void setUrl( LPCTSTR url );
	void setUrl( tstring& url );
	void setUrl( tstring* url );
	void setCloudRootName( LPCTSTR cloud_root_name );
	void setCloudRootName( tstring& cloud_root_name );
	void setCloudRootName( tstring* cloud_root_name );
	void setCloudRecycleBinRootName( LPCTSTR cloud_bin_root_name );
	void setCloudRecycleBinRootName( tstring& cloud_bin_root_name );
	void setCloudRecycleBinRootName( tstring* cloud_bin_root_name );
	//get
	inline int getJumlahCellperPanjang() { return jumlah_cell_per_panjang; write(); }
	inline int getJumlahCellperLebar() { return jumlah_cell_per_lebar; write(); }
	inline bool getLogResult() { return log_result; }
	inline bool getLogPost() { return log_post; }
	inline void setLogUrl( bool log_url ) { this->log_url = log_url; }
	inline bool getLogUrl() { return log_url; }
	inline int getLogTimeType() { return log_time_type; }
	inline bool getShowWindowDownloadAfterCreateNewDownload() { return show_window_download_after_create_new_download; }
	inline bool getShowWindowUploadAfterCreateNewUpload() { return show_window_upload_after_create_new_upload; }
	inline QWORD getMaxFileSizeUploadChunked() { return max_size_uploaded_file_chunked; }
	std::unique_ptr<tstring> getServerName();
	std::unique_ptr<tstring> getUrl();
	std::unique_ptr<tstring> getCloudRootName();
	std::unique_ptr<tstring> getCloudRecycleBinRootName();

	std::unique_ptr<tstring> getPath();

	~Setting();
};
