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

#include "deoglArrayTexture.h"
#include "../deoglTextureStageManager.h"
#include "../pixelbuffer/deoglPixelBuffer.h"
#include "../texture2d/deoglTexture.h"
#include "../../capabilities/deoglCapabilities.h"
#include "../../capabilities/deoglCapsTextureFormat.h"
#include "../../memory/deoglMemoryManager.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTTexture.h"
#include "../../renderthread/deoglRTLogger.h"

#ifdef ANDROID
#include "../../framebuffer/deoglFramebuffer.h"
#include "../../framebuffer/deoglFramebufferManager.h"
#include "../../renderthread/deoglRTFramebuffer.h"
#include "../../renderthread/deoglRTLogger.h"
#endif

#include <dragengine/common/exceptions.h>



// Class deoglArrayTexture
////////////////////////////

// Constructor, destructor
////////////////////////////

deoglArrayTexture::deoglArrayTexture( deoglRenderThread &renderThread ) :
pRenderThread( renderThread )
{
	pTexture = 0;
	pFormat = renderThread.GetCapabilities().GetFormats().GetUseArrayTexFormatFor( deoglCapsFmtSupport::eutfRGB8 );
	
	pWidth = 1;
	pHeight = 1;
	pLayerCount = 1;
	pMipMapLevelCount = 0;
	pRealMipMapLevelCount = 0;
	pMipMapped = false;
	
	pMemoryUsageGPU = 0;
	pMemoryUsageCompressed = false;
	pMemoryUsageColor = true;
}

deoglArrayTexture::~deoglArrayTexture(){
	DestroyTexture();
}



// Management
///////////////

void deoglArrayTexture::SetSize( int width, int height, int layerCount ){
	if( width < 1 || height < 1 || layerCount < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( width != pWidth || height != pHeight || layerCount != pLayerCount ){
		DestroyTexture();
		
		pWidth = width;
		pHeight = height;
		pLayerCount = layerCount;
	}
}

void deoglArrayTexture::SetFormat( const deoglCapsTextureFormat *format ){
	if( ! format ){
		DETHROW( deeInvalidParam );
	}
	
	if( format != pFormat ){
		DestroyTexture();
		pFormat = format;
	}
}

void deoglArrayTexture::SetFormatMappingByNumber( deoglCapsFmtSupport::eUseTextureFormats formatNumber ){
	SetFormat( pRenderThread.GetCapabilities().GetFormats().GetUseArrayTexFormatFor( formatNumber ) );
}

void deoglArrayTexture::SetFormatFBOByNumber( deoglCapsFmtSupport::eUseTextureFormats formatNumber ){
	SetFormat( pRenderThread.GetCapabilities().GetFormats().GetUseFBOArrayTexFormatFor( formatNumber ) );
}

void deoglArrayTexture::SetMipMapped( bool mipmapped ){
	if( mipmapped != pMipMapped ){
		DestroyTexture();
		pMipMapped = mipmapped;
	}
}

void deoglArrayTexture::SetMipMapLevelCount( int count ){
	if( count < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( count != pMipMapLevelCount ){
		DestroyTexture();
		pMipMapLevelCount = count;
	}
}



void deoglArrayTexture::CreateTexture(){
	if( pTexture ){
		return;
	}
	
	deoglTextureStageManager &tsmgr = pRenderThread.GetTexture().GetStages();
	const GLenum glpixelformat = pFormat->GetPixelFormat();
	const GLenum glpixeltype = pFormat->GetPixelType();
	const GLenum glformat = pFormat->GetFormat();
	
	OGL_CHECK( pRenderThread, glGenTextures( 1, &pTexture ) );
	
	if( ! pTexture ){
		OGL_CHECK( pRenderThread, glGenTextures( 1, &pTexture ) );
		
		if( ! pTexture ){
			DETHROW( deeInvalidParam );
		}
	}
	
	tsmgr.EnableBareArrayTexture( 0, *this );
	
	OGL_CHECK( pRenderThread, pglTexImage3D( GL_TEXTURE_2D_ARRAY, 0, glformat, pWidth, pHeight, pLayerCount, 0, glpixelformat, glpixeltype, NULL ) );
	
	if( pMipMapped ){
		int count = pMipMapLevelCount;
		int height = pHeight;
		int width = pWidth;
		int i;
		
		if( count == 0 ){
			count = ( int )( floorf( log2f( ( height > width ) ? height : width ) ) );
		}
		
		for( i=0; i<count; i++ ){
			width >>= 1;
			height >>= 1;
			
			if( width < 1 ){
				width = 1;
			}
			if( height < 1 ){
				height = 1;
			}
			
			OGL_CHECK( pRenderThread, pglTexImage3D( GL_TEXTURE_2D_ARRAY, i + 1, glformat, width, height, pLayerCount, 0, glpixelformat, glpixeltype, NULL ) );
		}
		
		pRealMipMapLevelCount = count;
	}
	
	OGL_CHECK( pRenderThread, glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_LEVEL, pRealMipMapLevelCount ) );
	
	OGL_CHECK( pRenderThread, glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR ) );
	OGL_CHECK( pRenderThread, glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR ) );
	
	OGL_CHECK( pRenderThread, glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE ) );
	OGL_CHECK( pRenderThread, glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE ) );
	
	OGL_CHECK( pRenderThread, glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_COMPARE_MODE, GL_NONE ) );
	OGL_CHECK( pRenderThread, glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL ) );
	
	switch( glpixelformat ){
	case GL_RED:
	case GL_LUMINANCE:
	case GL_RED_INTEGER:
		OGL_CHECK( pRenderThread, glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_SWIZZLE_R, GL_RED ) );
		OGL_CHECK( pRenderThread, glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_SWIZZLE_G, GL_RED ) );
		OGL_CHECK( pRenderThread, glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_SWIZZLE_B, GL_RED ) );
		OGL_CHECK( pRenderThread, glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_SWIZZLE_A, GL_ONE ) );
		break;
		
	case GL_RG:
	case GL_LUMINANCE_ALPHA:
	case GL_RG_INTEGER:
		OGL_CHECK( pRenderThread, glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_SWIZZLE_R, GL_RED ) );
		OGL_CHECK( pRenderThread, glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_SWIZZLE_G, GL_RED ) );
		OGL_CHECK( pRenderThread, glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_SWIZZLE_B, GL_RED ) );
		OGL_CHECK( pRenderThread, glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_SWIZZLE_A, GL_GREEN ) );
		break;
		
	default:
		break;
	}
	
	
	tsmgr.DisableStage( 0 );
	
	UpdateMemoryUsage();
}

void deoglArrayTexture::DestroyTexture(){
	// to avoid problems with threading and such it would be a good idea to move this deletion call
	// out to a safe place. for this we need a texture deletion manager which hosts opengl textures
	// to be deleted the next time it is safe. a safe time would be before or after frame rendering
	// calls or somewhere during the clean up process. in this case the texture would be simply
	// registered with the deletion manager and set to NULL in here
	
	if( pTexture ){
		OGL_CHECK( pRenderThread, glDeleteTextures( 1, &pTexture ) );
		pTexture = 0;
		
		UpdateMemoryUsage();
	}
}

void deoglArrayTexture::SetPixels( const deoglPixelBuffer &pixels ){
	SetPixelsLevel( 0, pixels );
}

void deoglArrayTexture::SetPixelsLevel( int level, const deoglPixelBuffer &pixelBuffer ){
	int width, height;
	GetLevelSize( level, width, height );
	
	if( pixelBuffer.GetWidth() != width || pixelBuffer.GetHeight() != height || pixelBuffer.GetDepth() != pLayerCount ){
		//pRenderThread.GetLogger().LogErrorFormat( "SetPixelsLevel(%d) mismatch: Image(%d %d %d) PixelBuffer(%d %d %d)",
		//	level, width, height, pLayerCount, pixelBuffer.GetWidth(), pixelBuffer.GetHeight(), pixelBuffer.GetDepth() );
		DETHROW( deeInvalidParam );
	}
	
	deoglTextureStageManager &tsmgr = pRenderThread.GetTexture().GetStages();
	
	CreateTexture();
	tsmgr.EnableBareArrayTexture( 0, *this );
	
	OGL_CHECK( pRenderThread, glPixelStorei( GL_UNPACK_ALIGNMENT, 1 ) );
	
	if( pixelBuffer.GetCompressed() ){
		OGL_CHECK( pRenderThread, pglCompressedTexImage3D( GL_TEXTURE_2D_ARRAY, level, pFormat->GetFormat(),
			width, height, pLayerCount, 0, pixelBuffer.GetImageSize(), ( const GLvoid * )pixelBuffer.GetPointer() ) );
		//OGL_CHECK( pRenderThread, pglCompressedTexSubImage3D( GL_TEXTURE_2D_ARRAY, level, 0, 0, width, height,
		//	pFormat->GetFormat(), pixels.GetImageSize(), ( const GLvoid * )pixels.GetPointer() ) );
		
	}else{
		OGL_CHECK( pRenderThread, pglTexSubImage3D( GL_TEXTURE_2D_ARRAY, level, 0, 0, 0, width, height, pLayerCount,
			pixelBuffer.GetGLPixelFormat(), pixelBuffer.GetGLPixelType(), ( const GLvoid * )pixelBuffer.GetPointer() ) );
	}
	
	OGL_CHECK( pRenderThread, glPixelStorei( GL_UNPACK_ALIGNMENT, 4 ) );
	
	tsmgr.DisableStage( 0 );
}

void deoglArrayTexture::GetPixels( deoglPixelBuffer &pixelBuffer ) const{
	GetPixelsLevel( 0, pixelBuffer );
}

void deoglArrayTexture::GetPixelsLevel( int level, deoglPixelBuffer &pixelBuffer ) const{
	int width, height;
	GetLevelSize( level, width, height );
	
	if( pixelBuffer.GetWidth() != width || pixelBuffer.GetHeight() != height || pixelBuffer.GetDepth() != pLayerCount ){
		DETHROW( deeInvalidParam );
	}
	if( ! pTexture ){
		return;
	}
	
	#ifdef ANDROID
	// glReadPixels under OpenGL ES does only support GL_RGBA and GL_RGBA_INTEGRAL.
	// if something else is required (for exampel GL_RGB, GL_RG or GL_RED) we have
	// to use a temporary texture containing 4 components and copy over from there.
	// inefficient but the only solution possible
	switch( pixelBuffer.GetFormat() ){
	case deoglPixelBuffer::epfByte1:
	case deoglPixelBuffer::epfByte2:
	case deoglPixelBuffer::epfByte3:{
		deoglPixelBuffer tempPixBuf( deoglPixelBuffer::epfByte4, width, height, pLayerCount );
		const int count = width * height;
		int i, j;
		
		GetPixelsLevel( level, tempPixBuf ); // get data using temporary 4-channels buffer
		
		switch( pixelBuffer.GetFormat() ){
		case deoglPixelBuffer::epfByte1:{
			for( j=0; j<pLayerCount; j++ ){
				deoglPixelBuffer::sByte1 *dataDest = pixelBuffer.GetPointerByte1() + count;
				const deoglPixelBuffer::sByte4 *dataSrc = tempPixBuf.GetPointerByte4() + count;
				for( i=0; i<count; i++ ){
					dataDest[ i ].r = dataSrc[ i ].r;
				}
			}
			}return;
			
		case deoglPixelBuffer::epfByte2:{
			for( j=0; j<pLayerCount; j++ ){
				deoglPixelBuffer::sByte2 *dataDest = pixelBuffer.GetPointerByte2() + count;
				const deoglPixelBuffer::sByte4 *dataSrc = tempPixBuf.GetPointerByte4() + count;
				for( i=0; i<count; i++ ){
					dataDest[ i ].r = dataSrc[ i ].r;
					dataDest[ i ].g = dataSrc[ i ].g;
				}
			}
			}return;
			
		case deoglPixelBuffer::epfByte3:{
			for( j=0; j<pLayerCount; j++ ){
				deoglPixelBuffer::sByte3 *dataDest = pixelBuffer.GetPointerByte3() + count;
				const deoglPixelBuffer::sByte4 *dataSrc = tempPixBuf.GetPointerByte4() + count;
				for( i=0; i<count; i++ ){
					dataDest[ i ].r = dataSrc[ i ].r;
					dataDest[ i ].g = dataSrc[ i ].g;
					dataDest[ i ].b = dataSrc[ i ].b;
				}
			}
			}return;
			
		default:
			return;
		}
		}return;
		
	case deoglPixelBuffer::epfFloat1:
	case deoglPixelBuffer::epfFloat2:
	case deoglPixelBuffer::epfFloat3:{
		deoglPixelBuffer tempPixBuf( deoglPixelBuffer::epfFloat4, width, height, pLayerCount );
		const int count = width * height;
		int i, j;
		
		GetPixelsLevel( level, tempPixBuf ); // get data using temporary 4-channels buffer
		
		switch( pixelBuffer.GetFormat() ){
		case deoglPixelBuffer::epfFloat1:{
			for( j=0; j<pLayerCount; j++ ){
				deoglPixelBuffer::sFloat1 *dataDest = pixelBuffer.GetPointerFloat1() + count;
				const deoglPixelBuffer::sFloat4 *dataSrc = tempPixBuf.GetPointerFloat4() + count;
				for( i=0; i<count; i++ ){
					dataDest[ i ].r = dataSrc[ i ].r;
				}
			}
			}return;
			
		case deoglPixelBuffer::epfFloat2:{
			for( j=0; j<pLayerCount; j++ ){
				deoglPixelBuffer::sFloat2 *dataDest = pixelBuffer.GetPointerFloat2() + count;
				const deoglPixelBuffer::sFloat4 *dataSrc = tempPixBuf.GetPointerFloat4() + count;
				for( i=0; i<count; i++ ){
					dataDest[ i ].r = dataSrc[ i ].r;
					dataDest[ i ].g = dataSrc[ i ].g;
				}
			}
			}return;
			
		case deoglPixelBuffer::epfFloat3:{
			for( j=0; j<pLayerCount; j++ ){
				deoglPixelBuffer::sFloat3 *dataDest = pixelBuffer.GetPointerFloat3() + count;
				const deoglPixelBuffer::sFloat4 *dataSrc = tempPixBuf.GetPointerFloat4() + count;
				for( i=0; i<count; i++ ){
					dataDest[ i ].r = dataSrc[ i ].r;
					dataDest[ i ].g = dataSrc[ i ].g;
					dataDest[ i ].b = dataSrc[ i ].b;
				}
			}
			}return;
			
		default:
			return;
		}
		}return;
		
	default:
		break;
	}
	
	// this is called if the format is 4 channels
	const GLubyte * const pixelsPtr = ( const GLubyte * )pixelBuffer.GetPointer();
	const int faceStride = pixelBuffer.GetLayerStride();
	deoglRTFramebuffer &rtframebuffer = pRenderThread.GetFramebuffer();
	deoglFramebuffer * const oldFbo = rtframebuffer.GetActive();
	deoglFramebuffer *fbo = NULL;
	int i;
	
	try{
		fbo = rtframebuffer.GetManager().GetFBOWithResolution( width, height );
		//fbo = new deoglFramebuffer( pRenderThread, false );
		rtframebuffer.Activate( fbo );
		fbo->DetachAllImages();
		
		const GLenum buffers[ 1 ] = { GL_NONE };
		OGL_CHECK( pRenderThread, pglDrawBuffers( 1, buffers ) );
		if( pFormat->GetIsDepth() ){
			OGL_CHECK( pRenderThread, glReadBuffer( GL_NONE ) );
		}else{
			OGL_CHECK( pRenderThread, glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
		}
		
		OGL_CHECK( pRenderThread, glPixelStorei( GL_PACK_ALIGNMENT, 1 ) );
		
		for( i=0; i<pLayerCount; i++ ){
			if( pFormat->GetIsDepth() ){
				fbo->AttachDepthArrayTextureLayerLevel( ( deoglArrayTexture* )this, i, level );
			}else{
				fbo->AttachColorArrayTextureLayerLevel( 0, ( deoglArrayTexture* )this, i, level );
			}
			
			OGL_CHECK( pRenderThread, glReadPixels( 0, 0, width, height, pixelBuffer.GetGLPixelFormat(),
				pixelBuffer.GetGLPixelType(), ( GLvoid* )( pixelsPtr + faceStride * i ) ) );
		}
		
		OGL_CHECK( pRenderThread, glPixelStorei( GL_PACK_ALIGNMENT, 4 ) );
		
		fbo->DetachColorImage( 0 );
		fbo->DetachDepthImage();
		
		rtframebuffer.Activate( oldFbo );
		
		fbo->DecreaseUsageCount();
		//delete fbo;
		fbo = NULL;
		
	}catch( const deException & ){
		rtframebuffer.Activate( oldFbo );
		if( fbo ){
			fbo->DecreaseUsageCount();
			//delete fbo;
		}
		throw;
	}
	
	#else
	deoglTextureStageManager &tsmgr = pRenderThread.GetTexture().GetStages();
	
	tsmgr.EnableBareArrayTexture( 0, *this );
	OGL_CHECK( pRenderThread, glPixelStorei( GL_PACK_ALIGNMENT, 1 ) );
	
	OGL_CHECK( pRenderThread, glGetTexImage( GL_TEXTURE_2D_ARRAY, level, pixelBuffer.GetGLPixelFormat(),
		pixelBuffer.GetGLPixelType(), ( GLvoid* )pixelBuffer.GetPointer() ) );
	
	OGL_CHECK( pRenderThread, glPixelStorei( GL_PACK_ALIGNMENT, 4 ) );
	tsmgr.DisableStage( 0 );
	#endif
}



void deoglArrayTexture::GetLevelSize( int level, int &width, int &height ) const{
	if( level < 0 || level > pRealMipMapLevelCount ){
		DETHROW( deeInvalidParam );
	}
	
	int i;
	
	width = pWidth;
	height = pHeight;
	
	for( i=0; i<level; i++ ){
		width >>= 1;
		if( width < 1 ){
			width = 1;
		}
		
		height >>= 1;
		if( height < 1 ){
			height = 1;
		}
	}
}

void deoglArrayTexture::CreateMipMaps(){
	if( pMipMapped && pMipMapLevelCount == 0 ){
		deoglTextureStageManager &tsmgr = pRenderThread.GetTexture().GetStages();
		
		tsmgr.EnableBareArrayTexture( 0, *this );
		pglGenerateMipmap( GL_TEXTURE_2D_ARRAY );
		tsmgr.DisableStage( 0 );
	}
}



void deoglArrayTexture::CopyFrom( const deoglArrayTexture &texture, bool withMipMaps, int srcLayer, int destLayer ){
	CopyFrom( texture, withMipMaps, srcLayer, destLayer, pWidth, pHeight, 1, 0, 0, 0, 0 );
}

void deoglArrayTexture::CopyFrom( const deoglArrayTexture &texture, bool withMipMaps, int srcLayer, int destLayer,
int width, int height, int layerCount, int srcX, int srcY, int destX, int destY ){
	if( destX < 0 || destY < 0 || srcX < 0 || srcY < 0 || srcLayer < 0 || destLayer < 0
	|| destX + width > pWidth || destY + height > pHeight || destLayer + layerCount > pLayerCount ){
		DETHROW( deeInvalidParam );
	}
	
	const int srcHeight = texture.GetHeight();
	const int srcWidth = texture.GetWidth();
	const int srcLayerCount = texture.GetLayerCount();
	
	if( srcX + width > srcWidth || srcY + height > srcHeight || srcLayer + layerCount > srcLayerCount ){
		DETHROW( deeInvalidParam );
	}
	
	CreateTexture();
	
	if( withMipMaps && texture.GetMipMapped() && pMipMapped ){
		int destMipMapLevelCount = texture.GetMipMapLevelCount();
		int srcMipMapLevelCount = pMipMapLevelCount;
		int i, mipMapLevelCount;
		
		if( destMipMapLevelCount == 0 ){
			destMipMapLevelCount = ( int )( ceilf( log2f( ( pHeight > pWidth ) ? pHeight : pWidth ) ) ) + 1;
		}
		if( srcMipMapLevelCount == 0 ){
			srcMipMapLevelCount = ( int )( ceilf( log2f( ( srcHeight > srcWidth ) ? srcHeight : srcWidth ) ) ) + 1;
		}
		mipMapLevelCount = ( ( srcMipMapLevelCount < destMipMapLevelCount ) ? srcMipMapLevelCount : destMipMapLevelCount );
		
		for( i=0; i<mipMapLevelCount; i++ ){
			if( pglCopyImageSubData ){
				pglCopyImageSubData( texture.GetTexture(), GL_TEXTURE_2D_ARRAY, i, srcX, srcY, srcLayer,
					pTexture, GL_TEXTURE_2D_ARRAY, i, destX, destY, destLayer, width, height, layerCount );
				
			}else if( pglCopyImageSubDataNV ){
				pglCopyImageSubDataNV( texture.GetTexture(), GL_TEXTURE_2D_ARRAY, i, srcX, srcY, srcLayer,
					pTexture, GL_TEXTURE_2D_ARRAY, i, destX, destY, destLayer, width, height, layerCount );
				
			}else{
				DETHROW( deeInvalidParam );
			}
			
			srcX >>= 1;
			srcY >>= 1;
			destX >>= 1;
			destY >>= 1;
			width >>= 1;
			height >>= 1;
			if( width < 1 ){
				width = 1;
			}
			if( height < 1 ){
				height = 1;
			}
		}
		
	}else{
		if( pglCopyImageSubData ){
			pglCopyImageSubData( texture.GetTexture(), GL_TEXTURE_2D_ARRAY, 0, srcX, srcY, srcLayer,
				pTexture, GL_TEXTURE_2D_ARRAY, 0, destX, destY, destLayer, width, height, layerCount );
			
		}else if( pglCopyImageSubDataNV ){
			pglCopyImageSubDataNV( texture.GetTexture(), GL_TEXTURE_2D_ARRAY, 0, srcX, srcY, srcLayer,
				pTexture, GL_TEXTURE_2D_ARRAY, 0, destX, destY, destLayer, width, height, layerCount );
			
		}else{
			DETHROW( deeInvalidParam );
		}
	}
}

void deoglArrayTexture::CopyFrom( const deoglTexture &texture, bool withMipMaps, int destLayer ){
	CopyFrom( texture, withMipMaps, destLayer, pWidth, pHeight, 0, 0, 0, 0 );
}

void deoglArrayTexture::CopyFrom( const deoglTexture &texture, bool withMipMaps, int destLayer,
int width, int height, int srcX, int srcY, int destX, int destY ){
	//printf( "deoglArrayTexture.CopyFrom: tex=%u destLayer=%i width=%i height=%i srcX=%i srcY=%i destX=%i destY=%i\n", texture.GetTexture(), destLayer, width, height, srcX, srcY, destX, destY );
	//printf( "TEX: width=%i height=%i\n", texture.GetWidth(), texture.GetHeight() );
	if( destX < 0 || destY < 0 || srcX < 0 || srcY < 0 || destLayer < 0
	|| destX + width > pWidth || destY + height > pHeight || destLayer >= pLayerCount ){
		DETHROW( deeInvalidParam );
	}
	
	const int srcHeight = texture.GetHeight();
	const int srcWidth = texture.GetWidth();
	
	if( srcX + width > srcWidth || srcY + height > srcHeight ){
		DETHROW( deeInvalidParam );
	}
	
	CreateTexture();
	
	if( withMipMaps && texture.GetMipMapped() && pMipMapped ){
		int destMipMapLevelCount = texture.GetMipMapLevelCount();
		int srcMipMapLevelCount = pMipMapLevelCount;
		int i, mipMapLevelCount;
		
		if( destMipMapLevelCount == 0 ){
			destMipMapLevelCount = ( int )( ceilf( log2f( ( pHeight > pWidth ) ? pHeight : pWidth ) ) ) + 1;
		}
		if( srcMipMapLevelCount == 0 ){
			srcMipMapLevelCount = ( int )( ceilf( log2f( ( srcHeight > srcWidth ) ? srcHeight : srcWidth ) ) ) + 1;
		}
		mipMapLevelCount = ( ( srcMipMapLevelCount < destMipMapLevelCount ) ? srcMipMapLevelCount : destMipMapLevelCount );
		
		for( i=0; i<mipMapLevelCount; i++ ){
			//printf( "copy image level %i\n", i );
			if( pglCopyImageSubData ){
				pglCopyImageSubData( texture.GetTexture(), GL_TEXTURE_2D, i, srcX, srcY, 0,
					pTexture, GL_TEXTURE_2D_ARRAY, i, destX, destY, destLayer, width, height, 1 );
				
			}else if( pglCopyImageSubDataNV ){
				pglCopyImageSubDataNV( texture.GetTexture(), GL_TEXTURE_2D, i, srcX, srcY, 0,
					pTexture, GL_TEXTURE_2D_ARRAY, i, destX, destY, destLayer, width, height, 1 );
				
			}else{
				DETHROW( deeInvalidParam );
			}
			
			srcX >>= 1;
			srcY >>= 1;
			destX >>= 1;
			destY >>= 1;
			width >>= 1;
			height >>= 1;
			if( width < 1 ){
				width = 1;
			}
			if( height < 1 ){
				height = 1;
			}
		}
		
	}else{
		if( pglCopyImageSubData ){
			pglCopyImageSubData( texture.GetTexture(), GL_TEXTURE_2D, 0, srcX, srcY, 0,
				pTexture, GL_TEXTURE_2D_ARRAY, 0, destX, destY, destLayer, width, height, 1 );
			
		}else if( pglCopyImageSubDataNV ){
			pglCopyImageSubDataNV( texture.GetTexture(), GL_TEXTURE_2D, 0, srcX, srcY, 0,
				pTexture, GL_TEXTURE_2D_ARRAY, 0, destX, destY, destLayer, width, height, 1 );
			
		}else{
			DETHROW( deeInvalidParam );
		}
	}
}



void deoglArrayTexture::UpdateMemoryUsage(){
	deoglMemoryConsumptionTexture &consumption = pRenderThread.GetMemoryManager().GetConsumption().GetTextureArray();
	
	if( pMemoryUsageGPU > 0 ){
		consumption.DecrementCount();
		consumption.DecrementGPU( pMemoryUsageGPU );
		
		if( pMemoryUsageColor ){
			consumption.DecrementColorCount();
			consumption.DecrementColorGPU( pMemoryUsageGPU );
			
			if( pMemoryUsageCompressed ){
				consumption.DecrementGPUCompressed( pMemoryUsageGPU );
				consumption.DecrementColorGPUCompressed( pMemoryUsageGPU );
				
			}else{
				consumption.DecrementGPUUncompressed( pMemoryUsageGPU );
				consumption.DecrementColorGPUUncompressed( pMemoryUsageGPU );
			}
			
		}else{
			consumption.DecrementDepthCount();
			consumption.DecrementDepthGPU( pMemoryUsageGPU );
			
			if( pMemoryUsageCompressed ){
				consumption.DecrementGPUCompressed( pMemoryUsageGPU );
				consumption.DecrementDepthGPUCompressed( pMemoryUsageGPU );
				
			}else{
				consumption.DecrementGPUUncompressed( pMemoryUsageGPU );
				consumption.DecrementDepthGPUUncompressed( pMemoryUsageGPU );
			}
		}
		
		pMemoryUsageGPU = 0;
		pMemoryUsageCompressed = false;
		pMemoryUsageColor = true;
	}
	
	if( pTexture ){
		const int bitsPerPixel = pFormat->GetBitsPerPixel();
		int baseSize;
		
		baseSize = pWidth * pHeight * pLayerCount;
		
		baseSize *= bitsPerPixel >> 3;
		if( ( bitsPerPixel & 0x7 ) > 0 ){
			baseSize >>= 1;
		}
		
		pMemoryUsageColor = ! pFormat->GetIsDepth();
		
		#ifdef ANDROID
		pMemoryUsageGPU = baseSize;
		
		#else
		if( pFormat->GetIsCompressed() ){
			deoglTextureStageManager &tsmgr = pRenderThread.GetTexture().GetStages();
			GLint compressedSize, isCompressed;
			
			tsmgr.EnableBareArrayTexture( 0, *this );
			
			isCompressed = 0;
			OGL_CHECK( pRenderThread, glGetTexLevelParameteriv( GL_TEXTURE_2D_ARRAY, 0, GL_TEXTURE_COMPRESSED, &isCompressed ) );
			
			if( isCompressed ){
				compressedSize = 0;
				OGL_CHECK( pRenderThread, glGetTexLevelParameteriv( GL_TEXTURE_2D_ARRAY, 0, GL_TEXTURE_COMPRESSED_IMAGE_SIZE, &compressedSize ) );
				pMemoryUsageGPU = compressedSize;
				pMemoryUsageCompressed = true;
				
			}else{
				pMemoryUsageGPU = baseSize;
			}
			
			tsmgr.DisableStage( 0 );
			
		}else{
			pMemoryUsageGPU = baseSize;
		}
		#endif
	}
	
	if( pMemoryUsageGPU > 0 ){
		consumption.IncrementCount();
		consumption.IncrementGPU( pMemoryUsageGPU );
		
		if( pMemoryUsageColor ){
			consumption.IncrementColorCount();
			consumption.IncrementColorGPU( pMemoryUsageGPU );
			
			if( pMemoryUsageCompressed ){
				consumption.IncrementGPUCompressed( pMemoryUsageGPU );
				consumption.IncrementColorGPUCompressed( pMemoryUsageGPU );
				
			}else{
				consumption.IncrementGPUUncompressed( pMemoryUsageGPU );
				consumption.IncrementColorGPUUncompressed( pMemoryUsageGPU );
			}
			
		}else{
			consumption.IncrementDepthCount();
			consumption.IncrementDepthGPU( pMemoryUsageGPU );
			
			if( pMemoryUsageCompressed ){
				consumption.IncrementGPUCompressed( pMemoryUsageGPU );
				consumption.IncrementDepthGPUCompressed( pMemoryUsageGPU );
				
			}else{
				consumption.IncrementGPUUncompressed( pMemoryUsageGPU );
				consumption.IncrementDepthGPUUncompressed( pMemoryUsageGPU );
			}
		}
	}
}



// Helper Functions
/////////////////////

void deoglArrayTexture::SetMapingFormat( int channels, bool useFloat, bool compressed ){
	if( channels == 1 ){
		if( useFloat ){
			SetFormatMappingByNumber( deoglCapsFmtSupport::eutfR16F );
			
		}else{
			if( compressed ){
				SetFormatMappingByNumber( deoglCapsFmtSupport::eutfR8_C );
				
			}else{
				SetFormatMappingByNumber( deoglCapsFmtSupport::eutfR8 );
			}
		}
		
	}else if( channels == 2 ){
		if( useFloat ){
			SetFormatMappingByNumber( deoglCapsFmtSupport::eutfRG16F );
			
		}else{
			if( compressed ){
				SetFormatMappingByNumber( deoglCapsFmtSupport::eutfRG8_C );
				
			}else{
				SetFormatMappingByNumber( deoglCapsFmtSupport::eutfRG8 );
			}
		}
		
	}else if( channels == 3 ){
		if( useFloat ){
			SetFormatMappingByNumber( deoglCapsFmtSupport::eutfRGB16F );
			
		}else{
			if( compressed ){
				SetFormatMappingByNumber( deoglCapsFmtSupport::eutfRGB8_C );
				
			}else{
				SetFormatMappingByNumber( deoglCapsFmtSupport::eutfRGB8 );
			}
		}
		
	}else if( channels == 4 ){
		if( useFloat ){
			SetFormatMappingByNumber( deoglCapsFmtSupport::eutfRGBA16F );
			
		}else{
			if( compressed ){
				SetFormatMappingByNumber( deoglCapsFmtSupport::eutfRGBA8_C );
				
			}else{
				SetFormatMappingByNumber( deoglCapsFmtSupport::eutfRGBA8 );
			}
		}
		
	}else{
		DETHROW( deeInvalidParam );
	}
}

void deoglArrayTexture::SetFBOFormat( int channels, bool useFloat ){
	if( channels == 1 ){
		if( useFloat ){
			SetFormatFBOByNumber( deoglCapsFmtSupport::eutfR16F );
			
		}else{
			SetFormatFBOByNumber( deoglCapsFmtSupport::eutfR8 );
		}
		
	}else if( channels == 2 ){
		if( useFloat ){
			SetFormatFBOByNumber( deoglCapsFmtSupport::eutfRG16F );
			
		}else{
			SetFormatFBOByNumber( deoglCapsFmtSupport::eutfRG8 );
		}
		
	}else if( channels == 3 ){
		if( useFloat ){
			SetFormatFBOByNumber( deoglCapsFmtSupport::eutfRGB16F );
			
		}else{
			SetFormatFBOByNumber( deoglCapsFmtSupport::eutfRGB8 );
		}
		
	}else if( channels == 4 ){
		if( useFloat ){
			SetFormatFBOByNumber( deoglCapsFmtSupport::eutfRGBA16F );
			
		}else{
			SetFormatFBOByNumber( deoglCapsFmtSupport::eutfRGBA8 );
		}
		
	}else{
		DETHROW( deeInvalidParam );
	}
}

void deoglArrayTexture::SetDepthFormat( bool packedStencil ){
	if( packedStencil ){
		SetFormatFBOByNumber( deoglCapsFmtSupport::eutfDepth_Stencil );
		
	}else{
		SetFormatFBOByNumber( deoglCapsFmtSupport::eutfDepth );
	}
}
