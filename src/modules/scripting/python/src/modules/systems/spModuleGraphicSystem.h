/* 
 * Drag[en]gine Python Script Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _SPMODULEGRAPHICSYSTEM_H_
#define _SPMODULEGRAPHICSYSTEM_H_

#include "../spBaseModule.h"




/**
 * \brief Graphic system module.
 */
class spModuleGraphicSystem : public spBaseModule{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new module. */
	spModuleGraphicSystem( ScriptingPython &sp );
	/** \brief Cleans up the module. */
	virtual ~spModuleGraphicSystem();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/*@}*/
	
	/** \name Functions */
	/*@{*/
	static PyObject *cfGetWindowWidth( PyObject *myself );
	static PyObject *cfGetWindowHeight( PyObject *myself );
	static PyObject *cfSetWindowGeometry( PyObject *myself, PyObject *args );
	static PyObject *cfSetWindowTitle( PyObject *myself, PyObject *args );
	static PyObject *cfGetPrimaryCanvas( PyObject *myself );
	/*@}*/
};

#endif
