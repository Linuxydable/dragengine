/* 
 * Drag[en]gine DragonScript Script Module
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
#include <stdint.h>
#include <string.h>

#include "deClassSSWave.h"
#include "../deClassSynthesizer.h"
#include "../deClassSynthesizerSource.h"
#include "../../../deScriptingDragonScript.h"
#include "../../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/synthesizer/deSynthesizer.h>
#include <dragengine/resources/synthesizer/deSynthesizerLink.h>
#include <dragengine/resources/synthesizer/source/deSynthesizerSourceWave.h>

#include <libdscript/exceptions.h>
#include <libdscript/packages/default/dsClassEnumeration.h>



// Native structure
/////////////////////

struct sSSWaveNatDat{
	deSynthesizer *synthesizer;
	deSynthesizerSourceWave *source;
};



// Native Functions
/////////////////////

// public func new()
deClassSSWave::nfNew::nfNew( const sInitData &init ) : dsFunction( init.clsSSWave,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassSSWave::nfNew::RunFunction( dsRunTime *rt, dsValue *myself ){
	sSSWaveNatDat &nd = *( ( sSSWaveNatDat* )p_GetNativeData( myself ) );
	
	// clear ( important )
	nd.synthesizer = NULL;
	nd.source = NULL;
	
	// super call
	deClassSynthesizerSource * const baseClass = ( deClassSynthesizerSource* )GetOwnerClass()->GetBaseClass();
	baseClass->CallBaseClassConstructor( rt, myself, baseClass->GetFirstConstructor(), 0 );
	
	// create synthesizer source
	nd.source = new deSynthesizerSourceWave;
	baseClass->AssignSource( myself->GetRealObject(), nd.source );
}

// public func destructor()
deClassSSWave::nfDestructor::nfDestructor( const sInitData &init ) : dsFunction( init.clsSSWave,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassSSWave::nfDestructor::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( myself->GetRealObject()->GetRefCount() != 1 ){
		return; // protected against GC cleaning up leaking
	}
	
	sSSWaveNatDat &nd = *( ( sSSWaveNatDat* )p_GetNativeData( myself ) );
	
	if( nd.synthesizer ){
		nd.synthesizer->FreeReference();
		nd.synthesizer = NULL;
	}
	
	if( nd.source ){
		nd.source->FreeReference();
		nd.source = NULL;
	}
}



// public func void targetAddLink( SSWaveTarget target, int link )
deClassSSWave::nfTargetAddLink::nfTargetAddLink( const sInitData &init ) : dsFunction( init.clsSSWave,
"targetAddLink", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsSSWaveTarget ); // target
	p_AddParameter( init.clsInt ); // link
}
void deClassSSWave::nfTargetAddLink::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( ! rt->GetValue( 0 )->GetRealObject() ){
		DSTHROW( dueNullPointer );
	}
	
	sSSWaveNatDat &nd = *( ( sSSWaveNatDat* )p_GetNativeData( myself ) );
	const deClassSSWave::eTargets target = ( deClassSSWave::eTargets )
		( ( dsClassEnumeration* )rt->GetEngine()->GetClassEnumeration() )->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() );
	const int link = rt->GetValue( 1 )->GetInt();
	
	switch( target ){
	case deClassSSWave::etBlendFactor:
		nd.source->GetTargetBlendFactor().AddLink( link );
		break;
		
	case deClassSSWave::etVolume:
		nd.source->GetTargetVolume().AddLink( link );
		break;
		
	case deClassSSWave::etPanning:
		nd.source->GetTargetPanning().AddLink( link );
		break;
		
	case deClassSSWave::etFrequency:
		nd.source->GetTargetFrequency().AddLink( link );
		break;
		
	default:
		DSTHROW( dueInvalidParam );
	}
	
	if( nd.synthesizer ){
		nd.synthesizer->NotifySourcesChanged();
	}
}

// public func void targetRemoveAllLinks( SSWaveTarget target )
deClassSSWave::nfTargetRemoveAllLinks::nfTargetRemoveAllLinks( const sInitData &init ) : dsFunction( init.clsSSWave,
"targetRemoveAllLinks", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsSSWaveTarget ); // target
}
void deClassSSWave::nfTargetRemoveAllLinks::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( ! rt->GetValue( 0 )->GetRealObject() ){
		DSTHROW( dueNullPointer );
	}
	
	sSSWaveNatDat &nd = *( ( sSSWaveNatDat* )p_GetNativeData( myself ) );
	const deClassSSWave::eTargets target = ( deClassSSWave::eTargets )
		( ( dsClassEnumeration* )rt->GetEngine()->GetClassEnumeration() )->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() );
	
	switch( target ){
	case deClassSSWave::etBlendFactor:
		nd.source->GetTargetBlendFactor().RemoveAllLinks();
		break;
		
	case deClassSSWave::etVolume:
		nd.source->GetTargetVolume().RemoveAllLinks();
		break;
		
	case deClassSSWave::etPanning:
		nd.source->GetTargetPanning().RemoveAllLinks();
		break;
		
	case deClassSSWave::etFrequency:
		nd.source->GetTargetFrequency().RemoveAllLinks();
		break;
		
	default:
		DSTHROW( dueInvalidParam );
	}
	
	if( nd.synthesizer ){
		nd.synthesizer->NotifySourcesChanged();
	}
}



// public func void setType( SSWaveType type )
deClassSSWave::nfSetType::nfSetType( const sInitData &init ) : dsFunction( init.clsSSWave,
"setType", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsSSWaveType ); // type
}
void deClassSSWave::nfSetType::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( ! rt->GetValue( 0 )->GetRealObject() ){
		DSTHROW( dueNullPointer );
	}
	
	sSSWaveNatDat &nd = *( ( sSSWaveNatDat* )p_GetNativeData( myself ) );
	const deSynthesizerSourceWave::eWaveType type = ( deSynthesizerSourceWave::eWaveType )
		( ( dsClassEnumeration* )rt->GetEngine()->GetClassEnumeration() )->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() );
	
	if( type == nd.source->GetType() ){
		return;
	}
	
	nd.source->SetType( type );
	
	if( nd.synthesizer ){
		nd.synthesizer->NotifySourcesChanged();
	}
}

// public func void setMinFrequency( float frequency )
deClassSSWave::nfSetMinFrequency::nfSetMinFrequency( const sInitData &init ) : dsFunction( init.clsSSWave,
"setMinFrequency", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // frequency
}
void deClassSSWave::nfSetMinFrequency::RunFunction( dsRunTime *rt, dsValue *myself ){
	sSSWaveNatDat &nd = *( ( sSSWaveNatDat* )p_GetNativeData( myself ) );
	const float frequency = rt->GetValue( 0 )->GetFloat();
	
	if( fabsf( frequency - nd.source->GetMinFrequency() ) <= FLOAT_SAFE_EPSILON ){
		return;
	}
	
	nd.source->SetMinFrequency( frequency );
	
	if( nd.synthesizer ){
		nd.synthesizer->NotifySourcesChanged();
	}
}

// public func void setMaxFrequency( float frequency )
deClassSSWave::nfSetMaxFrequency::nfSetMaxFrequency( const sInitData &init ) : dsFunction( init.clsSSWave,
"setMaxFrequency", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // frequency
}
void deClassSSWave::nfSetMaxFrequency::RunFunction( dsRunTime *rt, dsValue *myself ){
	sSSWaveNatDat &nd = *( ( sSSWaveNatDat* )p_GetNativeData( myself ) );
	const float frequency = rt->GetValue( 0 )->GetFloat();
	
	if( fabsf( frequency - nd.source->GetMaxFrequency() ) <= FLOAT_SAFE_EPSILON ){
		return;
	}
	
	nd.source->SetMaxFrequency( frequency );
	
	if( nd.synthesizer ){
		nd.synthesizer->NotifySourcesChanged();
	}
}



// Class deClassSSWave
/////////////////////////

// Constructor
////////////////

deClassSSWave::deClassSSWave( deScriptingDragonScript &ds ) :
dsClass( "SSWave", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE ),
pDS( ds ){
	GetParserInfo()->SetParent( DENS_SCENERY );
	GetParserInfo()->SetBase( "SynthesizerSource" );
	
	p_SetNativeDataSize( sizeof( sSSWaveNatDat ) );
}

deClassSSWave::~deClassSSWave(){
}



// Management
///////////////

void deClassSSWave::CreateClassMembers( dsEngine *engine ){
	pClsSSWaveTarget = engine->GetClass( "Dragengine.Scenery.SSWaveTarget" );
	pClsSSWaveType = engine->GetClass( "Dragengine.Scenery.SSWaveType" );
	
	sInitData init;
	init.clsSSWave = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFloat = engine->GetClassFloat();
	init.clsString = engine->GetClassString();
	init.clsBool = engine->GetClassBool();
	init.clsObject = engine->GetClassObject();
	init.clsSSWaveTarget = pClsSSWaveTarget;
	init.clsSSWaveType = pClsSSWaveType;
	
	// add functions
	AddFunction( new nfNew( init ) );
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfTargetAddLink( init ) );
	AddFunction( new nfTargetRemoveAllLinks( init ) );
	
	AddFunction( new nfSetType( init ) );
	AddFunction( new nfSetMinFrequency( init ) );
	AddFunction( new nfSetMaxFrequency( init ) );
	
	// calculate member offsets
	CalcMemberOffsets();
}

deSynthesizerSourceWave *deClassSSWave::GetSource( dsRealObject *myself ) const{
	if( ! myself ){
		return NULL;
	}
	
	return ( ( sSSWaveNatDat* )p_GetNativeData( myself->GetBuffer() ) )->source;
}

void deClassSSWave::AssignSynthesizer( dsRealObject *myself, deSynthesizer *synthesizer ){
	if( ! myself ){
		DSTHROW( dueInvalidParam );
	}
	
	pDS.GetClassSynthesizerSource()->AssignSynthesizer( myself, synthesizer );
	
	sSSWaveNatDat &nd = *( ( sSSWaveNatDat* )p_GetNativeData( myself->GetBuffer() ) );
	
	if( synthesizer == nd.synthesizer ){
		return;
	}
	
	if( nd.synthesizer ){
		nd.synthesizer->FreeReference();
	}
	
	nd.synthesizer = synthesizer;
	
	if( synthesizer ){
		synthesizer->AddReference();
	}
}

void deClassSSWave::PushSource( dsRunTime *rt, deSynthesizer *synthesizer, deSynthesizerSourceWave *source ){
	if( ! rt ){
		DSTHROW( dueInvalidParam );
	}
	
	if( ! source ){
		rt->PushObject( NULL, this );
		return;
	}
	
	deClassSynthesizerSource * const baseClass = ( deClassSynthesizerSource* )GetBaseClass();
	rt->CreateObjectNakedOnStack( this );
	sSSWaveNatDat &nd = *( ( sSSWaveNatDat* )p_GetNativeData( rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) );
	nd.synthesizer = NULL;
	nd.source = NULL;
	
	try{
		baseClass->CallBaseClassConstructor( rt, rt->GetValue( 0 ), baseClass->GetFirstConstructor(), 0 );
		
		nd.synthesizer = synthesizer;
		if( synthesizer ){
			synthesizer->AddReference();
		}
		
		nd.source = source;
		source->AddReference();
		
		baseClass->AssignSource( rt->GetValue( 0 )->GetRealObject(), source );
		baseClass->AssignSynthesizer( rt->GetValue( 0 )->GetRealObject(), synthesizer );
		
	}catch( ... ){
		rt->RemoveValues( 1 ); // remove pushed object
		throw;
	}
}
