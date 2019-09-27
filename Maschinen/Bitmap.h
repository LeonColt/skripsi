#pragma once
namespace maschinen {
	class Bitmap {
		HBITMAP handler;
	public:
		Bitmap();
		HBITMAP getHandler();
		~Bitmap();
	};
}

