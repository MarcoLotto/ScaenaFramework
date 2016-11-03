/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include <GL/glew.h>
#include "OpenglConstantCatalog.h"

int OpenglConstantCatalog::getTextureAnisotropyFlag(){
	return GL_TEXTURE_MAX_ANISOTROPY_EXT;
}
int OpenglConstantCatalog::getTextureMagFilterFlag(){
	return GL_TEXTURE_MAG_FILTER;
}
int OpenglConstantCatalog::getTextureMinFilterFlag(){
	return GL_TEXTURE_MIN_FILTER;
}
int OpenglConstantCatalog::getTextureLinearFilteringFlag(){
	return GL_LINEAR;
}
int OpenglConstantCatalog::getTextureMipmapLinearFilteringFlag(){
	return GL_LINEAR_MIPMAP_LINEAR;
}
int OpenglConstantCatalog::getStaticDrawFlag(){
	return GL_STATIC_DRAW;
}
int OpenglConstantCatalog::getFloatTypeFlag(){
	return GL_FLOAT;
}
int OpenglConstantCatalog::getUnsignedIntTypeFlag(){
	return GL_UNSIGNED_INT;
}
int OpenglConstantCatalog::getUnsignedShortTypeFlag(){
    return GL_UNSIGNED_SHORT;
}
int OpenglConstantCatalog::getTextureWrapSFlag(){
	return GL_TEXTURE_WRAP_S;
}
int OpenglConstantCatalog::getTextureWrapTFlag(){
	return GL_TEXTURE_WRAP_T;
}
int OpenglConstantCatalog::getTextureClampToEdgeFlag(){
	return GL_CLAMP_TO_EDGE;
}
int OpenglConstantCatalog::getTextureClampToBorderFlag(){
	return GL_CLAMP_TO_BORDER;
}
int OpenglConstantCatalog::getTextureBorderColorFlag(){
	return GL_TEXTURE_BORDER_COLOR;
}
int OpenglConstantCatalog::getTextureNearestFilterFlag(){
	return GL_NEAREST;
}
int OpenglConstantCatalog::getTextureRepeatFlag(){
	return GL_REPEAT;
}
int OpenglConstantCatalog::getFormatRGB(){
	return GL_RGB;
}
int OpenglConstantCatalog::getFormatRGB32F(){
	return GL_RGB32F;
}
int OpenglConstantCatalog::getFormatRGB8(){
	return GL_RGB8;
}
int OpenglConstantCatalog::getFormatRG(){
	return GL_RG;
}
int OpenglConstantCatalog::getFormatRG8(){
	return GL_RG8;
}
int OpenglConstantCatalog::getFormatR(){
	return GL_R;
}
int OpenglConstantCatalog::getFormatR8(){
	return GL_R8;
}
int OpenglConstantCatalog::getFormatR32F(){
	return GL_R32F;
}
int OpenglConstantCatalog::getFormatRG32F(){
	return GL_RG32F;
}
int OpenglConstantCatalog::getFormatRGBA(){
	return GL_RGBA;
}
int OpenglConstantCatalog::getFormatRGBA8(){
	return GL_RGBA8;
}
int OpenglConstantCatalog::getFormatDepth24(){
	return GL_DEPTH_COMPONENT24;
}
int OpenglConstantCatalog::getFormatDepth(){
	return GL_DEPTH_COMPONENT;
}
int OpenglConstantCatalog::getDepthTestFlag(){
	return GL_DEPTH_TEST;
}
int OpenglConstantCatalog::getBlendFlag(){
	return GL_BLEND;
}
int OpenglConstantCatalog::getBlendSrcAlphaFlag(){
	return GL_SRC_ALPHA;
}
int OpenglConstantCatalog::getBlendOneMinusSrcAlphaFlag(){
	return GL_ONE_MINUS_SRC_ALPHA;
}
int OpenglConstantCatalog::getCullFaceFlag(){
	return GL_CULL_FACE;
}
int OpenglConstantCatalog::getCwFrontFaceFlag(){
	return GL_CW;
}
unsigned int OpenglConstantCatalog::getTimeElapsedQueryType(){
	return GL_TIME_ELAPSED;
}
