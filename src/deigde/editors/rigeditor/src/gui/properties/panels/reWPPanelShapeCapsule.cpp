/* 
 * Drag[en]gine IGDE Rig Editor
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

#include "reWPPanelShapeCapsule.h"
#include "../reWPShape.h"
#include "../reWindowProperties.h"
#include "../../reWindowMain.h"
#include "../../../rig/reRig.h"
#include "../../../rig/shape/reRigShapeCapsule.h"
#include "../../../undosys/properties/shape/reUSetShapeCapsuleHalfHeight.h"
#include "../../../undosys/properties/shape/reUSetShapeCapsuleTopRadius.h"
#include "../../../undosys/properties/shape/reUSetShapeCapsuleBottomRadius.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/composed/igdeEditVector.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndoReference.h>

#include <dragengine/common/exceptions.h>



// Actions
////////////

namespace {

class cTextHalfHeight : public igdeTextFieldListener{
	reWPPanelShapeCapsule &pPanel;
public:
	cTextHalfHeight( reWPPanelShapeCapsule &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		reRig * const rig = pPanel.GetRig();
		reRigShapeCapsule * const capsule = ( reRigShapeCapsule* )pPanel.GetShape();
		if( ! rig || ! capsule ){
			return;
		}
		
		const float value = textField->GetFloat();
		if( fabsf( value - capsule->GetHalfHeight() ) < FLOAT_SAFE_EPSILON ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new reUSetShapeCapsuleHalfHeight( capsule, value ) );
		if( undo ){
			rig->GetUndoSystem()->Add( undo );
		}
	}
};

class cTextTopRadius : public igdeTextFieldListener{
	reWPPanelShapeCapsule &pPanel;
public:
	cTextTopRadius( reWPPanelShapeCapsule &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		reRig * const rig = pPanel.GetRig();
		reRigShapeCapsule * const capsule = ( reRigShapeCapsule* )pPanel.GetShape();
		if( ! rig || ! capsule ){
			return;
		}
		
		const float value = textField->GetFloat();
		if( fabsf( value - capsule->GetTopRadius() ) < FLOAT_SAFE_EPSILON ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new reUSetShapeCapsuleTopRadius( capsule, value ) );
		if( undo ){
			rig->GetUndoSystem()->Add( undo );
		}
	}
};

class cTextBottomRadius : public igdeTextFieldListener{
	reWPPanelShapeCapsule &pPanel;
public:
	cTextBottomRadius( reWPPanelShapeCapsule &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		reRig * const rig = pPanel.GetRig();
		reRigShapeCapsule * const capsule = ( reRigShapeCapsule* )pPanel.GetShape();
		if( ! rig || ! capsule ){
			return;
		}
		
		const float value = textField->GetFloat();
		if( fabsf( value - capsule->GetBottomRadius() ) < FLOAT_SAFE_EPSILON ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new reUSetShapeCapsuleBottomRadius( capsule, value ) );
		if( undo ){
			rig->GetUndoSystem()->Add( undo );
		}
	}
};

}



// Class reWPPanelShapeCapsule
////////////////////////////////

// Constructor, destructor
////////////////////////////

reWPPanelShapeCapsule::reWPPanelShapeCapsule( reWPShape &wpShapes ) :
reWPPanelShape( wpShapes, reRigShape::estCapsule )
{
	igdeEnvironment &env = wpShapes.GetEnvironment();
	igdeContainerReference groupBox;
	igdeUIHelper &helper = env.GetUIHelperProperties();
	
	
	
	helper.GroupBox( *this, groupBox, "Capsule Parameters:" );
	
	helper.EditVector( groupBox, "Position:", "Position of the sphere relative to the parent bone.",
		pEditPosition, new cEditPosition( *this ) );
	
	helper.EditVector( groupBox, "Rotation:", "Rotation of the capsule.",
		pEditRotation, new cEditRotation( *this ) );
	
	helper.EditString( groupBox, "Half Height:", "Half height of the capsule.",
		pEditHalfHeight, new cTextHalfHeight( *this ) );
	
	helper.EditString( groupBox, "Top Radius:", "Top radius in meters.",
		pEditTopRadius, new cTextTopRadius( *this ) );
	
	helper.EditString( groupBox, "Bottom Radius:", "Bottom radius in meters.",
		pEditBottomRadius, new cTextBottomRadius( *this ) );
}

reWPPanelShapeCapsule::~reWPPanelShapeCapsule(){
}



// Management
///////////////

void reWPPanelShapeCapsule::UpdateShape(){
	const reRigShapeCapsule * const capsule = ( reRigShapeCapsule* )GetShape();
	
	reWPPanelShape::UpdateShape();
	
	if( capsule ){
		pEditPosition->SetVector( capsule->GetPosition() );
		pEditRotation->SetVector( capsule->GetOrientation() );
		pEditHalfHeight->SetFloat( capsule->GetHalfHeight() );
		pEditTopRadius->SetFloat( capsule->GetTopRadius() );
		pEditBottomRadius->SetFloat( capsule->GetBottomRadius() );
		
	}else{
		pEditPosition->SetVector( decVector() );
		pEditRotation->SetVector( decVector() );
		pEditHalfHeight->ClearText();
		pEditTopRadius->ClearText();
		pEditBottomRadius->ClearText();
	}
}
