/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include <GLES3/gl3.h>
#include <GLES3/gl2ext.h>
#include "OpenglES3ConstantCatalog.h"

int OpenglES3ConstantCatalog::getTextureAnisotropyFlag(){
	return GL_TEXTURE_MAX_ANISOTROPY_EXT;
}
int OpenglES3ConstantCatalog::getTextureMagFilterFlag(){
	return GL_TEXTURE_MAG_FILTER;
}
int OpenglES3ConstantCatalog::getTextureMinFilterFlag(){
	return GL_TEXTURE_MIN_FILTER;
}
int OpenglES3ConstantCatalog::getTextureLinearFilteringFlag(){
	return GL_LINEAR;
}
int OpenglES3ConstantCatalog::getTextureMipmapLinearFilteringFlag(){
	return GL_LINEAR_MIPMAP_LINEAR;
}
int OpenglES3ConstantCatalog::getStaticDrawFlag(){
	return GL_STATIC_DRAW;
}
int OpenglES3ConstantCatalog::getFloatTypeFlag(){
	return GL_FLOAT;
}
int OpenglES3ConstantCatalog::getUnsignedIntTypeFlag(){
	return GL_UNSIGNED_INT;
}
int OpenglES3ConstantCatalog::getUnsignedShortTypeFlag(){
    return GL_UNSIGNED_SHORT;
}
int OpenglES3ConstantCatalog::getTextureWrapSFlag(){
	return GL_TEXTURE_WRAP_S;
}
int OpenglES3ConstantCatalog::getTextureWrapTFlag(){
	return GL_TEXTURE_WRAP_T;
}
int OpenglES3ConstantCatalog::getTextureClampToEdgeFlag(){
	return GL_CLAMP_TO_EDGE;
}
int OpenglES3ConstantCatalog::getTextureClampToBorderFlag(){
	// OpenGL ES no permite definir color de bordes de imagen, por lo que hago que se tomen los bordes propios de la imagen
	return GL_CLAMP_TO_EDGE;
}
int OpenglES3ConstantCatalog::getTextureBorderColorFlag(){
	// Opengl es no permite definir color de bordes de las imagenes, por lo que este parametro no tiene sentido
	// ESTO NO SE SI ESTA BIEN (definir el parametro como cero, probar)
	return 0;
}
int OpenglES3ConstantCatalog::getTextureNearestFilterFlag(){
	return GL_NEAREST;
}
int OpenglES3ConstantCatalog::getTextureRepeatFlag(){
	return GL_REPEAT;
}
int OpenglES3ConstantCatalog::getFormatRGB(){
	return GL_RGB;
}
int OpenglES3ConstantCatalog::getFormatRGB32F(){
	return GL_RGB16F; //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!cambiar!!!!!!!!!!
}
int OpenglES3ConstantCatalog::getFormatRGB8(){
	return GL_RGB8;
}
int OpenglES3ConstantCatalog::getFormatRG(){
	return GL_RG;
}
int OpenglES3ConstantCatalog::getFormatRG8(){
	return GL_RG8;
}
int OpenglES3ConstantCatalog::getFormatR(){
	// REVIEW: Cuidado que aca es GL_R, pero opengl es no lo soporta. No es la mejor solucion (no se ni si es valida esta). 
	return GL_R8;
}
int OpenglES3ConstantCatalog::getFormatR8(){
	return GL_R8;
}
int OpenglES3ConstantCatalog::getFormatR32F(){
	return GL_R32F;
}
int OpenglES3ConstantCatalog::getFormatRG32F(){
	return GL_RG32F;
}
int OpenglES3ConstantCatalog::getFormatRGBA(){
	return GL_RGBA;
}
int OpenglES3ConstantCatalog::getFormatRGBA8(){
	return GL_RGBA8;
}
int OpenglES3ConstantCatalog::getFormatDepth24(){
	return GL_DEPTH_COMPONENT24;
}
int OpenglES3ConstantCatalog::getFormatDepth(){
	return GL_DEPTH_COMPONENT;
}
int OpenglES3ConstantCatalog::getDepthTestFlag(){
	return GL_DEPTH_TEST;
}
int OpenglES3ConstantCatalog::getBlendFlag(){
	return GL_BLEND;
}
int OpenglES3ConstantCatalog::getBlendSrcAlphaFlag(){
	return GL_SRC_ALPHA;
}
int OpenglES3ConstantCatalog::getBlendOneMinusSrcAlphaFlag(){
	return GL_ONE_MINUS_SRC_ALPHA;
}
int OpenglES3ConstantCatalog::getCullFaceFlag(){
	return GL_CULL_FACE;
}
int OpenglES3ConstantCatalog::getCwFrontFaceFlag(){
	return GL_CW;
}
unsigned int OpenglES3ConstantCatalog::getTimeElapsedQueryType(){
	// Opengl ES no soporta Time Queries. Vamos a ver si no rompe nada con cero (salvo un error de opengl).
	return 0;
}
