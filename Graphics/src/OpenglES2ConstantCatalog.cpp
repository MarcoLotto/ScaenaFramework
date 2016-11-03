/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "OpenglES2ConstantCatalog.h"

int OpenglES2ConstantCatalog::getTextureAnisotropyFlag(){
	return GL_TEXTURE_MAX_ANISOTROPY_EXT;
}
int OpenglES2ConstantCatalog::getTextureMagFilterFlag(){
	return GL_TEXTURE_MAG_FILTER;
}
int OpenglES2ConstantCatalog::getTextureMinFilterFlag(){
	return GL_TEXTURE_MIN_FILTER;
}
int OpenglES2ConstantCatalog::getTextureLinearFilteringFlag(){
	return GL_LINEAR;
}
int OpenglES2ConstantCatalog::getTextureMipmapLinearFilteringFlag(){
	return GL_LINEAR_MIPMAP_LINEAR;
}
int OpenglES2ConstantCatalog::getStaticDrawFlag(){
	return GL_STATIC_DRAW;
}
int OpenglES2ConstantCatalog::getFloatTypeFlag(){
	return GL_FLOAT;
}
int OpenglES2ConstantCatalog::getUnsignedIntTypeFlag(){
	return GL_UNSIGNED_INT;
}
int OpenglES2ConstantCatalog::getUnsignedShortTypeFlag(){
    return GL_UNSIGNED_SHORT;
}
int OpenglES2ConstantCatalog::getTextureWrapSFlag(){
	return GL_TEXTURE_WRAP_S;
}
int OpenglES2ConstantCatalog::getTextureWrapTFlag(){
	return GL_TEXTURE_WRAP_T;
}
int OpenglES2ConstantCatalog::getTextureClampToEdgeFlag(){
	return GL_CLAMP_TO_EDGE;
}
int OpenglES2ConstantCatalog::getTextureClampToBorderFlag(){
	// OpenGL ES no permite definir color de bordes de imagen, por lo que hago que se tomen los bordes propios de la imagen
	return GL_CLAMP_TO_EDGE;
}
int OpenglES2ConstantCatalog::getTextureBorderColorFlag(){
	// Opengl es no permite definir color de bordes de las imagenes, por lo que este parametro no tiene sentido
	// ESTO NO SE SI ESTA BIEN (definir el parametro como cero, probar)
	return 0;
}
int OpenglES2ConstantCatalog::getTextureNearestFilterFlag(){
	return GL_NEAREST;
}
int OpenglES2ConstantCatalog::getTextureRepeatFlag(){
	return GL_REPEAT;
}
int OpenglES2ConstantCatalog::getFormatRGB(){
	return GL_RGB;
}
int OpenglES2ConstantCatalog::getFormatRGB32F(){
	// Opengl ES 2.0 esta limitado a RGB 8 bits
	return GL_RGB32F_EXT;
}
int OpenglES2ConstantCatalog::getFormatRGB8(){
	return GL_RGB;    // REVIEW: DEBERIA SER RGBA8 PERO NO ESTA SOPORTADO EN OPENGL ES 2 (NI COMO EXT)
}
int OpenglES2ConstantCatalog::getFormatRG(){
	return GL_RG_EXT;
}
int OpenglES2ConstantCatalog::getFormatRG8(){
	return GL_RG8_EXT;
}
int OpenglES2ConstantCatalog::getFormatR(){
	// REVIEW: Cuidado que aca es GL_R, pero opengl es no lo soporta. No es la mejor solucion (no se ni si es valida esta). 
	return GL_R8_EXT;
}
int OpenglES2ConstantCatalog::getFormatR8(){
	return GL_R8_EXT;
}
int OpenglES2ConstantCatalog::getFormatR32F(){
	return GL_R32F_EXT;
}
int OpenglES2ConstantCatalog::getFormatRG32F(){
	return GL_RG32F_EXT;
}
int OpenglES2ConstantCatalog::getFormatRGBA(){
	return GL_RGBA;
}
int OpenglES2ConstantCatalog::getFormatRGBA8(){
	return GL_RGBA;   // REVIEW: DEBERIA SER RGBA8 PERO NO ESTA SOPORTADO EN OPENGL ES 2 (NI COMO EXT)
}
int OpenglES2ConstantCatalog::getFormatDepth24(){
	return GL_DEPTH_COMPONENT16;  // Opengl es 2 no soporta mas de 16 bits para escritura
}
int OpenglES2ConstantCatalog::getFormatDepth(){
	return GL_DEPTH_COMPONENT;
}
int OpenglES2ConstantCatalog::getDepthTestFlag(){
	return GL_DEPTH_TEST;
}
int OpenglES2ConstantCatalog::getBlendFlag(){
	return GL_BLEND;
}
int OpenglES2ConstantCatalog::getBlendSrcAlphaFlag(){
	return GL_SRC_ALPHA;
}
int OpenglES2ConstantCatalog::getBlendOneMinusSrcAlphaFlag(){
	return GL_ONE_MINUS_SRC_ALPHA;
}
int OpenglES2ConstantCatalog::getCullFaceFlag(){
	return GL_CULL_FACE;
}
int OpenglES2ConstantCatalog::getCwFrontFaceFlag(){
	return GL_CW;
}
unsigned int OpenglES2ConstantCatalog::getTimeElapsedQueryType(){
	// Opengl ES no soporta Time Queries. Vamos a ver si no rompe nada con cero (salvo un error de opengl).
	return 0;
}
