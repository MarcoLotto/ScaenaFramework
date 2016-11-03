/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "png.h"
#include "TextureLoadException.h"
#include "PngTextureLoader.h"
#include "WindowConnector.h"
#include <string.h>
using namespace std;

TextureDataTransfer* PngTextureLoader::loadTexture(string filename)
{
	// Primero consigo la ruta completa del archivo
	string fullFileName = WindowConnector::getBaseApplicationPath() + filename;

	// Abro el archivo de textura
	FILE *fp = fopen(fullFileName.c_str(), "rb");
	if (!fp){
		throw new TextureLoadException("No se puede abrir el archivo " + fullFileName);
	}	

	// El archivo de encabezado del png tiene siempre 8 bytes por convencion
	unsigned char sig[8];

	// Controlo que sea v�lido el encabezado del png
    fread(sig, 1, 8, fp);
    if (!png_check_sig(sig, 8)){
        throw new TextureLoadException("El archivo " + fullFileName + " no es un formato PNG valido");
	}

    // Se realizan validaciones sobre el archivo png
   png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr)
       throw new TextureLoadException("El archivo " + fullFileName + " no se ha podido cargar");
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        throw new TextureLoadException("El archivo " + fullFileName + " no se ha podido cargar");
    }
	if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        throw new TextureLoadException("El archivo " + fullFileName + " no se ha podido cargar");
    }

    png_init_io(png_ptr, fp);
    png_set_sig_bytes(png_ptr, 8);  /* we already read the 8 signature bytes */
    png_read_info(png_ptr, info_ptr);  /* read all PNG info up to image data */

	// Creo la estructura para contener los datos de la textura y cargo el encabezado
	TextureDataTransfer* textureDt = new TextureDataTransfer(); 
	textureDt->filename = fullFileName;

	png_get_IHDR(png_ptr, info_ptr, &textureDt->width, &textureDt->height, &textureDt->bitDepth, &textureDt->colorType, NULL, NULL, NULL);
	
	// Handler de errores
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return NULL;
    }

    /* expand palette images to RGB, low-bit-depth grayscale images to 8 bits,
     * transparency chunks to full alpha channel; strip 16-bit-per-sample
     * images to 8 bits per sample; and convert grayscale to RGB[A] */

	if (textureDt->colorType == PNG_COLOR_TYPE_PALETTE)
        png_set_expand(png_ptr);
	if (textureDt->colorType == PNG_COLOR_TYPE_GRAY && textureDt->bitDepth < 8)
        png_set_expand(png_ptr);
    if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
        png_set_expand(png_ptr);
    if (textureDt->bitDepth == 16)
        png_set_strip_16(png_ptr);
    if (textureDt->colorType == PNG_COLOR_TYPE_GRAY ||
        textureDt->colorType == PNG_COLOR_TYPE_GRAY_ALPHA)
        png_set_gray_to_rgb(png_ptr);


	// Reservo memoria para guardar la textura
    png_read_update_info(png_ptr, info_ptr);

    unsigned int rowbytes = png_get_rowbytes(png_ptr, info_ptr);
	textureDt->numberOfChannels = (int)png_get_channels(png_ptr, info_ptr);
	textureDt->data = new unsigned char[rowbytes*textureDt->height];

    if (textureDt->data == NULL) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return NULL;
    }
	png_bytepp row_pointers;
	if ((row_pointers = (png_bytepp)malloc(textureDt->height*sizeof(png_bytep))) == NULL) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		delete textureDt;
		textureDt = NULL;
        return NULL;
    }
	for (unsigned int i = 0;  i < textureDt->height;  ++i)
        row_pointers[textureDt->height - i - 1] = textureDt->data + i*rowbytes;


    // Leemos la imagen completa
    png_read_image(png_ptr, row_pointers);
	png_read_end(png_ptr, NULL);

    // Borramos la data intermedia y terminamos
    free(row_pointers);
    row_pointers = NULL;
	    
	 if (png_ptr && info_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        png_ptr = NULL;
        info_ptr = NULL;
    }
	fclose(fp);

    return textureDt;
}



void PngTextureLoader::writeTexture(string filename, TextureDataTransfer* textureDt)
{
	// Primero consigo la ruta completa del archivo
	string fullFileName = WindowConnector::getBaseApplicationPath() + filename;

	// Creo el archivo y lo abro
	FILE *fp = fopen(fullFileName.c_str(), "wb");
	if (!fp){
		throw new TextureLoadException("El archivo " + fullFileName + " no se puede abrir/crear para escritura");
	}

	// Inicializo todo
	png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr){
		throw new TextureLoadException("Error preparando para escribir el archivo " + fullFileName);
	}

	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr){
		throw new TextureLoadException("Error preparando para escribir el archivo " + fullFileName);
	}

	if (setjmp(png_jmpbuf(png_ptr))){
		throw new TextureLoadException("Error preparando para escribir el archivo " + fullFileName);
	}

	png_init_io(png_ptr, fp);


	// Se escribe el header
	if (setjmp(png_jmpbuf(png_ptr))){
		throw new TextureLoadException("Error al escribir el header del archivo " + fullFileName);
	}

	png_set_IHDR(png_ptr, info_ptr, textureDt->width, textureDt->height,
	textureDt->bitDepth, textureDt->colorType, PNG_INTERLACE_NONE,
	PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	png_write_info(png_ptr, info_ptr);


	// Se escriben los datos en si
	if (setjmp(png_jmpbuf(png_ptr))){
		throw new TextureLoadException("Error en la escritura del archivo " + fullFileName);
	}

	//Mapeo los datos en formato que espera libpng
	png_bytepp row_pointers;
	if ((row_pointers = (png_bytepp)malloc(textureDt->height*sizeof(png_bytep))) == NULL) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        throw new TextureLoadException("No hay suficiente memoria principal para escribir el archivo " + fullFileName);
    }
	unsigned int rowbytes = textureDt->width * textureDt->numberOfChannels * (textureDt->bitDepth / 8);
	for (unsigned int i = 0;  i < textureDt->height;  ++i){
        row_pointers[i] = textureDt->data + i*rowbytes;
	}

	// Escribo el archivo
	png_write_image(png_ptr, row_pointers);
	

	// Se escribe el fin de archivo
	if (setjmp(png_jmpbuf(png_ptr))){
		throw new TextureLoadException("Error terminando la escritura del archivo " + fullFileName);
	}

	png_write_end(png_ptr, NULL);
	fclose(fp);
	
	// Limpio la data temporal
	if (png_ptr && info_ptr) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        png_ptr = NULL;
        info_ptr = NULL;
    }	
	free(row_pointers);
}

