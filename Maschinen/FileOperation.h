#pragma once
#include <ShlObj.h>
#include "Maschinen.h"
#include "ComponentObjectModel.h"
#include "Exception.h"
#include "FileNotDirectory.h"
namespace maschinen {
	namespace com {
		class MASCHINEN_API FileOperation {
		private:
			const ComponentObjectModel* COM;
			IFileOperation* ifo;
		public:
			FileOperation( const ComponentObjectModel* com );
			void rename(LPCTSTR old_name, LPCTSTR new_name) const;
			void rename( const std::vector<tstring>* const old_names, LPCTSTR new_name ) const;
			void move( LPCTSTR from, LPCTSTR to ) const;
			void move( const std::vector<tstring>* const froms, LPCTSTR to ) const;
			void copy(LPCTSTR from, LPCTSTR to) const;
			void copy( const std::vector<tstring>* const froms, LPCTSTR to ) const;
			void deleteFile( LPCTSTR path ) const;
			void deleteFile( const std::vector<tstring>* const files ) const;
			void commit() const;
			~FileOperation();
		};
	}
}

