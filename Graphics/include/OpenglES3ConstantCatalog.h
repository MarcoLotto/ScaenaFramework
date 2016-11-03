/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "GraphicDeviceConstantCatalog.h"

class OpenglES3ConstantCatalog : public GraphicDeviceConstantCatalog{

public:
	int getTextureAnisotropyFlag();
	int getTextureMagFilterFlag();
	int getTextureMinFilterFlag();
	int getTextureLinearFilteringFlag();
	int getTextureMipmapLinearFilteringFlag();
	int getTextureWrapSFlag();
	int getTextureWrapTFlag();
	int getTextureClampToEdgeFlag();
	int getTextureClampToBorderFlag();
	int getTextureBorderColorFlag();
	int getTextureNearestFilterFlag();
	int getTextureRepeatFlag();

	int getFormatRGB();
	int getFormatRGB32F();
	int getFormatRGB8();
	int getFormatRG();
	int getFormatRG8();
	int getFormatR();
	int getFormatR8();
	int getFormatR32F();
	int getFormatRG32F();
	int getFormatRGBA();
	int getFormatRGBA8();
	int getFormatDepth24();
	int getFormatDepth();

	int getDepthTestFlag();

	int getStaticDrawFlag();
	int getFloatTypeFlag();
	int getUnsignedIntTypeFlag();
    int getUnsignedShortTypeFlag();

	int getBlendFlag();
	int getBlendSrcAlphaFlag();
	int getBlendOneMinusSrcAlphaFlag();

	int getCullFaceFlag();
	int getCwFrontFaceFlag();

	unsigned int getTimeElapsedQueryType();
};
