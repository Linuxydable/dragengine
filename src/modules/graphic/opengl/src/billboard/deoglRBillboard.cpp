/* 
 * Drag[en]gine OpenGL Graphic Module
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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deoglRBillboard.h"
#include "../delayedoperation/deoglDelayedDeletion.h"
#include "../delayedoperation/deoglDelayedOperations.h"
#include "../envmap/deoglEnvironmentMap.h"
#include "../occlusiontest/deoglOcclusionTest.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTBufferObject.h"
#include "../renderthread/deoglRTChoices.h"
#include "../renderthread/deoglRTDefaultTextures.h"
#include "../renderthread/deoglRTLogger.h"
#include "../renderthread/deoglRTShader.h"
#include "../rendering/plan/deoglRenderPlan.h"
#include "../rendering/plan/deoglRenderPlanMasked.h"
#include "../shaders/paramblock/deoglSPBlockUBO.h"
#include "../shaders/paramblock/deoglSPBParameter.h"
#include "../shaders/paramblock/shared/deoglSharedSPBElement.h"
#include "../shaders/paramblock/shared/deoglSharedSPBList.h"
#include "../shaders/paramblock/shared/deoglSharedSPBListUBO.h"
#include "../shaders/paramblock/shared/deoglSharedSPBRTIGroup.h"
#include "../skin/channel/deoglSkinChannel.h"
#include "../skin/deoglRSkin.h"
#include "../skin/deoglSkinRenderable.h"
#include "../skin/deoglSkinTexture.h"
#include "../skin/dynamic/deoglRDynamicSkin.h"
#include "../skin/dynamic/renderables/render/deoglRDSRenderable.h"
#include "../skin/shader/deoglSkinShader.h"
#include "../skin/state/deoglSkinState.h"
#include "../skin/state/deoglSkinStateRenderable.h"
#include "../texture/texunitsconfig/deoglTexUnitConfig.h"
#include "../texture/texunitsconfig/deoglTexUnitsConfig.h"
#include "../texture/texunitsconfig/deoglTexUnitsConfigList.h"
#include "../utils/collision/deoglDCollisionBox.h"
#include "../utils/collision/deoglDCollisionSphere.h"
#include "../utils/deoglCubeHelper.h"
#include "../visitors/deoglFindBestEnvMap.h"
#include "../world/deoglRWorld.h"
#include "../world/deoglWorldOctree.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/billboard/deBillboard.h>



// Class deoglRBillboard
//////////////////////////

// Constructor, destructor
////////////////////////////

deoglRBillboard::deoglRBillboard( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),

pParentWorld( NULL ),
pOctreeNode( NULL ),

pSkin( NULL ),
pUseSkinTexture( NULL ),
pDynamicSkin( NULL ),
pLocked( true ),
pSpherical( false ),
pSizeFixedToScreen( false ),
pVisible( true ),

pSkinState( NULL ),
pSkinRendered( renderThread, *this ),
pSkyShadowSplitMask( 0 ),
pSortDistance( 0.0f ),
pOccluded( false ),
pDirtyRenderables( true ),

pRenderEnvMap( NULL ),
pRenderEnvMapFade( NULL ),
pRenderEnvMapFadePerTime( 1.0f ),
pRenderEnvMapFadeFactor( 1.0f ),
pDirtyRenderEnvMap( true ),

pParamBlockDepth( NULL ),
pParamBlockGeometry( NULL ),
pParamBlockEnvMap( NULL ),
pSharedSPBElement( NULL ),
pSharedSPBRTIGroup( NULL ),

pTUCDepth( NULL ),
pTUCGeometry( NULL ),
pTUCCounter( NULL ),
pTUCEnvMap( NULL ),

pValidParamBlockDepth( false ),
pValidParamBlockGeometry( false ),
pValidParamBlockEnvMap( false ),
pDirtyParamBlockDepth( true ),
pDirtyParamBlockGeometry( true ),
pDirtyParamBlockEnvMap( true ),
pDirtySharedSPBElement( true ),

pDirtyTUCDepth( true ),
pDirtyTUCGeometry( true ),
pDirtyTUCCounter( true ),
pDirtyTUCEnvMap( true ),

pParamBlockSpecial( NULL ),
pSpecialFlags( 0 ),

pCullSphereRadius( 0.0f ),
pDirtyCulling( true ),
pRenderVisible( true ),

pMarked( false ),

pWorldMarkedRemove( false ),
pLLWorldPrev( NULL ),
pLLWorldNext( NULL )
{
	try{
		pSkinState = new deoglSkinState( renderThread, *this );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
	LEAK_CHECK_CREATE( renderThread, Billboard );
}

deoglRBillboard::~deoglRBillboard(){
	LEAK_CHECK_FREE( pRenderThread, Billboard );
	pCleanUp();
}



// Management
///////////////

void deoglRBillboard::SetParentWorld( deoglRWorld *parentWorld ){
	if( parentWorld == pParentWorld ){
		return;
	}
	
	InvalidateRenderEnvMap();
	pSkinRendered.DropDelayedDeletionObjects();
	
	pParentWorld = parentWorld;
	
	if( pOctreeNode ){
		pOctreeNode->RemoveBillboard( this );
	}
	
	pDirtyRenderEnvMap = true;
}



void deoglRBillboard::SetOctreeNode( deoglWorldOctree *octreeNode ){
	pOctreeNode = octreeNode;
}

void deoglRBillboard::UpdateOctreeNode(){
	if( ! pParentWorld ){
		return;
	}
	
	// insert into parent world octree
	if( pVisible ){
		pParentWorld->GetOctree().InsertBillboardIntoTree( this, 8 );
		
	}else{
		if( pOctreeNode ){
			pOctreeNode->RemoveBillboard( this );
		}
	}
}



void deoglRBillboard::SetSkin( deoglRSkin *skin ){
	if( skin == pSkin ){
		return;
	}
	
	if( pSkin ){
		pSkin->FreeReference();
	}
	
	pSkin = skin;
	pUseSkinTexture = skin && skin->GetTextureCount() > 0 ? &skin->GetTextureAt( 0 ) : NULL;
	
	if( skin ){
		skin->AddReference();
	}
	
	pSkinRendered.SetDirty();
}

void deoglRBillboard::SetDynamicSkin( deoglRDynamicSkin *dynamicSkin ){
	// NOTE this is called from the main thread during synchronization
	if( dynamicSkin == pDynamicSkin ){
		return;
	}
	
	if( pDynamicSkin ){
		pDynamicSkin->FreeReference();
	}
	
	pDynamicSkin = dynamicSkin;
	
	if( dynamicSkin ){
		dynamicSkin->AddReference();
	}
	
	pSkinRendered.SetDirty();
}



void deoglRBillboard::SetPosition( const decDVector &position ){
	pPosition = position;
}

void deoglRBillboard::SetAxis( const decVector &axis ){
	pAxis = axis;
}

void deoglRBillboard::SetSize( const decVector2 &size ){
	pSize = size;
}

void deoglRBillboard::SetOffset( const decVector2 &offset ){
	pOffset = offset;
}

void deoglRBillboard::SetLayerMask( const decLayerMask &layerMask ){
	pLayerMask = layerMask;
}

void deoglRBillboard::SetVisible( bool visible ){
	pVisible = visible;
}

void deoglRBillboard::SetLocked( bool locked ){
	pLocked = locked;
}

void deoglRBillboard::SetSpherical( bool spherical ){
	pSpherical = spherical;
}

void deoglRBillboard::SetSizeFixedToScreen( bool sizeFixedToScreen ){
	pSizeFixedToScreen = sizeFixedToScreen;
}

void deoglRBillboard::UpdateSkin( float elapsed ){
	// WARNING called from main thread during synchronization time
	
	pSkinState->AdvanceTime( elapsed );
	if( pUseSkinTexture ){
		if( pUseSkinTexture->GetDynamicChannels() ){
			MarkParamBlocksDirty();
			MarkTUCsDirty();
			
		}else if( pUseSkinTexture->GetCalculatedProperties() ){
			MarkParamBlocksDirty();
		}
	}
	
	if( pRenderEnvMapFade ){
		pRenderEnvMapFadeFactor += pRenderEnvMapFadePerTime * elapsed;
		
		if( pRenderEnvMapFadeFactor >= 1.0f ){
			SetRenderEnvMapFade( NULL );
			pRenderEnvMapFadeFactor = 1.0f;
		}
	}
}

void deoglRBillboard::InitSkinStateCalculatedProperties(){
	pSkinState->InitCalculatedProperties();
}

void deoglRBillboard::SetDirtyRendereables(){
	pDirtyRenderables = true;
}

void deoglRBillboard::UpdateRenderables( deoglRenderPlan& ){
	// update renderables. required here to ensure skin state is properly set up
	pUpdateRenderables();
	
	// make sure all textures are updated and create the render info if required
	pSkinState->PrepareRenderables( pSkin, pDynamicSkin );
}

void deoglRBillboard::AddSkinStateRenderPlans( deoglRenderPlan &plan ){
	pSkinState->AddRenderPlans( plan );
	pSkinRendered.AddRenderPlans( plan );
}



void deoglRBillboard::UpdateExtends( const deBillboard &billboard ){
	const decDVector &position = billboard.GetPosition();
	const decVector2 &size = billboard.GetSize();
	const float width = size.x * 0.5f;
	const float height = size.y * 0.5f;
	
	if( width > height ){
		pMinExtend.x = position.x - width;
		pMinExtend.y = position.y - width;
		pMinExtend.z = position.z - width;
		pMaxExtend.x = position.x + width;
		pMaxExtend.y = position.y + width;
		pMaxExtend.z = position.z + width;
		
	}else{
		pMinExtend.x = position.x - height;
		pMinExtend.y = position.y - height;
		pMinExtend.z = position.z - height;
		pMaxExtend.x = position.x + height;
		pMaxExtend.y = position.y + height;
		pMaxExtend.z = position.z + height;
	}
}



deoglSPBlockUBO *deoglRBillboard::GetParamBlockFor( deoglSkinTexture::eShaderTypes shaderType ){
	switch( shaderType ){
	case deoglSkinTexture::estBillboardGeometry:
		return GetParamBlockGeometry();
		
	case deoglSkinTexture::estBillboardDepth:
	case deoglSkinTexture::estBillboardDepthClipPlane:
	case deoglSkinTexture::estBillboardDepthReversed:
	case deoglSkinTexture::estBillboardDepthClipPlaneReversed:
	case deoglSkinTexture::estBillboardCounter:
	case deoglSkinTexture::estBillboardCounterClipPlane:
		return GetParamBlockDepth();
		
	case deoglSkinTexture::estBillboardEnvMap:
		return GetParamBlockEnvMap();
		
	default:
		DETHROW( deeInvalidParam );
	}
}

deoglSPBlockUBO *deoglRBillboard::GetParamBlockDepth(){
	if( ! pValidParamBlockDepth ){
		if( pParamBlockDepth ){
			pParamBlockDepth->FreeReference();
			pParamBlockDepth = NULL;
		}
		
		if( pUseSkinTexture ){
			deoglSkinShader &skinShader = *pUseSkinTexture->GetShaderFor( deoglSkinTexture::estBillboardDepth );
			
			if( deoglSkinShader::USE_SHARED_SPB ){
				pParamBlockDepth = new deoglSPBlockUBO( *pRenderThread.GetBufferObject().GetLayoutSkinInstanceUBO() );
				
			}else{
				pParamBlockDepth = skinShader.CreateSPBInstParam();
			}
		}
		
		pValidParamBlockDepth = true;
		pDirtyParamBlockDepth = true;
	}
	
	if( pDirtyParamBlockDepth ){
		if( pParamBlockDepth && pUseSkinTexture ){
			pParamBlockDepth->MapBuffer();
			try{
				UpdateInstanceParamBlock( *pParamBlockDepth, 0,
					*pUseSkinTexture->GetShaderFor( deoglSkinTexture::estBillboardDepth ) );
				
			}catch( const deException & ){
				pParamBlockDepth->UnmapBuffer();
				throw;
			}
			pParamBlockDepth->UnmapBuffer();
		}
		
		pDirtyParamBlockDepth = false;
	}
	
	return pParamBlockDepth;
}

deoglSPBlockUBO *deoglRBillboard::GetParamBlockGeometry(){
	if( ! pValidParamBlockGeometry ){
		if( pParamBlockGeometry ){
			pParamBlockGeometry->FreeReference();
			pParamBlockGeometry = NULL;
		}
		
		if( pUseSkinTexture ){
			deoglSkinShader &skinShader = *pUseSkinTexture->GetShaderFor( deoglSkinTexture::estBillboardGeometry );
			
			if( deoglSkinShader::USE_SHARED_SPB ){
				pParamBlockGeometry = new deoglSPBlockUBO( *pRenderThread.GetBufferObject().GetLayoutSkinInstanceUBO() );
				
			}else{
				pParamBlockGeometry = skinShader.CreateSPBInstParam();
			}
		}
		
		pValidParamBlockGeometry = true;
		pDirtyParamBlockGeometry = true;
	}
	
	if( pDirtyParamBlockGeometry ){
		if( pParamBlockGeometry && pUseSkinTexture ){
			pParamBlockGeometry->MapBuffer();
			try{
				UpdateInstanceParamBlock( *pParamBlockGeometry, 0,
					*pUseSkinTexture->GetShaderFor( deoglSkinTexture::estBillboardGeometry ) );
				
			}catch( const deException & ){
				pParamBlockGeometry->UnmapBuffer();
				throw;
			}
			pParamBlockGeometry->UnmapBuffer();
		}
		
		pDirtyParamBlockGeometry = false;
	}
	
	return pParamBlockGeometry;
}

deoglSPBlockUBO *deoglRBillboard::GetParamBlockEnvMap(){
	return NULL;
}

deoglSharedSPBElement *deoglRBillboard::GetSharedSPBElement(){
	if( ! pSharedSPBElement ){
		if( pRenderThread.GetChoices().GetSharedSPBUseSSBO() ){
			pSharedSPBElement = pRenderThread.GetBufferObject()
				.GetSharedSPBList( deoglRTBufferObject::esspblSkinInstanceSSBO ).AddElement();
			
		}else if( pRenderThread.GetChoices().GetGlobalSharedSPBLists() ){
			pSharedSPBElement = pRenderThread.GetBufferObject()
				.GetSharedSPBList( deoglRTBufferObject::esspblSkinInstanceUBO ).AddElement();
			
		}else{
			pSharedSPBElement = pRenderThread.GetBufferObject().GetBillboardSPBListUBO().AddElement();
		}
	}
	
	if( pDirtySharedSPBElement ){
		if( pSharedSPBElement && pUseSkinTexture ){
			// it does not matter which shader type we use since all are required to use the
			// same shared spb instance layout
			deoglSkinShader &skinShader = *pUseSkinTexture->GetShaderFor( deoglSkinTexture::estBillboardGeometry );
			
			// update parameter block area belonging to this element
			deoglShaderParameterBlock &paramBlock = pSharedSPBElement->MapBuffer();
			try{
				UpdateInstanceParamBlock( paramBlock, pSharedSPBElement->GetIndex(), skinShader );
				
			}catch( const deException & ){
				paramBlock.UnmapBuffer();
				throw;
			}
			
			paramBlock.UnmapBuffer();
		}
		
		pDirtySharedSPBElement = false;
	}
	
	return pSharedSPBElement;
}

deoglSharedSPBRTIGroup &deoglRBillboard::GetSharedSPBRTIGroup(){
	if( ! pSharedSPBRTIGroup ){
		pSharedSPBRTIGroup = pRenderThread.GetBufferObject().GetBillboardRTIGroups()
			.GetWith( GetSharedSPBElement()->GetSPB() );
	}
	return *pSharedSPBRTIGroup;
}

deoglTexUnitsConfig *deoglRBillboard::GetTUCForShaderType( deoglSkinTexture::eShaderTypes shaderType ){
	switch( shaderType ){
	case deoglSkinTexture::estBillboardGeometry:
	case deoglSkinTexture::estDecalGeometry:
		return GetTUCGeometry();
		
	case deoglSkinTexture::estBillboardDepth:
	case deoglSkinTexture::estBillboardDepthClipPlane:
	case deoglSkinTexture::estBillboardDepthReversed:
	case deoglSkinTexture::estBillboardDepthClipPlaneReversed:
		return GetTUCDepth();
		
	case deoglSkinTexture::estBillboardCounter:
	case deoglSkinTexture::estBillboardCounterClipPlane:
		return GetTUCCounter();
		
	case deoglSkinTexture::estBillboardEnvMap:
		return GetTUCEnvMap();
		
	default:
		DETHROW( deeInvalidParam );
	}
}

deoglTexUnitsConfig *deoglRBillboard::GetTUCDepth(){
	if( pDirtyTUCDepth ){
		if( pTUCDepth ){
			pTUCDepth->RemoveUsage();
			pTUCDepth = NULL;
		}
		
		pTUCDepth = BareGetTUCFor( deoglSkinTexture::estBillboardDepth );
		
		pDirtyTUCDepth = false;
	}
	
	return pTUCDepth;
}

deoglTexUnitsConfig *deoglRBillboard::GetTUCGeometry(){
	if( pDirtyTUCGeometry ){
		if( pTUCGeometry ){
			pTUCGeometry->RemoveUsage();
			pTUCGeometry = NULL;
		}
		
		pTUCGeometry = BareGetTUCFor( deoglSkinTexture::estBillboardGeometry );
		
		pDirtyTUCGeometry = false;
	}
	
	return pTUCGeometry;
}

deoglTexUnitsConfig *deoglRBillboard::GetTUCCounter(){
	if( pDirtyTUCCounter ){
		if( pTUCCounter ){
			pTUCCounter->RemoveUsage();
			pTUCCounter = NULL;
		}
		
		pTUCCounter = BareGetTUCFor( deoglSkinTexture::estBillboardCounter );
		
		pDirtyTUCCounter = false;
	}
	
	return pTUCCounter;
}

deoglTexUnitsConfig *deoglRBillboard::GetTUCEnvMap(){
	if( pDirtyTUCEnvMap ){
		if( pTUCEnvMap ){
			pTUCEnvMap->RemoveUsage();
			pTUCEnvMap = NULL;
		}
		
		if( pUseSkinTexture ){
			deoglRenderThread &renderThread = pRenderThread;
			deoglTexUnitConfig unit[ 8 ];
			
			if( pUseSkinTexture->GetVariationU() || pUseSkinTexture->GetVariationV() ){
				unit[ 0 ].EnableArrayTextureFromChannel( renderThread, *pUseSkinTexture,
					deoglSkinChannel::ectColor, pSkinState, pDynamicSkin,
					renderThread.GetDefaultTextures().GetColorArray() );
				
				unit[ 1 ].EnableArrayTextureFromChannel( renderThread, *pUseSkinTexture,
					deoglSkinChannel::ectEmissivity, pSkinState, pDynamicSkin,
					renderThread.GetDefaultTextures().GetEmissivityArray() );
				
			}else{
				unit[ 0 ].EnableTextureFromChannel( renderThread, *pUseSkinTexture,
					deoglSkinChannel::ectColor, pSkinState, pDynamicSkin,
					renderThread.GetDefaultTextures().GetColor() );
				
				unit[ 1 ].EnableTextureFromChannel( renderThread, *pUseSkinTexture,
					deoglSkinChannel::ectEmissivity, pSkinState, pDynamicSkin,
					renderThread.GetDefaultTextures().GetEmissivity() );
			}
			
			pTUCEnvMap = renderThread.GetShader().GetTexUnitsConfigList().GetWith( &unit[ 0 ], 2 );
		}
		
		pDirtyTUCEnvMap = false;
	}
	
	return pTUCEnvMap;
}

deoglTexUnitsConfig *deoglRBillboard::BareGetTUCFor(
deoglSkinTexture::eShaderTypes shaderType ) const{
	if( ! pUseSkinTexture ){
		return NULL;
	}
	
	deoglRenderThread &renderThread = pRenderThread;
	deoglTexUnitConfig units[ deoglSkinShader::ETT_COUNT ];
	deoglRDynamicSkin *dynamicSkin = NULL;
	deoglSkinState *skinState = NULL;
	deoglTexUnitsConfig *tuc = NULL;
	
	deoglSkinShader &skinShader = *pUseSkinTexture->GetShaderFor( shaderType );
	
	if( skinShader.GetUsedTextureTargetCount() > 0 ){
		skinShader.SetTUCCommon( &units[ 0 ], *pUseSkinTexture, skinState, dynamicSkin );
		skinShader.SetTUCPerObjectEnvMap( &units[ 0 ], pParentWorld->GetSkyEnvironmentMap(),
			pRenderEnvMap, pRenderEnvMapFade );
		tuc = renderThread.GetShader().GetTexUnitsConfigList().GetWith(
			&units[ 0 ], skinShader.GetUsedTextureTargetCount() );
	}
	
	return tuc;
}

void deoglRBillboard::InvalidateParamBlocks(){
	pValidParamBlockDepth = false;
	pValidParamBlockGeometry = false;
	pValidParamBlockEnvMap = false;
	
	MarkParamBlocksDirty();
}

void deoglRBillboard::MarkParamBlocksDirty(){
	pDirtyParamBlockDepth = true;
	pDirtyParamBlockGeometry = true;
	pDirtyParamBlockEnvMap = true;
	pDirtySharedSPBElement = true;
}

void deoglRBillboard::MarkTUCsDirty(){
	pDirtyTUCDepth = true;
	pDirtyTUCGeometry = true;
	pDirtyTUCCounter = true;
	pDirtyTUCEnvMap = true;
}



void deoglRBillboard::UpdateInstanceParamBlock( deoglShaderParameterBlock &paramBlock,
int element, deoglSkinShader &skinShader ){
	if( ! pUseSkinTexture ){
		return;
	}
	
	// update shader parameter block
	int target;
	
	target = skinShader.GetInstanceUniformTarget( deoglSkinShader::eiutMatrixModel );
	if( target != -1 ){
		const decDVector &referencePosition = pParentWorld->GetReferencePosition();
		decDMatrix matrix;
		
		matrix.a14 = pPosition.x - referencePosition.x;
		matrix.a24 = pPosition.y - referencePosition.y;
		matrix.a34 = pPosition.z - referencePosition.z;
		
		matrix.a21 = pAxis.x;
		matrix.a22 = pAxis.y;
		matrix.a23 = pAxis.z;
		
		paramBlock.SetParameterDataMat4x3( target, element, matrix );
	}
	
	target = skinShader.GetInstanceUniformTarget( deoglSkinShader::eiutMatrixNormal );
	if( target != -1 ){
		paramBlock.SetParameterDataMat3x3( target, element, decMatrix() );
		//paramBlock.SetParameterDataMat3x3( target, element, decMatrix().GetRotationMatrix().Invert() );
	}
	
	target = skinShader.GetInstanceUniformTarget( deoglSkinShader::eiutBillboardPosTransform );
	if( target != -1 ){
		decVector2 scale( pSize.x * 0.5f, pSize.y * 0.5f );
		decVector2 offset( pOffset.x, pOffset.y );
		paramBlock.SetParameterDataVec4( target, scale.x, scale.y, offset.x, offset.y );
	}
	
	target = skinShader.GetInstanceUniformTarget( deoglSkinShader::eiutBillboardParams );
	if( target != -1 ){
		paramBlock.SetParameterDataBVec3( target, pLocked, pSpherical, pSizeFixedToScreen );
	}
	
	// per texture properties
	target = skinShader.GetInstanceUniformTarget( deoglSkinShader::eiutMatrixTexCoord );
	if( target != -1 ){
		decTexMatrix2 matrix;
		
		const deoglSkinTextureProperty &propertyOffset =
			pUseSkinTexture->GetMaterialPropertyAt( deoglSkinTexture::empTexCoordOffset );
		const deoglSkinTextureProperty &propertyScale =
			pUseSkinTexture->GetMaterialPropertyAt( deoglSkinTexture::empTexCoordScale );
		const deoglSkinTextureProperty &propertyRotate =
			pUseSkinTexture->GetMaterialPropertyAt( deoglSkinTexture::empTexCoordRotate );
		
		const decVector2 offset( propertyOffset.ResolveVector2(
			pSkinState, pDynamicSkin, pUseSkinTexture->GetTexCoordOffset() ) );
		const decVector2 scale( propertyScale.ResolveVector2(
			pSkinState, pDynamicSkin, pUseSkinTexture->GetTexCoordScale() ) );
		const float rotate = propertyRotate.ResolveAsFloat(
			pSkinState, pDynamicSkin, pUseSkinTexture->GetTexCoordRotate() );
		
		const bool hasOffset = ! decVector2().IsEqualTo( offset );
		const bool hasScale = ! decVector2( 1.0f, 1.0f ).IsEqualTo( scale );
		const bool hasRotate = fabsf( rotate ) > FLOAT_SAFE_EPSILON;
		
		if( hasScale || hasRotate ){
			matrix = decTexMatrix2::CreateCenterSRT( scale, rotate * TWO_PI, offset ) * matrix;
			
		}else if( hasOffset ){
			matrix = decTexMatrix2::CreateTranslation( offset ) * matrix;
		}
		
		paramBlock.SetParameterDataMat3x2( target, element, matrix );
	}
	
	target = skinShader.GetInstanceUniformTarget( deoglSkinShader::eiutDoubleSided );
	if( target != -1 ){
		paramBlock.SetParameterDataBool( target, element, true );
	}
	
	target = skinShader.GetInstanceUniformTarget( deoglSkinShader::eiutEnvMapFade );
	if( target != -1 ){
		paramBlock.SetParameterDataFloat( target, element, pRenderEnvMapFadeFactor );
	}
	
	target = skinShader.GetInstanceUniformTarget( deoglSkinShader::eiutVariationSeed );
	if( target != -1 ){
		if( pSkinState ){
			paramBlock.SetParameterDataVec2( target, element, pSkinState->GetVariationSeed() );
			
		}else{
			paramBlock.SetParameterDataVec2( target, element, 0.0f, 0.0f );
		}
	}
	
	// per texture dynamic texture properties
	skinShader.SetDynTexParamsInInstParamSPB( paramBlock, element, *pUseSkinTexture, pSkinState, pDynamicSkin );
}


deoglSPBlockUBO *deoglRBillboard::GetParamBlockSpecial(){
	if( ! pParamBlockSpecial ){
		pParamBlockSpecial = deoglSkinShader::CreateSPBSpecial( pRenderThread );
	}
	return pParamBlockSpecial;
}

void deoglRBillboard::UpdateCubeFaceVisibility( const decDVector &cubePosition ){
	deoglCubeHelper::CalcFaceVisibility(
		pMinExtend - cubePosition, pMaxExtend - cubePosition, pCubeFaceVisible );
	
	// DEBUG
	/*
	pRenderThread.GetLogger().LogInfoFormat( "DEBUG: (%g,%g,%g) [%d, %d, %d, %d, %d, %d] {(%g,%g,%g), (%g,%g,%g)}",
		pMatrix.GetPosition().x, pMatrix.GetPosition().y, pMatrix.GetPosition().z,
		    pCubeFaceVisible[0], pCubeFaceVisible[1], pCubeFaceVisible[2],
		    pCubeFaceVisible[3], pCubeFaceVisible[4], pCubeFaceVisible[5],
		    (pMinExtend - cubePosition).x, (pMinExtend - cubePosition).y, (pMinExtend - cubePosition).z,
		    (pMaxExtend - cubePosition).x, (pMaxExtend - cubePosition).y, (pMaxExtend - cubePosition).z );
	*/
}

bool deoglRBillboard::GetCubeFaceVisible( int cubeFace ) const{
	if( cubeFace < 0 || cubeFace > 5 ){
		DETHROW( deeInvalidParam );
	}
	return pCubeFaceVisible[ cubeFace ];
}

void deoglRBillboard::SetSpecialFlagsFromFaceVisibility(){
	pSpecialFlags = 0;
	if( pCubeFaceVisible[ 0 ] ){
		pSpecialFlags |= 0x1;
	}
	if( pCubeFaceVisible[ 1 ] ){
		pSpecialFlags |= 0x2;
	}
	if( pCubeFaceVisible[ 2 ] ){
		pSpecialFlags |= 0x8;
	}
	if( pCubeFaceVisible[ 3 ] ){
		pSpecialFlags |= 0x4;
	}
	if( pCubeFaceVisible[ 4 ] ){
		pSpecialFlags |= 0x10;
	}
	if( pCubeFaceVisible[ 5 ] ){
		pSpecialFlags |= 0x20;
	}
}

void deoglRBillboard::UpdateSpecialSPBCubeRender(){
	deoglSPBlockUBO &spb = *GetParamBlockSpecial();
	
	SetSpecialFlagsFromFaceVisibility();
	
	spb.MapBuffer();
	try{
		spb.SetParameterDataInt( deoglSkinShader::esutCubeFaceVisible, pSpecialFlags );
		
	}catch( const deException & ){
		spb.UnmapBuffer();
		throw;
	}
	spb.UnmapBuffer();
}



const decDVector &deoglRBillboard::GetCullSphereCenter(){
	if( pDirtyCulling ){
		pUpdateCullSphere();
		pDirtyCulling = false;
	}
	
	return pCullSphereCenter;
}

float deoglRBillboard::GetCullSphereRadius(){
	if( pDirtyCulling ){
		pUpdateCullSphere();
		pDirtyCulling = false;
	}
	
	return pCullSphereRadius;
}

void deoglRBillboard::SetDirtyCulling(){
	pDirtyCulling = true;
}



void deoglRBillboard::StartOcclusionTest( const decDVector &cameraPosition ){
	if( ! pUseSkinTexture ){
		return;
	}
	
	const decVector minExtend( pMinExtend - cameraPosition );
	const decVector maxExtend( pMaxExtend - cameraPosition );
	pRenderThread.GetOcclusionTest().AddInputData( minExtend, maxExtend, this );
}

void deoglRBillboard::OcclusionTestInvisible(){
	pRenderVisible = false;
}

void deoglRBillboard::SetRenderVisible( bool visible ){
	pRenderVisible = visible;
}

void deoglRBillboard::SetSortDistance( float distance ){
	pSortDistance = distance;
}

void deoglRBillboard::SetOccluded( bool occluded ){
	pOccluded = occluded;
}

void deoglRBillboard::SetSkyShadowSplitMask( int mask ){
	pSkyShadowSplitMask = mask;
}



// Render world usage
///////////////////////

void deoglRBillboard::SetWorldMarkedRemove( bool marked ){
	pWorldMarkedRemove = marked;
}

void deoglRBillboard::SetRenderEnvMap( deoglEnvironmentMap *envmap ){
	// note about the switch process. we have to wait setting the fading environment map until the
	// new environment map has been set. if this is not done the SetRenderEnvMapFade function tries
	// to add the billboard to the billboard list of the same environment map as the current one
	// resulting in an exception. to avoid this the fade environment map is first cleared, then
	// the environment set and then the fade environment map is set again
	// 
	// another note. if a new environment map is set while the fading is still in progress a hard
	// switch may still occur. a possible solution would be to delay the switch until the fading
	// is finished. for this we would have to keep the dirty flag set, which is currently set
	// outside somewhere
	if( envmap == pRenderEnvMap ){
		return;
	}
	
	deoglEnvironmentMap * const prevEnvMap = pRenderEnvMap;
	if( prevEnvMap ){
		prevEnvMap->AddReference(); // guard reference
	}
	
	try{
		if( pRenderEnvMap ){
			pRenderEnvMap->GetBillboardList().RemoveIfExisting( this );
			pRenderEnvMap->FreeReference();
		}
		
		pRenderEnvMap = envmap;
		
		if( envmap ){
			envmap->AddReference();
			envmap->GetBillboardList().Add( this );
		}
		
		// now it is safe to set the fade env map
		SetRenderEnvMapFade( prevEnvMap );
		pRenderEnvMapFadeFactor = 0.0f;
		
		if( ! prevEnvMap ){ // in case SetRenderEnvMapFade did not mark all textures dirty yet
			MarkTUCsDirty();
		}
		
	}catch( const deException & ){
		if( prevEnvMap ){
			prevEnvMap->FreeReference(); // drop guard reference
		}
		throw;
	}
	
	if( prevEnvMap ){
		prevEnvMap->FreeReference(); // drop guard reference
	}
}

void deoglRBillboard::SetRenderEnvMapFade( deoglEnvironmentMap *envmap ){
	if( envmap == pRenderEnvMapFade ){
		return;
	}
	
	if( pRenderEnvMapFade ){
		pRenderEnvMapFade->GetBillboardList().RemoveIfExisting( this );
		pRenderEnvMapFade->FreeReference();
	}
	
	pRenderEnvMapFade = envmap;
	
	if( envmap ){
		envmap->AddReference();
		envmap->GetBillboardList().Add( this );
	}
	
	MarkTUCsDirty();
}

void deoglRBillboard::SetRenderEnvMapFadePerTime( float fadePerTime ){
	if( fadePerTime < 0.1f ){
		pRenderEnvMapFadePerTime = 0.1f;
		
	}else{
		pRenderEnvMapFadePerTime = fadePerTime;
	}
}

void deoglRBillboard::SetRenderEnvMapFadeFactor( float factor ){
	if( factor < 0.0f ){
		factor = 0.0f;
		
	}else if( factor > 1.0f ){
		factor = 1.0f;
	}
	
	if( fabsf( factor - pRenderEnvMapFadeFactor ) > 0.001f ){
		pRenderEnvMapFadeFactor = factor;
		
	}
}

void deoglRBillboard::WorldEnvMapLayoutChanged(){
	pDirtyRenderEnvMap = true;
}

void deoglRBillboard::UpdateRenderEnvMap(){
	if( ! pDirtyRenderEnvMap ){
		return;
	}
	
	if( ! pParentWorld ){
		DETHROW( deeInvalidParam );
	}
	
	// for the time being we simply pick the environment map that is closest to the billboard position.
	// this can lead to wrong picks and harshly switching environment maps but this is enough for the
	// first test.
	// 
	// for the time being we choose no region but visit all environment maps existing in the world.
	// a more optimal solution would be to search in an area somewhat larger than the distance to the
	// currently used environment map. the new environment map has to be closer than the current one
	// to be better. if no environment map exists yet a full search could be done.
	// 
	// better solutions could be using grid like connection between environment maps. this way the next
	// best environment map can be searched by following the grid.
	// 
	// for the time being the center of the bounding box is used as the reference point. for large
	// billboards we've got the problem of what environment map to use. this though has to be solved
	// later on most probably by splitting up the billboard into smaller billboards.
	deoglFindBestEnvMap visitor;
	decDVector position;
	
	position = ( pMinExtend + pMaxExtend ) * 0.5;
	
	visitor.SetPosition( position );
	//pParentWorld->VisitRegion( pMinExtend, pMaxExtend, visitor );
	visitor.VisitList( pParentWorld->GetEnvMapList() );
	
	if( visitor.GetEnvMap() ){
		SetRenderEnvMap( visitor.GetEnvMap() );
		
	}else if( pParentWorld->GetSkyEnvironmentMap() ){
		SetRenderEnvMap( pParentWorld->GetSkyEnvironmentMap() );
		
	}else{
		SetRenderEnvMap( NULL );
		SetRenderEnvMapFade( NULL );
		pRenderEnvMapFadeFactor = 1.0f;
	}
	//pOgl->LogInfoFormat( "update billboard %p render env map %p\n", pBillboard, pRenderEnvMap );
	
	pDirtyRenderEnvMap = false;
}

void deoglRBillboard::InvalidateRenderEnvMap(){
	if( ! pRenderEnvMap && ! pRenderEnvMapFade ){
		return;
	}
	
	SetRenderEnvMap( NULL );
	SetRenderEnvMapFade( NULL );
	pDirtyRenderEnvMap = true;
}

void deoglRBillboard::InvalidateRenderEnvMapIf( deoglEnvironmentMap *envmap ){
	if( pRenderEnvMap == envmap || pRenderEnvMapFade == envmap ){
		InvalidateRenderEnvMap();
	}
}

void deoglRBillboard::SetLLWorldPrev( deoglRBillboard *billboard ){
	pLLWorldPrev = billboard;
}

void deoglRBillboard::SetLLWorldNext( deoglRBillboard *billboard ){
	pLLWorldNext = billboard;
}



void deoglRBillboard::WorldReferencePointChanged(){
	MarkParamBlocksDirty();
	MarkParamBlocksDirty();
}



void deoglRBillboard::PrepareQuickDispose(){
	pParentWorld = NULL;
	pOctreeNode = NULL;
}



// Private Functions
//////////////////////

class deoglRBillboardDeletion : public deoglDelayedDeletion{
public:
	deoglSkinState *skinState;
	deoglSPBlockUBO *paramBlockSpecial;
	deoglSPBlockUBO *paramBlockDepth;
	deoglSPBlockUBO *paramBlockGeometry;
	deoglSPBlockUBO *paramBlockEnvMap;
	deoglTexUnitsConfig *tucDepth;
	deoglTexUnitsConfig *tucGeometry;
	deoglTexUnitsConfig *tucCounter;
	deoglTexUnitsConfig *tucEnvMap;
	
	deoglRBillboardDeletion() :
	skinState( NULL ),
	paramBlockSpecial( NULL ),
	paramBlockDepth( NULL ),
	paramBlockGeometry( NULL ),
	paramBlockEnvMap( NULL ),
	tucDepth( NULL ),
	tucGeometry( NULL ),
	tucCounter( NULL ),
	tucEnvMap( NULL ){
	}
	
	virtual ~deoglRBillboardDeletion(){
	}
	
	virtual void DeleteObjects( deoglRenderThread& ){
		if( tucDepth ){
			tucDepth->RemoveUsage();
		}
		if( tucGeometry ){
			tucGeometry->RemoveUsage();
		}
		if( tucCounter ){
			tucCounter->RemoveUsage();
		}
		if( tucEnvMap ){
			tucEnvMap->RemoveUsage();
		}
		if( paramBlockDepth ){
			paramBlockDepth->FreeReference();
		}
		if( paramBlockGeometry ){
			paramBlockGeometry->FreeReference();
		}
		if( paramBlockEnvMap ){
			paramBlockEnvMap->FreeReference();
		}
		if( paramBlockSpecial ){
			paramBlockSpecial->FreeReference();
		}
		if( skinState ){
			delete skinState;
		}
	}
};

void deoglRBillboard::pCleanUp(){
	SetParentWorld( NULL );
	
	if( pSkin ){
		pSkin->FreeReference();
	}
	if( pDynamicSkin ){
		pDynamicSkin->FreeReference();
	}
	if( pRenderEnvMap ){
		pRenderEnvMap->FreeReference();
	}
	if( pRenderEnvMapFade ){
		pRenderEnvMapFade->FreeReference();
	}
	
	if( pSharedSPBRTIGroup ){
		pSharedSPBRTIGroup->FreeReference();
	}
	if( pSharedSPBElement ){
		pSharedSPBElement->FreeReference();
	}
	
	// drop reference otherwise deletion can cause other deletions to be generated
	// causing a deletion race
	if( pSkinState ){
		pSkinState->DropDelayedDeletionObjects();
	}
	
	// delayed deletion of opengl containing objects
	deoglRBillboardDeletion *delayedDeletion = NULL;
	
	try{
		delayedDeletion = new deoglRBillboardDeletion;
		delayedDeletion->skinState = pSkinState;
		delayedDeletion->paramBlockSpecial = pParamBlockSpecial;
		delayedDeletion->paramBlockDepth = pParamBlockDepth;
		delayedDeletion->paramBlockEnvMap = pParamBlockEnvMap;
		delayedDeletion->paramBlockGeometry = pParamBlockGeometry;
		delayedDeletion->skinState = pSkinState;
		delayedDeletion->tucDepth = pTUCDepth;
		delayedDeletion->tucEnvMap = pTUCEnvMap;
		delayedDeletion->tucCounter = pTUCCounter;
		delayedDeletion->tucGeometry = pTUCGeometry;
		pRenderThread.GetDelayedOperations().AddDeletion( delayedDeletion );
		
	}catch( const deException &e ){
		if( delayedDeletion ){
			delete delayedDeletion;
		}
		pRenderThread.GetLogger().LogException( e );
		throw;
	}
}



void deoglRBillboard::pUpdateCullSphere(){
	deoglDCollisionSphere sphere;
	deoglDCollisionBox box;
	
	box.SetFromExtends( pMinExtend, pMaxExtend );
	box.GetEnclosingSphere( &sphere );
	
	pCullSphereCenter = sphere.GetCenter();
	pCullSphereRadius = ( float )sphere.GetRadius();
}

void deoglRBillboard::pUpdateRenderables(){
	// TODO this situation here is not blistering at all. usually there happens only one update
	// at the beginning of the billboard life time where the renderables are set up. then usually
	// no change happens at all anymore during the life time of the billboard. with the current
	// code though a lot of counter checking is done for all the billboards having a dynamic skin
	// to verify no change happened. a notification system would be a better here not only to
	// remove the counter handling but also to remove potential errors.
	
	// update dynamic skins to check
	if( pDynamicSkin && pSkin && pSkin->GetHasRenderables() ){
		const int updateNumber = pDynamicSkin->Update();
		if( updateNumber != pSkinState->GetUpdateNumber() ){
			pSkinState->SetUpdateNumber( updateNumber );
			pDirtyRenderables = true;
			if( pUseSkinTexture && pUseSkinTexture->GetDynamicChannels() ){
				MarkParamBlocksDirty();
				MarkTUCsDirty();
			}
		}
	}
	
	if( pDynamicSkin ){
		const int updateNumber = pDynamicSkin->Update();
		if( updateNumber != pSkinState->GetUpdateNumber() ){
			pSkinState->SetUpdateNumber( updateNumber );
			MarkParamBlocksDirty();
			MarkTUCsDirty();
			pDirtyRenderables = true;
		}
	}
	
	if( pDirtyRenderables ){
		pSkinState->RemoveAllRenderables();
		if( pSkin && pDynamicSkin ){
			pSkinState->AddRenderables( *pSkin, *pDynamicSkin );
		}
		
		MarkParamBlocksDirty();
		MarkTUCsDirty();
		
		pDirtyRenderables = false;
	}
}
