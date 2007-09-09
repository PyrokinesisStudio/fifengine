/***************************************************************************
 *   Copyright (C) 2005-2007 by the FIFE Team                              *
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

#ifndef FIFE_AUDIO_MANAGER_H
#define FIFE_AUDIO_MANAGER_H

// Standard C++ library includes
#include <string>

// Platform specific includes
#include "util/fifeint.h"

// 3rd party library includes

// FIFE includes
// These includes are split up in two parts, separated by one empty line
// First block: files included from the FIFE root src directory
// Second block: files included from the same folder
#include "singleton.h"

namespace FIFE { namespace audio {

	typedef uint8_t type_audiobufptr;

	/** Manager dummy class - for builds without audio support.
	 */
	class Manager : public DynamicSingleton<Manager> {
		public:
			type_audiobufptr getBufferFromFile(const std::string &) { return 0; }

			void setAmbientSound(const std::string &) {}

			static void setVolume(float vol) {}
			static void mute() {}
			static void unmute() {}
	};

} } //FIFE::audio
#endif