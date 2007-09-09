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
#ifndef FIFESDL_BLENDING_FUNCTIONS_H
#define FIFESDL_BLENDING_FUNCTIONS_H

namespace FIFE {

/** blend one row of n pixels from src with n pixels of dst. */
void SDL_BlendRow_RGBA8_to_RGBA8( const unsigned char* src, unsigned char* dst, unsigned int alpha, int n );
/** blend one row of n pixels from src with n pixels of dst. */
void SDL_BlendRow_RGBA8_to_RGB8( const unsigned char* src, unsigned char* dst, unsigned int alpha, int n );
/** blend one row of n pixels from src with n pixels of dst. */
void SDL_BlendRow_RGBA8_to_RGB565( const unsigned char* src, unsigned char* dst, unsigned int alpha, int n );
/** blend one row of n pixels from src with n pixels of dst. */
void SDL_BlendRow_RGBA4_to_RGB565( const unsigned char* src, unsigned char* dst, unsigned int alpha, int n );

}

#endif
