/* 
 * Drag[en]gine OpenAL Audio Module
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

#ifndef _DEOALENVIRONMENT_H_
#define _DEOALENVIRONMENT_H_

#include "deoalEnvProbeListenerSmooth.h"
#include "raytrace/deoalImpulseResponse.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/utils/decLayerMask.h>

class deoalAudioThread;
class deoalAWorld;
class deoalAMicrophone;
class deoalRayTraceHitElement;
class deoalRayTraceResult;
class deoalEnergyHistogram;
class deoalEnvProbe;
class deoalEnvironmentDebug;

class deDebugBlockInfo;
class deDebugDrawer;



/**
 * \brief Environment tracking for speakers.
 * 
 * Keeps track of the environment a speaker is located in. Stores the effect parameters
 * required to be assigned to a speaker to give the appropriate impression.
 * 
 * Instances of this object are typically stored in active speakers and regularily
 * updated to reflect changes in the environment.
 */
class deoalEnvironment{
private:
	deoalAudioThread &pAudioThread;
	
	deoalAWorld *pWorld;
	decDVector pPosition;
	float pRange;
	float pRangeSquared;
	float pAttenuationRefDist;
	float pAttenuationRolloff;
	decLayerMask pLayerMask;
	
	float pGainLow;
	float pGainMedium;
	float pGainHigh;
	
	float pBandPassGain;
	float pBandPassGainLF;
	float pBandPassGainHF;
	
	float pReverbGain;
	float pReverbGainLF;
	float pReverbGainHF;
	float pReverbDecayTime;
	float pReverbDecayHFRatio;
	float pReverbDecayLFRatio;
	float pReverbReflectionGain;
	float pReverbReflectionDelay;
	decVector pReverbReflectionPan;
	float pReverbLateReverbGain;
	float pReverbLateReverbDelay;
	decVector pReverbLateReverbPan;
	float pReverbEchoTime;
	
	deoalEnvProbeListenerSmooth pListenerSmooth;
	bool pResetListenerSmooth;
	
	deoalEnvProbe *pEnvProbe;
	
	deoalEnvironmentDebug *pDebug;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create environment tracker. */
	deoalEnvironment( deoalAudioThread &audioThread );
	
	/** \brief Clean up environment. */
	~deoalEnvironment();
	/*@}*/
	
	
	
	/** \name Manegement */
	/*@{*/
	/** \brief Audio thread. */
	inline deoalAudioThread &GetAudioThread() const{ return pAudioThread; }
	
	/** \brief World or NULL. */
	inline deoalAWorld *GetWorld() const{ return pWorld; }
	
	/** \brief Set world or NULL. */
	void SetWorld( deoalAWorld *world );
	
	/** \brief Position of speaker. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	
	/** \brief Set position of speaker. */
	void SetPosition( const decDVector &position );
	
	/** \brief Range of speaker. */
	inline float GetRange() const{ return pRange; }
	
	/** \brief Range squared of speaker. */
	inline float GetRangeSquared() const{ return pRangeSquared; }
	
	/** \brief Set range of speaker. */
	void SetRange( float range );
	
	/** \brief Attenuation reference distance. */
	inline float GetAttenuationRefDist() const{ return pAttenuationRefDist; }
	
	/** \brief Attenuation rolloff. */
	inline float GetAttenuationRolloff() const{ return pAttenuationRolloff; }
	
	/** \brief Set attenuation parameters. */
	void SetAttenuation( float refDist, float rolloff );
	
	/** \brief Layer mask. */
	inline const decLayerMask &GetLayerMask() const{ return pLayerMask; }
	
	/** \brief Set layer mask. */
	void SetLayerMask( const decLayerMask &layerMask );
	
	
	
	/** \brief Low frequency gain. */
	inline float GetGainLow() const{ return pGainLow; }
	
	/** \brief Medium frequency gain. */
	inline float GetGainMedium() const{ return pGainMedium; }
	
	/** \brief High frequency gain. */
	inline float GetGainHigh() const{ return pGainHigh; }
	
	
	
	/** \brief Band pass gain value. */
	inline float GetBandPassGain() const{ return pBandPassGain; }
	
	/** \brief Band pass low frequency gain value. */
	inline float GetBandPassGainLF() const{ return pBandPassGainLF; }
	
	/** \brief Band pass high frequency gain value. */
	inline float GetBandPassGainHF() const{ return pBandPassGainHF; }
	
	
	
	/** \brief Reverb effect gain value. */
	inline float GetReverbGain() const{ return pReverbGain; }
	
	/** \brief Reverb effect low frequency gain value. */
	inline float GetReverbGainLF() const{ return pReverbGainLF; }
	
	/** \brief Reverb effect high frequency gain value. */
	inline float GetReverbGainHF() const{ return pReverbGainHF; }
	
	/** \brief Reverb effect decay time value. */
	inline float GetReverbDecayTime() const{ return pReverbDecayTime; }
	
	/** \brief Reverb effect decay lf ratio value. */
	inline float GetReverbDecayLFRatio() const{ return pReverbDecayLFRatio; }
	
	/** \brief Reverb effect decay hf ratio value. */
	inline float GetReverbDecayHFRatio() const{ return pReverbDecayHFRatio; }
	
	/** \brief Reverb effect reflection gain value. */
	inline float GetReverbReflectionGain() const{ return pReverbReflectionGain; }
	
	/** \brief Reverb effect reflection delay value. */
	inline float GetReverbReflectionDelay() const{ return pReverbReflectionDelay; }
	
	/** \brief Reverb effect reflection pan direction. */
	inline const decVector &GetReverbReflectionPan() const{ return pReverbReflectionPan; }
	
	/** \brief Reverb effect late reverb gain value. */
	inline float GetReverbLateReverbGain() const{ return pReverbLateReverbGain; }
	
	/** \brief Reverb effect late reverb delay value. */
	inline float GetReverbLateReverbDelay() const{ return pReverbLateReverbDelay; }
	
	/** \brief Reverb effect late reverb pan direction. */
	inline const decVector &GetReverbLateReverbPan() const{ return pReverbLateReverbPan; }
	
	/** \brief Reverbe effect Echo time. */
	inline float GetReverbEchoTime() const{ return pReverbEchoTime; }
	
	
	
	/** \brief Update environment. */
	void Update();
	
	
	
	/** \brief Debug print. */
	void DebugPrint();
	
	/**
	 * \brief Update debug information.
	 * \warning Called during synchronization time from main thread.
	 */
	void DebugUpdateInfo( deDebugBlockInfo &debugInfo );
	void DebugUpdateDirect( deDebugDrawer &debugDrawer, const deoalAMicrophone &microphone );
	void DebugSoundRays( deDebugDrawer &debugDrawer );
	/*@}*/
	
	
	
private:
	void pDirectPath( const deoalAMicrophone &microphone, const decDVector &micPos );
	void pEnvReflection( deoalAMicrophone &microphone, const decDVector &micPos,
		const decQuaternion &micOrient );
	void pSetSilent();
	void pCalcEffectParameters();
};

#endif
