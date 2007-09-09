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

// Standard C++ library includes

// 3rd party library includes
#include <guichan.hpp>
#include <SDL.h>
#include <boost/algorithm/string.hpp>

// FIFE includes
// These includes are split up in two parts, separated by one empty line
// First block: files included from the FIFE root src directory
// Second block: files included from the same folder
// FIXME: Do we really have all these dependencies here?
#include "input/inputmanager.h"
#include "vfs/vfs.h"
#include "gui/guimanager.h"
#include "video/renderbackend.h"
#include "util/debugutils.h"
#include "input/events.h"
#include "util/exception.h"
#include "video/imagecache.h"
#include "util/settingsmanager.h"
#include "structures/elevation.h"
#include "structures/layer.h"

#include "camera.h"
#include "control.h"
#include "view.h"
#include "viewgamestate.h"

namespace FIFE { namespace map {
	ViewGameState::ViewGameState(IEventController& ec)
		: FIFE::GameState("MapView", ec),
		m_filename(),
		m_control(new Control()),
		m_map(0),
		m_view(0),
		m_prevDragX(0),
		m_prevDragY(0) {
		m_camera = new Camera(m_control);
		input::Manager::instance()->registerEventListener("map_view", this);
	}

	ViewGameState::~ViewGameState() {
		delete m_control;
		delete m_camera;
	}

	void ViewGameState::addMapLoader(MapLoader* loader) {
		if(m_control)
			m_control->addMapLoader(loader);
	}

	void ViewGameState::setFile(const std::string& map) {
		m_filename = boost::trim_copy_if(map, boost::algorithm::is_cntrl());
	}

	void ViewGameState::activate() {
		try {
			m_control->load(m_filename);
			m_control->start();
		} catch( CannotOpenFile& e ) {
			Warn("view_state")
				<< "Couldn't open " << m_filename 
				<< " (" << e.getMessage() << ")";
			m_valid_map = false;
			return;
		}
		m_valid_map = true;

		m_view = m_control->getView();
		m_camera->moveTo( m_view->getCurrentElevation()->get<Point>("_START_POSITION") );
		m_eventcontroller.addMouseListener(m_view);
		m_eventcontroller.addMouseListener(this);
		std::cout << " >>>> added" << std::endl;

		input::Manager* inputmanager = input::Manager::instance();
		inputmanager->pushContext("map_view");
	}

	void ViewGameState::deactivate() {
		if(!m_valid_map)
			return;
		input::Manager* inputmanager = input::Manager::instance();
		inputmanager->popContext("map_view");
		m_eventcontroller.removeMouseListener(this);
		m_eventcontroller.removeMouseListener(m_view);
		std::cout << " >>>> removed" << std::endl;
		m_control->stop();
	}

	void ViewGameState::turn() {
		if(m_valid_map) {
			m_control->turn();
			m_camera->render();
		}
	}

	void ViewGameState::mousePressed(IMouseEvent& evt) {
		m_prevDragX = 0;
		m_prevDragY = 0;
	}

	void ViewGameState::mouseDragged(IMouseEvent& evt) {
		const float draggingFactor = 1.0;
		Log(evt.getDebugString());
		if (evt.isControlPressed() || (evt.getButton() == IMouseEvent::MIDDLE)) {
			if (m_prevDragX && m_prevDragY) {
				Log(evt.getDebugString());
				int xd = static_cast<int>(static_cast<float>(m_prevDragX - evt.getX()) / draggingFactor);
				int yd = static_cast<int>(static_cast<float>(m_prevDragY - evt.getY()) / draggingFactor);
				m_camera->moveBy(Point(xd, yd));
			}
			m_prevDragX = evt.getX();
			m_prevDragY = evt.getY();
		}
	}

	void ViewGameState::handleEvent(int event) {
		if(!m_valid_map)
			return;

		FIFE::map::ElevationPtr elevation = m_view->getCurrentElevation();
		FIFE::map::LayerPtr obj_grid, floor_grid, roof_grid;

		// FIXME Hardcoded map structure
		if( elevation->getNumLayers() > 0)
			floor_grid = elevation->getLayer(0);
		if( elevation->getNumLayers() > 1)
			obj_grid   = elevation->getLayer(1);
		if( elevation->getNumLayers() > 2)
			roof_grid  = elevation->getLayer(2);

		switch (event) {
			case FIFE::Event::GO_NORTH:
				m_camera->moveBy(Point(0,-20));
				break;
		
			case FIFE::Event::GO_SOUTH:
				m_camera->moveBy(Point(0,+20));
				break;
		
			case FIFE::Event::GO_EAST:
				m_camera->moveBy(Point(-20,0));
				break;
		
			case FIFE::Event::GO_WEST:
				m_camera->moveBy(Point(+20,0));
				break;

			case FIFE::Event::TEST_TOGGLE_OBJECTS:
				if(obj_grid)
					obj_grid->toggleObjectsVisible();
				break;
	
			case FIFE::Event::TEST_CHANGE_ELEVATION:
			{
				size_t elevation = m_view->getElevationNum() + 1;
				if (elevation >= m_view->getNumElevations())
					elevation = 0;
	
				m_control->setElevation(elevation);
				break;
			}
	
			case FIFE::Event::TEST_TOGGLE_ROOF:
				if(roof_grid)
					roof_grid->toggleTilesVisible();
				break;
				
			case FIFE::Event::TEST_TOGGLE_TILE_GRID:
				if(floor_grid)
					floor_grid->toggleGridOverlayEnabled();
				break;
				
			case FIFE::Event::TEST_TOGGLE_OBJ_GRID:
				if(obj_grid)
					obj_grid->toggleGridOverlayEnabled();
				break;
			
			case FIFE::Event::TEST_TOGGLE_COORDS:
				if (m_view)
					m_view->toggleCoordInfoEnabled();
				break;

			case FIFE::Event::TEST_TOGGLE_FADEOUT:
				if (m_view)
					m_view->toggleFadeOutEffect();
			  break;

			case FIFE::Event::TEST_INCREASE_ALPHA:
				if(roof_grid)
					roof_grid->addToGlobalAlpha(5);
				break;
		
			case FIFE::Event::TEST_DECREASE_ALPHA:
				if(roof_grid)
					roof_grid->addToGlobalAlpha(-5);
				break;

			case FIFE::Event::TEST_SAVE:
				if(m_control)
					m_control->save("content/maps/savefile.xml");
				break;
	
			default:
				break;
		}
	}

} } // FIFE::map