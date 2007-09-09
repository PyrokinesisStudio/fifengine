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
#ifndef _AUDIOCONFIG_H
#define _AUDIOCONFIG_H

namespace FIFE {

	// AUDIO_MAX_KEEP_IN_MEM specifies the max. length of a decoded audio data 
	// for which streaming is not used.
	const unsigned int AUDIO_MAX_KEEP_IN_MEM = 1<<21; 
	
	// AUDIO_BUFFER_NUM specifies the number of buffers used for streaming.
	const short AUDIO_BUFFER_NUM=3;
	
	// AUDIO_BUFFER_LEN specifies the length of one buffer.
	const unsigned int AUDIO_BUFFER_LEN=1<<20;

}

#endif
