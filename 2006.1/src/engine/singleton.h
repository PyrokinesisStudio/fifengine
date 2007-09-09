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

#ifndef SMARTOBJECTS_SINGLETONE_H
#define SMARTOBJECTS_SINGLETONE_H

#define SINGLEFRIEND(classname) friend class FIFE::StaticSingleton<classname>; classname(); virtual ~classname();

#include <cassert>

namespace FIFE {

	/** The "classic" Singleton
	 * @see DynamicSingleton
	 */
	template <typename T> class StaticSingleton {
		public:

			static T* instance() {
				static T inst;
				return &inst;
			}

		protected:

			StaticSingleton() {
			}

			virtual ~StaticSingleton() {
			}

		private:
			StaticSingleton(const StaticSingleton<T>&) {}
			StaticSingleton<T>& operator=(const StaticSingleton<T>&) {
				return this;
			}

	};

	/** Another Singleton
	 *
	 * This implementations needs to be created and destroyed explicitly.
	 * That way the order of con- and destruction is well defined, so we don't
	 * get those nasty static initilization/destruction order problems.
	 *
	 * Engine will create all standard FIFE Singletons when created (just call Engine::instance())
	 * and destroy them on exit.
	 *
	 * Maybe we'l change this one day to use one of those funny NiftyCounter implementations.
	 * @see StaticSingleton
	 * @see Engine
	 */
	template <typename T> class DynamicSingleton {
		public:
			static T* instance() {
				assert(m_instance);
				return m_instance;
			}

			DynamicSingleton() {
				assert(!m_instance);
				m_instance = static_cast<T*>(this);
			}

			virtual ~DynamicSingleton() {
				m_instance = 0;
			}

		private:
			static T* m_instance;

			DynamicSingleton(const DynamicSingleton<T>&) {};
			DynamicSingleton<T&> operator=(const DynamicSingleton<T>&) {};

	};

	template <typename T> T* DynamicSingleton<T>::m_instance = 0;

}//FIFE

#endif