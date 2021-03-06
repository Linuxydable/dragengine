/* 
 * Drag[en]gine IGDE Language Pack Editor
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

#include <stdlib.h>

#include "lpeIGDEModule.h"

#include <dragengine/common/exceptions.h>



#ifdef __cplusplus
extern "C" {
#endif
MOD_ENTRY_POINT_ATTR igdeEditorModule *LangPackEditorCreateModule( igdeEnvironment *environment );
#ifdef  __cplusplus
}
#endif



igdeEditorModule *LangPackEditorCreateModule( igdeEnvironment *environment ){
	lpeIGDEModule *module = NULL;
	
	try{
		module = new lpeIGDEModule( *environment );
		
	}catch( const deException & ){
		return NULL;
	}
	
	return module;
}
