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

// FIFE includes
// These includes are split up in two parts, separated by one empty line
// First block: files included from the FIFE root src directory
// Second block: files included from the same folder
#include "map/command/enqueueaction.h"
#include "map/command/setvisual.h"
#include "map/command/startmovement.h"
#include "map/geometries/geometry.h"
#include "map/structures/elevation.h"
#include "map/structures/objectinfo.h"
#include "map/structures/map.h"
#include "script/luascript.h"
#include "video/renderbackend.h"
#include "util/exception.h"
#include "util/log.h"
#include "util/purge.h"
#include "util/settingsmanager.h"
#include "map/map_loader.h"

#include "camera.h"
#include "control.h"
#include "runner.h"
#include "view.h"

namespace FIFE { namespace map {

	Control::Control() 
		: m_map_filename(""),
		m_map(),
		m_view(new View()),
		m_runner(new Runner()),
		m_scriptengine(LuaScript::instance()),
		m_settings(SettingsManager::instance()),
		m_isrunning(false) {

    // load these geometries manually for Fallout backwards-compatibility
		Geometry::registerGeometry(s_geometry_info(
			Geometry::FalloutTileGeometry,
			"RECTANGULAR",
			Point(80,36),  // TILE SIZE
			Point(48,24),  // TRANSFORM
			Point(),       // OFFSET
			0));           // FLAGS: NONE
		Geometry::registerGeometry(s_geometry_info(
			Geometry::FalloutObjectGeometry,
			"HEXAGONAL",
			Point(32,16),             // TILESIZE
			Point(16,12),             // TRANSFORM
			Point(32,10),             // OFFSET
			Geometry::ShiftXAxis));   // FLAGS: SHIFT AROUND X AXIS

		registerCommands();
	}

	Control::~Control() {
		clearMap();

		purge_map(m_loaders);

		// Real cleanup after 'stop()'
		std::set<Camera*>::iterator i(m_cameras.begin());
		for(; i != m_cameras.end(); ++i)
			(*i)->controlDeleted();

		delete m_view;
		delete m_runner;

		Log("map_control") << "objects left alive: " << ObjectInfo::globalCount();
	}

	void Control::addMapLoader(MapLoader* loader) {
		m_loaders.insert(std::pair<std::string,MapLoader*>(loader->getName(), loader));
	}

	void Control::load(const std::string& filename) {
		m_map_filename = filename;

		MapPtr map;
		
		type_loaders::iterator i = m_loaders.begin();
		for (; i != m_loaders.end(); ++i) {
			try {
				MapLoader* loader = i->second;
				Debug("maploader") << "trying to load " << filename;
				map = loader->loadFile(filename);
				break;
			} catch (const Exception& ex) {
			Log("maploader") << ex.getMessage();
			continue;
			}
		}

		if(i == m_loaders.end()) {
			Log("map::Map::load") << "no loader succeeded for " << filename << " :(";
		}

		if (!map) {
			Log("map_control") << "couldn't load map: " << m_map_filename;
			throw CannotOpenFile(m_map_filename);
		}
		setMap(map);
	}

	void Control::save(const std::string& filename) {
		m_loaders["XML"]->saveFile(filename, m_map);
	}

	void Control::setMap(MapPtr map) {
		if (map->getNumElevations() == 0) {
			Warn("map_control") 
				<< "map: " << m_map_filename << " has no elevations";
			throw CannotOpenFile(m_map_filename);
		}
		clearMap();
		m_map = map;

		m_view->setMap(m_map, 0);
		m_view->setViewport(RenderBackend::instance()->getMainScreen());
		std::string ruleset_file = m_settings->read<std::string>("Ruleset", 
		                           "content/scripts/demos/example_ruleset.lua");
		m_runner->setRuleset(ScriptContainer::fromFile(ruleset_file));
		m_runner->initialize(m_map, m_view);
		m_elevation = size_t(-1);
		setElevation(m_map->get<long>("_START_ELEVATION", 0));
	}

	void Control::start() {
		if( !m_map || isRunning() ) {
			return;
		}

		if (m_map->hasScript(Map::OnStartScript)) {
			Log("map_control") 
				<< "Executing: OnStartScript ";
			m_scriptengine->execute(m_map->getScript(Map::OnStartScript));
		}
		m_runner->start();
		m_isrunning = true;
		m_runner->activateElevation(m_elevation);
	}

	void Control::setElevation(size_t elev) {
		if( m_elevation == elev || !m_map ) {
			return;
		}
		m_elevation = elev;
		m_view->setMap(m_map, elev);
		m_runner->displayElevation(elev);

		// Assure a default starting position is set
		ElevationPtr current_elevation = m_view->getCurrentElevation();
		if( !current_elevation->hasAttribute("_START_POSITION") ) {
			Point start_pos = current_elevation->centerOfMass();
			Log("map_control")
				<< "No start position found. "
				<< "Using centerOfMass(): " << start_pos;
			current_elevation->set<Point>("_START_POSITION",start_pos);
		}

		// Assure camera(s) are aware of the change.
		resetCameras();

		if( !isRunning() ) {
			return;
		}

		m_runner->activateElevation(elev);
	}

	size_t Control::getCurrentElevation() const {
		return m_elevation;
	}

	void Control::stop() {
		if (m_map && isRunning()) {
			m_runner->stop();
			m_isrunning = false;
		}
	}

	void Control::clearMap() {
		if( m_map ) {
			stop();
			resetCameras();
			m_view->reset();
			m_map.reset();
		}
	}

	bool Control::isRunning() const {
		return m_isrunning;
	}

	void Control::turn() {
		if( isRunning() ) {
			m_runner->turn();
		}
	}

	MapPtr Control::getMap() {
		return m_map;
	}

	View* Control::getView() {
		return m_view;
	}

	void Control::addCamera(Camera* camera) {
		m_cameras.insert(camera);
	}

	void Control::removeCamera(Camera* camera) {
		m_cameras.erase(camera);
	}

	void Control::registerCommands() {
		m_runner->registerCommand(1, new command::SetVisual());
		m_runner->registerCommand(2, new command::StartMovement());
		m_runner->registerCommand(3, new command::EnqueueAction());
	}

	void Control::resetCameras() {
		std::set<Camera*>::iterator i(m_cameras.begin());
		for(; i != m_cameras.end(); ++i)
			(*i)->reset();
	}

} } // FIFE::map