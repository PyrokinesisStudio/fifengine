/***************************************************************************
 *   Copyright (C) 2006 by the FIFE Team                                   *
 *   fife-public@lists.sourceforge.net                                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
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
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef	FIFE_TILEGEOMETRY_H
#define	FIFE_TILEGEOMETRY_H
#include "mapgeometry.h"

namespace FIFE {
	class DefaultTileGeometry : public GridGeometry {
		public:
			DefaultTileGeometry();
			virtual ~DefaultTileGeometry();
			
			virtual Point toScreen(const Point& gridPos);
			virtual Point fromScreen(const Point& screenPos);
			virtual Rect  screenBoundingRect( const Rect& rect );
			virtual Rect  gridBoundingRect( const Rect& rect );
			
			virtual Point baseSize();
	};
	
};

#endif //  FIFE_TileGEOMETRY_H