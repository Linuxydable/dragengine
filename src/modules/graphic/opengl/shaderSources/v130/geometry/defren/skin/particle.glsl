// request high precision if the graphic card supports this
#ifdef HIGH_PRECISION
precision highp float;
precision highp int;
#endif

// layout specifications
layout( points ) in;
layout( triangle_strip, max_vertices=4 ) out;

// some helper definitions to make the code easier to read
#if defined TEXTURE_EMISSIVITY || defined TEXTURE_RIM_EMISSIVITY
	#define WITH_EMISSIVITY 1
#endif
#if defined TEXTURE_ENVMAP || defined TEXTURE_RIM_EMISSIVITY
	#define WITH_REFLECT_DIR 1
#endif



// Uniform Parameters
///////////////////////

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/skin/ubo_render_parameters.glsl"
#include "v130/shared/defren/skin/ubo_instance_parameters.glsl"

#ifdef NODE_GEOMETRY_UNIFORMS
NODE_GEOMETRY_UNIFORMS
#endif



// Inputs
///////////

in vec3 vParticle0[ 1 ]; // size, emissivity, rotation
in vec4 vParticle1[ 1 ]; // red, green, blue, transparency

#ifdef SHARED_SPB
	flat in int vGSSPBIndex[ 1 ];
	#define spbIndex vGSSPBIndex[0]
	
	#ifdef GS_RENDER_CUBE
		flat in int vGSSPBFlags[ 1 ];
		#define spbFlags vGSSPBFlags[0]
	#endif
	
	#include "v130/shared/defren/skin/shared_spb_redirect.glsl"
#endif

#ifdef NODE_GEOMETRY_INPUTS
NODE_GEOMETRY_INPUTS
#endif



// Outputs
////////////

out vec2 vTCColor;
#ifdef TEXTURE_COLOR_TINT_MASK
	out vec2 vTCColorTintMask;
#endif
#ifdef TEXTURE_NORMAL
	out vec2 vTCNormal;
#endif
#ifdef TEXTURE_REFLECTIVITY
	out vec2 vTCReflectivity;
#endif
#ifdef WITH_EMISSIVITY
	out vec2 vTCEmissivity;
#endif
#ifdef TEXTURE_REFRACTION_DISTORT
	out vec2 vTCRefractionDistort;
#endif
#ifdef TEXTURE_AO
	out vec2 vTCAO;
#endif

out vec3 vNormal;
#ifdef TEXTURE_NORMAL
	out vec3 vTangent;
	out vec3 vBitangent;
#endif
#ifdef WITH_REFLECT_DIR
	out vec3 vReflectDir;
#endif
#ifdef FADEOUT_RANGE
	out float vFadeZ;
#endif

out vec4 vParticleColor; // from curve property
#ifdef WITH_EMISSIVITY
	out float vParticleEmissivity; // from curve property
#endif

#ifdef SHARED_SPB
	flat out int vSPBIndex;
#endif

#ifdef NODE_GEOMETRY_OUTPUTS
NODE_GEOMETRY_OUTPUTS
#endif



// Constants
//////////////

const vec2 bc[4] = vec2[4]( vec2( -0.5,  0.5 ), vec2( -0.5, -0.5 ), vec2(  0.5,  0.5 ), vec2(  0.5, -0.5 ) );
const vec2 tc[4] = vec2[4]( vec2(  0.0,  0.0 ), vec2(  0.0,  1.0 ), vec2(  1.0,  0.0 ), vec2(  1.0,  1.0 ) );



// Main Function
//////////////////

void main( void ){
	int i;
	
	// this is the same for all points
	vParticleColor = vParticle1[ 0 ];
	#ifdef WITH_EMISSIVITY
		vParticleEmissivity = vParticle0[ 0 ].y;
	#endif
	
	#ifdef SHARED_SPB
	vSPBIndex = spbIndex;
	#endif
	
	gl_PrimitiveID = gl_PrimitiveIDIn;
	gl_Layer = 0;
	
	// generate billboard
	vec4 tempRotMat; // cos(z), -sin(z), sin(z), cos(z)
	tempRotMat.xw = cos( vec2( vParticle0[ 0 ].z ) );
	tempRotMat.yz = sin( vec2( vParticle0[ 0 ].z ) ) * vec2( -1.0, 1.0 );
	tempRotMat *= vec4( vParticle0[ 0 ].x ); // tempRotMat * size
	mat2 rotmat = mat2( tempRotMat.x, tempRotMat.y, tempRotMat.z, tempRotMat.w );
	
	vec4 position = gl_in[ 0 ].gl_Position; // z and w stays the same for all vertices
	
	// calculate normal, tangent and bitangent. the same for all vertices
	vNormal = vec3( 0.0, 0.0, -1.0 );
	#ifdef TEXTURE_NORMAL
		vTangent = vec3( rotmat * vec2( 1.0, 0.0 ), 0.0 );
		vBitangent = vec3( rotmat * vec2( 0.0, -1.0 ), 0.0 );
	#endif
	
	// emit vertices
	for( i=0; i<4; i++ ){
		position.xy = rotmat * bc[i] + gl_in[ 0 ].gl_Position.xy;
		gl_Position = pMatrixP * position;
		
		vTCColor = tc[i];
		#ifdef TEXTURE_COLOR_TINT_MASK
			vTCColorTintMask = tc[i];
		#endif
		#ifdef TEXTURE_NORMAL
			vTCNormal = tc[i];
		#endif
		#ifdef TEXTURE_REFLECTIVITY
			vTCReflectivity = tc[i];
		#endif
		#ifdef WITH_EMISSIVITY
			vTCEmissivity = tc[i];
		#endif
		#ifdef TEXTURE_REFRACTION_DISTORT
			vTCRefractionDistort = tc[i];
		#endif
		#ifdef TEXTURE_AO
			vTCAO = tc[i];
		#endif
		
		#ifdef WITH_REFLECT_DIR
			vReflectDir = position.xyz;
		#endif
		#ifdef FADEOUT_RANGE
			vFadeZ = position.z;
		#endif
		
		EmitVertex();
	}
	
	EndPrimitive();
}
