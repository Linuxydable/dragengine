/* 
 * Drag[en]gine IGDE World Editor
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "meWPSNavSpace.h"
#include "meWPSNavSpaceListener.h"
#include "../../../../world/meWorld.h"
#include "../../../../world/navspace/meNavigationSpace.h"
#include "../../../../world/navspace/meNavigationSpaceSelection.h"

#include <dragengine/common/exceptions.h>



// Class meWPSNavSpaceListener
////////////////////////////////

// Constructor, destructor
////////////////////////////

meWPSNavSpaceListener::meWPSNavSpaceListener( meWPSNavSpace &panel ) :
pPanel( panel ){
}

meWPSNavSpaceListener::~meWPSNavSpaceListener(){
}



// Management
///////////////

void meWPSNavSpaceListener::NavSpaceSelectionChanged( meWorld* ){
	pPanel.UpdateSelection();
	pPanel.UpdateEnabled();
	pPanel.UpdateNavSpace();
	pPanel.UpdateGeometry();
	pPanel.UpdateUsedCostTypes();
}

void meWPSNavSpaceListener::NavSpaceChanged( meWorld*, meNavigationSpace *navspace ){
	if( ! navspace->GetActive() ){
		return;
	}
	
	pPanel.UpdateNavSpace();
	pPanel.UpdateGeometry();
}

void meWPSNavSpaceListener::NavSpaceGeometryChanged( meWorld*, meNavigationSpace *navspace ){
	if( ! navspace->GetActive() ){
		return;
	}
	
	pPanel.UpdateGeometry();
}

void meWPSNavSpaceListener::NavSpaceUsedCostTypesChanged( meWorld*, meNavigationSpace *navspace ){
	if( ! navspace->GetActive() ){
		return;
	}
	
	pPanel.UpdateUsedCostTypes();
}
