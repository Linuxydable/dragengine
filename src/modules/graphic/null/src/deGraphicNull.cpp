/* 
 * Drag[en]gine Null Graphic Module
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
#include <string.h>

#include "deGraphicNull.h"

#include <dragengine/common/exceptions.h>



// Export definition
//////////////////////

#ifdef __cplusplus
extern "C" {
#endif
MOD_ENTRY_POINT_ATTR deBaseModule *NullGraphicCreateModule( deLoadableModule *loadableModule );
#ifdef  __cplusplus
}
#endif



// Entry Function
///////////////////

deBaseModule *NullGraphicCreateModule( deLoadableModule *loadableModule ){
	deBaseModule *module = NULL;
	
	try{
		module = new deGraphicNull( *loadableModule );
		
	}catch( const deException & ){
		return NULL;
	}
	
	return module;
}



// Class deGraphicNull
////////////////////////

// Constructor, destructor
////////////////////////////

deGraphicNull::deGraphicNull( deLoadableModule &loadableModule ) :
deBaseGraphicModule( loadableModule ){
}

deGraphicNull::~deGraphicNull(){
}



// Management
///////////////

bool deGraphicNull::Init( deRenderWindow *renderWindow ){
	return true;
}

void deGraphicNull::CleanUp(){
}

void deGraphicNull::SetRenderWindow( deRenderWindow *renderWindow ){
}

#ifdef ANDROID
void deGraphicNull::InitAppWindow(){
}

void deGraphicNull::TerminateAppWindow(){
}
#endif



void deGraphicNull::RenderWindows(){
}

void deGraphicNull::InputOverlayCanvasChanged(){
}


deBaseGraphicCanvas *deGraphicNull::CreateCanvas( deCanvas* ){
	return NULL;
}

deBaseGraphicCaptureCanvas *deGraphicNull::CreateCaptureCanvas( deCaptureCanvas* ){
	return NULL;
}

deBaseGraphicDynamicSkin *deGraphicNull::CreateDynamicSkin( deDynamicSkin* ){
	return NULL;
}

deBaseGraphicEnvMapProbe *deGraphicNull::CreateEnvMapProbe( deEnvMapProbe* ){
	return NULL;
}

deBaseGraphicOcclusionMesh *deGraphicNull::CreateOcclusionMesh( deOcclusionMesh* ){
	return NULL;
}

deBaseGraphicModel *deGraphicNull::CreateModel( deModel* ){
	return NULL;
}

deBaseGraphicSkin *deGraphicNull::CreateSkin( deSkin* ){
	return NULL;
}

deBaseGraphicImage *deGraphicNull::CreateImage( deImage* ){
	return NULL;
}

deBaseGraphicComponent *deGraphicNull::CreateComponent( deComponent* ){
	return NULL;
}

deBaseGraphicLight *deGraphicNull::CreateLight( deLight* ){
	return NULL;
}

deBaseGraphicFont *deGraphicNull::CreateFont( deFont* ){
	return NULL;
}

deBaseGraphicSky *deGraphicNull::CreateSky( deSky* ){
	return NULL;
}

deBaseGraphicSkyInstance *deGraphicNull::CreateSkyInstance( deSkyInstance* ){
	return NULL;
}

deBaseGraphicWorld *deGraphicNull::CreateWorld( deWorld* ){
	return NULL;
}

deBaseGraphicEffect *deGraphicNull::CreateEffect( deEffect* ){
	return NULL;
}

deBaseGraphicDebugDrawer *deGraphicNull::CreateDebugDrawer( deDebugDrawer* ){
	return NULL;
}

deBaseGraphicRenderWindow *deGraphicNull::CreateRenderWindow( deRenderWindow* ){
	return NULL;
}

deBaseGraphicBillboard *deGraphicNull::CreateBillboard( deBillboard* ){
	return NULL;
}

deBaseGraphicDecal *deGraphicNull::CreateDecal( deDecal* ){
	return NULL;
}

deBaseGraphicCamera *deGraphicNull::CreateCamera( deCamera* ){
	return NULL;
}

deBaseGraphicLumimeter *deGraphicNull::CreateLumimeter( deLumimeter* ){
	return NULL;
}

deBaseGraphicHeightTerrain *deGraphicNull::CreateHeightTerrain( deHeightTerrain* ){
	return NULL;
}

deBaseGraphicPropField *deGraphicNull::CreatePropField( dePropField* ){
	return NULL;
}

deBaseGraphicParticleEmitter *deGraphicNull::CreateParticleEmitter( deParticleEmitter* ){
	return NULL;
}

deBaseGraphicParticleEmitterInstance *deGraphicNull::CreateParticleEmitterInstance( deParticleEmitterInstance* ){
	return NULL;
}

deBaseGraphicSmokeEmitter *deGraphicNull::CreateSmokeEmitter( deSmokeEmitter* ){
	return NULL;
}

deBaseGraphicVideo *deGraphicNull::CreateVideo( deVideo* ){
	return NULL;
}

deBaseGraphicVideoPlayer *deGraphicNull::CreateVideoPlayer( deVideoPlayer* ){
	return NULL;
}
