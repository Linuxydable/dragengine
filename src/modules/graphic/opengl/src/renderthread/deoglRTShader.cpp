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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deoglRTShader.h"
#include "deoglRenderThread.h"
#include "../light/shader/deoglLightShaderManager.h"
#include "../shaders/deoglShaderCompiled.h"
#include "../shaders/deoglShaderManager.h"
#include "../shaders/deoglShaderProgram.h"
#include "../skin/shader/deoglSkinShaderManager.h"
#include "../texture/deoglTextureStageManager.h"
#include "../texture/texsamplerconfig/deoglTexSamplerConfig.h"
#include "../texture/texunitsconfig/deoglTexUnitsConfigList.h"

#include <dragengine/common/exceptions.h>



// Class deoglRTShader
////////////////////////

// Constructor, destructor
////////////////////////////

deoglRTShader::deoglRTShader( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),

pTexUnitsConfigList( NULL ),

pShaderManager( NULL ),
pSkinShaderManager( NULL ),
pLightShaderManager( NULL ),
pCurShaderProg( NULL )
{
	int i;
	for( i=0; i<ETSC_COUNT; i++ ){
		pTexSamplerConfigs[ i ] = NULL;
	}
	
	try{
		pTexUnitsConfigList = new deoglTexUnitsConfigList( renderThread );
		pCreateTexSamplerConfigs();
		
		pShaderManager = new deoglShaderManager( renderThread );
		pShaderManager->LoadUnitSourceCodes();
		pShaderManager->LoadSources();
		
		pSkinShaderManager = new deoglSkinShaderManager( renderThread );
		pLightShaderManager = new deoglLightShaderManager( renderThread );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglRTShader::~deoglRTShader(){
	pCleanUp();
}



// Management
///////////////

deoglTexSamplerConfig *deoglRTShader::GetTexSamplerConfig( const deoglRTShader::eTextureSamplerConfigurations type ) const{
	if( type < 0 || type >= ETSC_COUNT ){
		DETHROW( deeInvalidParam );
	}
	
	return pTexSamplerConfigs[ type ];
}

void deoglRTShader::ActivateShader( deoglShaderProgram *shader ){
	if( shader == pCurShaderProg ){
		return;
	}
	
	if( shader ){
		deoglShaderCompiled * const compiled = shader->GetCompiled();
		if( ! compiled ){
			DETHROW( deeInvalidParam );
		}
		
		compiled->Activate();
		
	}else{
		OGL_CHECK( pRenderThread, pglUseProgram( 0 ) );
	}
	
	pCurShaderProg = shader;
}



// Private Functions
//////////////////////

void deoglRTShader::pCleanUp(){
	int i;
	
	if( pLightShaderManager ){
		delete pLightShaderManager;
	}
	if( pSkinShaderManager ){
		delete pSkinShaderManager;
	}
	if( pShaderManager ){
		delete pShaderManager;
	}
	
	for( i=0; i<ETSC_COUNT; i++ ){
		if( pTexSamplerConfigs[ i ] ){
			delete pTexSamplerConfigs[ i ];
		}
	}
	
	if( pTexUnitsConfigList ){
		delete pTexUnitsConfigList;
	}
}



void deoglRTShader::pCreateTexSamplerConfigs(){
	pTexSamplerConfigs[ etscClampNearest ] = new deoglTexSamplerConfig( pRenderThread );
	pTexSamplerConfigs[ etscClampNearest ]->SetFilterMode( deoglTextureStageManager::etfNearest );
	pTexSamplerConfigs[ etscClampNearest ]->SetWrapMode( GL_CLAMP_TO_EDGE );
	pTexSamplerConfigs[ etscClampNearest ]->SetDepthCompareMode( false );
	
	pTexSamplerConfigs[ etscClampLinear ] = new deoglTexSamplerConfig( pRenderThread );
	pTexSamplerConfigs[ etscClampLinear ]->SetFilterMode( deoglTextureStageManager::etfLinear );
	pTexSamplerConfigs[ etscClampLinear ]->SetWrapMode( GL_CLAMP_TO_EDGE );
	pTexSamplerConfigs[ etscClampLinear ]->SetDepthCompareMode( false );
	
	pTexSamplerConfigs[ etscClampNearestMipMap ] = new deoglTexSamplerConfig( pRenderThread );
	pTexSamplerConfigs[ etscClampNearestMipMap ]->SetFilterMode( deoglTextureStageManager::etfNearestMipMap );
	pTexSamplerConfigs[ etscClampNearestMipMap ]->SetWrapMode( GL_CLAMP_TO_EDGE );
	pTexSamplerConfigs[ etscClampNearestMipMap ]->SetDepthCompareMode( false );
	
	pTexSamplerConfigs[ etscClampLinearMipMap ] = new deoglTexSamplerConfig( pRenderThread );
	pTexSamplerConfigs[ etscClampLinearMipMap ]->SetFilterMode( deoglTextureStageManager::etfLinearMipMap );
	pTexSamplerConfigs[ etscClampLinearMipMap ]->SetWrapMode( GL_CLAMP_TO_EDGE );
	pTexSamplerConfigs[ etscClampLinearMipMap ]->SetDepthCompareMode( false );
	
	pTexSamplerConfigs[ etscClampLinearMipMapNearest ] = new deoglTexSamplerConfig( pRenderThread );
	pTexSamplerConfigs[ etscClampLinearMipMapNearest ]->SetFilterMode( deoglTextureStageManager::etfLinearMipMapNearest );
	pTexSamplerConfigs[ etscClampLinearMipMapNearest ]->SetWrapMode( GL_CLAMP_TO_EDGE );
	pTexSamplerConfigs[ etscClampLinearMipMapNearest ]->SetDepthCompareMode( false );
	
	pTexSamplerConfigs[ etscRepeatNearest ] = new deoglTexSamplerConfig( pRenderThread );
	pTexSamplerConfigs[ etscRepeatNearest ]->SetFilterMode( deoglTextureStageManager::etfNearest );
	pTexSamplerConfigs[ etscRepeatNearest ]->SetWrapMode( GL_REPEAT );
	pTexSamplerConfigs[ etscRepeatNearest ]->SetDepthCompareMode( false );
	
	pTexSamplerConfigs[ etscRepeatLinear ] = new deoglTexSamplerConfig( pRenderThread );
	pTexSamplerConfigs[ etscRepeatLinear ]->SetFilterMode( deoglTextureStageManager::etfLinear );
	pTexSamplerConfigs[ etscRepeatLinear ]->SetWrapMode( GL_REPEAT );
	pTexSamplerConfigs[ etscRepeatLinear ]->SetDepthCompareMode( false );
	
	pTexSamplerConfigs[ etscRepeatLinearMipMap ] = new deoglTexSamplerConfig( pRenderThread );
	pTexSamplerConfigs[ etscRepeatLinearMipMap ]->SetFilterMode( deoglTextureStageManager::etfLinearMipMap );
	pTexSamplerConfigs[ etscRepeatLinearMipMap ]->SetWrapMode( GL_REPEAT );
	pTexSamplerConfigs[ etscRepeatLinearMipMap ]->SetDepthCompareMode( false );
	
	pTexSamplerConfigs[ etscShadowClampNearest ] = new deoglTexSamplerConfig( pRenderThread );
	pTexSamplerConfigs[ etscShadowClampNearest ]->SetFilterMode( deoglTextureStageManager::etfNearest );
	pTexSamplerConfigs[ etscShadowClampNearest ]->SetWrapMode( GL_CLAMP_TO_EDGE );
	pTexSamplerConfigs[ etscShadowClampNearest ]->SetDepthCompareMode( true );
	
	pTexSamplerConfigs[ etscShadowClampLinear ] = new deoglTexSamplerConfig( pRenderThread );
	pTexSamplerConfigs[ etscShadowClampLinear ]->SetFilterMode( deoglTextureStageManager::etfLinear );
	pTexSamplerConfigs[ etscShadowClampLinear ]->SetWrapMode( GL_CLAMP_TO_EDGE );
	pTexSamplerConfigs[ etscShadowClampLinear ]->SetDepthCompareMode( true );
	
	pTexSamplerConfigs[ etscShadowClampLinearInverse ] = new deoglTexSamplerConfig( pRenderThread );
	pTexSamplerConfigs[ etscShadowClampLinearInverse ]->SetFilterMode( deoglTextureStageManager::etfLinear );
	pTexSamplerConfigs[ etscShadowClampLinearInverse ]->SetWrapMode( GL_CLAMP_TO_EDGE );
	pTexSamplerConfigs[ etscShadowClampLinearInverse ]->SetDepthCompareMode( true );
	pTexSamplerConfigs[ etscShadowClampLinearInverse ]->SetDepthCompareFunc( GL_GEQUAL );
	
	pTexSamplerConfigs[ etscShadowRepeatNearest ] = new deoglTexSamplerConfig( pRenderThread );
	pTexSamplerConfigs[ etscShadowRepeatNearest ]->SetFilterMode( deoglTextureStageManager::etfNearest );
	pTexSamplerConfigs[ etscShadowRepeatNearest ]->SetWrapMode( GL_REPEAT );
	pTexSamplerConfigs[ etscShadowRepeatNearest ]->SetDepthCompareMode( true );
	
	pTexSamplerConfigs[ etscShadowRepeatLinear ] = new deoglTexSamplerConfig( pRenderThread );
	pTexSamplerConfigs[ etscShadowRepeatLinear ]->SetFilterMode( deoglTextureStageManager::etfLinear );
	pTexSamplerConfigs[ etscShadowRepeatLinear ]->SetWrapMode( GL_REPEAT );
	pTexSamplerConfigs[ etscShadowRepeatLinear ]->SetDepthCompareMode( true );
}
