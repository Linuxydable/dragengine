#ifdef GS_RENDER_CUBE_INSTANCING
	#extension GL_ARB_gpu_shader5 : require
#endif

// request high precision if the graphic card supports this
#ifdef HIGH_PRECISION
precision highp float;
precision highp int;
#endif

// some helper definitions to make the code easier to read
#if defined OUTPUT_COLOR || defined TEXTURE_SOLIDITY || defined TEXTURE_HEIGHT
	#define _REQ_TEX_CLR_1 1
#endif
#if defined TEXTURE_EMISSIVITY || defined TEXTURE_RIM_EMISSIVITY
	#define _REQ_TEX_CLR_2 1
#endif
#if defined _REQ_TEX_CLR_1 || defined _REQ_TEX_CLR_2
	#define REQUIRES_TEX_COLOR 1
#endif

#if defined TEXTURE_HEIGHT || defined TEXTURE_RIM_EMISSIVITY
	#define REQUIRES_NORMAL 1
#endif

// layout definitions
#ifdef GS_RENDER_CUBE
	#ifdef GS_RENDER_CUBE_INSTANCING
		layout( triangles, invocations=6 ) in;
		layout( triangle_strip, max_vertices=3 ) out;
	#else
		layout( triangles ) in;
		layout( triangle_strip, max_vertices=18 ) out;
	#endif
#endif

// some helper definitions to make the code easier to read
#if defined TEXTURE_SOLIDITY || defined OUTPUT_COLOR
	#define REQUIRES_TEX_COLOR 1
#endif



// Uniform Parameters
///////////////////////

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/skin/ubo_render_parameters.glsl"
#ifdef SHARED_SPB
	#include "v130/shared/defren/skin/ubo_instance_parameters.glsl"
#endif



// Inputs
///////////

#ifdef REQUIRES_TEX_COLOR
	in vec2 vGSTCColor[ 3 ];
#endif
#ifdef CLIP_PLANE
	in vec3 vGSClipCoord[ 3 ];
#endif
#ifdef DEPTH_DISTANCE
	in vec3 vGSPosition[ 3 ];
#endif
#ifdef HEIGHT_MAP
	in float vGSHTMask[ 3 ];
#endif
#ifdef REQUIRES_NORMAL
	in vec3 vGSNormal[ 3 ];
	#ifdef TEXTURE_NORMAL
		in vec3 vGSTangent[ 3 ];
		in vec3 vGSBitangent[ 3 ];
	#endif
#endif

#ifdef SHARED_SPB
	flat in int vGSSPBIndex[ 3 ];
	#define spbIndex vGSSPBIndex[0]
	
	#ifdef GS_RENDER_CUBE
		flat in int vGSSPBFlags[ 3 ];
		#define spbFlags vGSSPBFlags[0]
	#endif
	
	#include "v130/shared/defren/skin/shared_spb_redirect.glsl"
#endif



// Outputs
////////////

#ifdef REQUIRES_TEX_COLOR
	out vec2 vTCColor;
#endif
#ifdef CLIP_PLANE
	out vec3 vClipCoord;
#endif
#ifdef DEPTH_ORTHOGONAL
	out float vZCoord;
#endif
#ifdef DEPTH_DISTANCE
	out vec3 vPosition;
#endif
#ifdef HEIGHT_MAP
	out float vHTMask;
#endif
#ifdef REQUIRES_NORMAL
	out vec3 vNormal;
	#ifdef TEXTURE_NORMAL
		out vec3 vTangent;
		out vec3 vBitangent;
	#endif
#endif
#ifdef FADEOUT_RANGE
	out float vFadeZ;
#endif

#ifdef SHARED_SPB
	flat out int vSPBIndex;
#endif



// Cube Map Rendering
///////////////////////

#ifdef GS_RENDER_CUBE
// cube map handling. each triangle is send to one of the cube map faces using
// appropriate matrix transformation

#include "v130/shared/defren/skin/ubo_special_parameters.glsl"

void main( void ){
	int i;
	
	gl_PrimitiveID = gl_PrimitiveIDIn;
	
	#ifdef SHARED_SPB
	vSPBIndex = spbIndex;
	#endif
	
	#ifdef GS_RENDER_CUBE_INSTANCING
	gl_Layer = gl_InvocationID;
	#else
	for( int face=0; face<6; face++ ){
		gl_Layer = face;
	#endif
		
		// TODO per-face matrix. has to be applied to pMatrixVP, pMatrixV and pMatrixVn.
		//      how can this be made optimal?
		// 
		// pMatrixP
		//    this needs no modification
		// 
		// pMatrixV
		//    this transform vertices into camera spaces. the most simple soution is to
		//    use the first face matrix as reference matrix (positive x-axis). for each
		//    face can be defined a 3x3 rotation matrix converting from the positive
		//    x-axis matrix into the required face matrix. this matrices can be defined
		//    as a constant matrix array and should be fast. the result is this:
		//    
		//       vPosition = cRotFace[gl_InvocationID] 
		//          * ( pMatrixV * vec4( gl_in[ i ].gl_Position, 1.0 ) );
		//    
		//    works since pMatrixV is max4x3 returning a vec3 and cRotFaceis a mat3
		//    
		// pMatrixVn
		//    works similar to pMatrixV but needs transposed cRotFace to work like this:
		//    
		//       vNormal = normalize( ( vGSNormal[ i ] * pMatrixVn )
		//          * cRotFaceN[gl_InvocationID] );
		// 
		// pMatrixVP
		//    can not be done directly since the rotation matrix has to go in between the
		//    view and projection part. has to be done like this instead:
		//    
		//       gl_Position = pMatrixP
		//          * vec4( cRotFace[gl_InvocationID]
		//             * ( pMatrixV * vec4( gl_in[ i ].gl_Position ), 1.0 ), 1.0 );
		#ifdef GS_RENDER_CUBE_CULLING
			// WARNING! there is a nasty bug in the MESA implementation causing 'continue' to
			//          skip the loop increment if used in if-statements resulting in GPU infinite
			//          loop. sometimes continue works but especially here it results in the GPU
			//          dying horribly. the only working solution is to use the code in a way
			//          no 'continue' statement is required to be used
			
			#ifdef GS_RENDER_CUBE_INSTANCING
			if( ( pCubeFaceVisible & ( 1 << gl_Layer ) ) == 0 ){
				return;
			}
			#else
			if( ( pCubeFaceVisible & ( 1 << gl_Layer ) ) != 0 ){
			#endif
		#endif
		
		
		// emit triangle
		for( i=0; i<3; i++ ){
			#ifdef BILLBOARD
				gl_Position = pMatrixP * gl_in[ i ].gl_Position;
			#else
				gl_Position = pCubeMatrixVP[ gl_Layer ] * gl_in[ i ].gl_Position;
			#endif
			
			#ifdef REQUIRES_TEX_COLOR
				vTCColor = vGSTCColor[ i ];
			#endif
			
			#ifdef CLIP_PLANE
				#ifdef BILLBOARD
					vClipCoord = vGSClipCoord[ i ];
				#else
					vClipCoord = pCubeMatrixV[ gl_Layer ] * vec4( vGSClipCoord[ i ], 1.0 );
				#endif
			#endif
			
			#ifdef DEPTH_ORTHOGONAL
				#ifdef NO_ZCLIP
					vZCoord = gl_Position.z * 0.5 + 0.5; // we have to do the normalization ourself
					gl_Position.z = 0.0;
				#else
					vZCoord = gl_Position.z;
				#endif
			#endif
			
			#ifdef DEPTH_DISTANCE
				#ifdef BILLBOARD
					vPosition = gl_in[ i ].gl_Position;
				#else
					vPosition = pCubeMatrixV[ gl_Layer ] * gl_in[ i ].gl_Position;
				#endif
			#endif
			
			#ifdef HEIGHT_MAP
				vHTMask = vGSHTMask[ i ];
			#endif
			
			#ifdef REQUIRES_NORMAL
				vNormal = normalize( vGSNormal[ i ] * pCubeMatrixVn[ gl_Layer ] );
				#ifdef TEXTURE_NORMAL
					vTangent = normalize( vGSTangent[ i ] * pCubeMatrixVn[ gl_Layer ] );
					vBitangent = normalize( vGSBitangent[ i ] * pCubeMatrixVn[ gl_Layer ] );
				#endif
			#endif
			
			#ifdef FADEOUT_RANGE
				#ifdef BILLBOARD
					vFadeZ = gl_in[ i ].gl_Position.z;
				#else
					vFadeZ = ( pCubeMatrixV[ gl_Layer ] * gl_in[ i ].gl_Position ).z;
				#endif
			#endif
			
			EmitVertex();
		}
		
		EndPrimitive();
		
		
		#if defined GS_RENDER_CUBE_CULLING && ! defined GS_RENDER_CUBE_INSTANCING
		}
		#endif
		
	#ifndef GS_RENDER_CUBE_INSTANCING
	}
	#endif
}

#endif // GS_RENDER_CUBE
