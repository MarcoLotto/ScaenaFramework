/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once

class GraphicDeviceConstantCatalog{

public:
	virtual int getTextureAnisotropyFlag() = 0;
	virtual int getTextureMagFilterFlag() = 0;
	virtual int getTextureMinFilterFlag() = 0;
	virtual int getTextureLinearFilteringFlag() = 0;
	virtual int getTextureMipmapLinearFilteringFlag() = 0;
	virtual int getTextureWrapSFlag() = 0;
	virtual int getTextureWrapTFlag() = 0;
	virtual int getTextureClampToEdgeFlag() = 0;
	virtual int getTextureClampToBorderFlag() = 0;
	virtual int getTextureBorderColorFlag() = 0;
	virtual int getTextureNearestFilterFlag() = 0;
	virtual int getTextureRepeatFlag() = 0;

	virtual int getFormatRGB() = 0;
	virtual int getFormatRGB32F() = 0;
	virtual int getFormatRGB8() = 0;
	virtual int getFormatRG() = 0;
	virtual int getFormatRG8() = 0;
	virtual int getFormatR() = 0;
	virtual int getFormatR8() = 0;
	virtual int getFormatR32F() = 0;
	virtual int getFormatRG32F() = 0;
	virtual int getFormatRGBA() = 0;
	virtual int getFormatRGBA8() = 0;
	virtual int getFormatDepth24() = 0;
	virtual int getFormatDepth() = 0;

	virtual int getDepthTestFlag() = 0;

	virtual int getStaticDrawFlag() = 0;
	virtual int getFloatTypeFlag() = 0;
	virtual int getUnsignedIntTypeFlag() = 0;
    virtual int getUnsignedShortTypeFlag() = 0;

	virtual int getBlendFlag()=0;
	virtual int getBlendSrcAlphaFlag()=0;
	virtual int getBlendOneMinusSrcAlphaFlag()=0;

	virtual int getCullFaceFlag()=0;
	virtual int getCwFrontFaceFlag()=0;

	// Soporte de queries
	virtual unsigned int getTimeElapsedQueryType() = 0;
};
