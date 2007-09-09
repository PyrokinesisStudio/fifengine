/***************************************************************************
 *   Copyright (C) 2005-2006 by the FIFE Team                              *
 *   fife-public@lists.sourceforge.net                                     *
 *   This file is part of FIFE.                                            *
 *                                                                         *
 *   FIFE is free software; you can redistribute it and/or modify          *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA              *
 ***************************************************************************/

#ifndef FIFE_MAPDUMP_H
#define FIFE_MAPDUMP_H

// Standard C++ library includes
#include <string>
#include <vector>

// 3rd party library includes

// FIFE includes
#include "map/map.h"
#include "vfs/raw/rawwritetext.h"
#include "vfs/vfs.h"
#include "vfs/vfsdatastruct.h"
#include "vfs/vfswriteformat.h"

namespace FIFE {

	/** MapDump write format
	 *
	 * MapDump dumps the contents of a Map object to disk without any specific 
	 * formatting.  It is implemented for write system testing and as an example 
	 * write format.
	 * @see VFS
	 * @see VFSWriteFormat
	 */
	class MapDump : public VFSWriteFormat {
		public:
			MapDump();
			virtual ~MapDump();

			/** searches the list of supported formats and returns true if the format is supported.
			 * 
			 * @param format the format to test for support.
			 * @return true for supporting the format, false for not supporting it.
			 */
			bool supportsFormat(file_format* format);
			
			/** Writes a file to disk
			 * 
			 * @param data_structure the data to write to disk
			 * @param file_name the path and file to write the data to
			 * @param format the file format to write the data
			 * @return true if the file was successfully written, false if any portion failed
			 */
			virtual bool writeFile(void* data_structure, std::string& file_name, file_format* format);
			
			/** Returns a list of the supported formats for this VFSWriteFormat
			 * 
			 * @return the list of supported formats
			 */
			format_list getSupportedFormats();
		private:
			file_format* dump_format;
			RawWriteText* m_output_file;
	};

}

#endif