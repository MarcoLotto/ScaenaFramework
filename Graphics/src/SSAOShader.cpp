/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "SSAOShader.h"
#include "MeshFactory.h"
#include "GraphicDevice.h"
#include "RandomTextureGeneratorShader.h"
#include "FrameBufferManager.h"
#include "WindowConnector.h"
#include <vector>
#include <algorithm>
using namespace std;

//***PARAMETROS GENERALES*********************************************************************************
#define VSHADER WindowConnector::getBaseApplicationPath() + "Shaders/SSAOShader.vert"
#define FSHADER WindowConnector::getBaseApplicationPath() + "Shaders/SSAOShader.frag"
#define CANT_VERTICES 4
//********************************************************************************************************

#define AO_MAX_RADIUS 1.0f  //REVIEW: define el radio maximo en donde se toma AO (en coordenadas de view space)
#define AO_MIN_RADIUS 0.0f  //REVIEW: define el radio minimo en donde se toma AO (en coordenadas de view space)

SSAOShader* SSAOShader::instance = NULL;

SSAOShader* SSAOShader::getInstance(){
	if(instance==NULL){
		instance = new SSAOShader();
	}
	return instance;
}

SSAOShader::SSAOShader(){
	this->init(); 
	this->initProyectionMatrix();
	this->rectangleMesh = MeshFactory::getInstance()->createRectangleMesh();
	this->sampleKernel = NULL;
	this->randomTexture = NULL;

	// Genero la textura de valores random para la rotacion del kernel
	this->generateRandomTexture();
}

SSAOShader::~SSAOShader(){
	if(this->randomTexture != NULL){
		// Saco el writelock y dejo que el shader manager se encargue de destruir si es necesario
		this->randomTexture->setWriteLock(false);
		this->randomTexture = NULL;
	}
	if(this->sampleKernel != NULL){
		delete this->sampleKernel;
		this->sampleKernel = NULL;
	}
}

void SSAOShader::initProyectionMatrix(){
	glm::vec3 escalaImagen = glm::vec3(2.0f, -2.0f, -2.0f);
	glm::mat4 proyeccionOrtogonal = glm::mat4(1.0f);
	proyeccionOrtogonal[2][2] = 0.0f;	
	glm::mat4 model = glm::scale(glm::mat4(1.0f), escalaImagen);
	this->PVMmatrix = proyeccionOrtogonal * model;
}

void SSAOShader::draw(Texture* positionTexture, Texture* normalTexture, unsigned int fetchsPerFragment, vec2 imageSize, mat4 sceneProjection){
	//Activo el programa de shaders
	this->use();
	GraphicDevice* graphicDevice = GraphicDevice::getInstance();

	//Cargo matrices de modelo, view, proyeccion y normal
	this->setUniform(this->pvmMatrixUniform, this->PVMmatrix);
	this->setUniform(this->imageSizeUniform, imageSize);
	positionTexture->assignToShader(this, this->depthTexUniform, 0);

	// Cargo la matriz de proyeccion de la escena la cual se renderizo (para hacer lineal el depth)
	this->setUniform(this->sceneProjUniform, sceneProjection);
	
	// Cargo el vector del sample kernel con las muestras a probar (es un float* de 3*numero de muestras)
	float* sampleKernel = this->getSampleKernel(fetchsPerFragment);
	this->setVectorUniform(this->sampleKernelUniform, 3*fetchsPerFragment, sampleKernel);
	this->setUniform(this->samplesCountUniform, fetchsPerFragment);
	
	// Cargo la textura random para hacer rotar el kernel
	this->randomTexture->assignToShader(this, this->randomTextureUniform, 1);
	this->setUniform(this->randomTextureSizeUniform, vec2(4, 4));

	// Asignamos la textura de normales
	normalTexture->assignToShader(this, this->normalTexUniform, 2);
	
	//Mando a renderizar la primer pasada
	graphicDevice->drawMesh(this->rectangleMesh);
}

// Consigue un valor random entre los dos pasados como parametro
float SSAOShader::random(float a, float b) {
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}

// Genero el kernel con las muestras a probar occlusion
float* SSAOShader::getSampleKernel(unsigned int samplesCount){
	if(this->sampleKernel == NULL){
		vector<vec3>* unorderedSamplePoints = new vector<vec3>(); 
		for (unsigned int i = 0; i < 3*samplesCount; i+=3) {			
			vec3 samplePoint = vec3(random(-1.0f, 1.0f)*random(0.0f, 1.0f), random(-1.0f, 1.0f)*random(0.0f, 1.0f), random(0.0f, 1.0f)*random(0.0f, 1.0f));
			samplePoint = glm::normalize(samplePoint);
			unorderedSamplePoints->push_back(samplePoint);
		}
		this->orderKernelByDistanceToOrigin(unorderedSamplePoints);
		delete unorderedSamplePoints;
	}
	return this->sampleKernel;
}

// Compara dos puntos por su modulo
bool distanceOrderingFunctions (vec3 p1, vec3 p2) { 
	return (p1.length() < p2.length()); 
}

void SSAOShader::orderKernelByDistanceToOrigin(vector<vec3>* unorderedSamplePoints){
	// Primero ordeno el vector por la distancia de los puntos
	std::sort(unorderedSamplePoints->begin(), unorderedSamplePoints->end(), distanceOrderingFunctions);

	// Guardo los valores ordenados en el sampleKernel final
	unsigned int j = 0;
	this->sampleKernel = new float[unorderedSamplePoints->size()*3]();
	for (unsigned int i=0; i < unorderedSamplePoints->size(); i++) {
		vec3 samplePoint = unorderedSamplePoints->at(i);
		this->sampleKernel[j] = AO_MIN_RADIUS + samplePoint.x * (AO_MAX_RADIUS - AO_MIN_RADIUS);
		this->sampleKernel[j+1] = AO_MIN_RADIUS + samplePoint.y * (AO_MAX_RADIUS - AO_MIN_RADIUS);
		this->sampleKernel[j+2] = AO_MIN_RADIUS + samplePoint.z * (AO_MAX_RADIUS - AO_MIN_RADIUS);
		j+=3;
	}
}

void SSAOShader::generateRandomTexture(){
	// Genero un request para la nueva textura de random a crear
	GraphicDeviceConstantCatalog* catalog = GraphicDevice::getInstance()->getConstantCatalog();
	FrameBufferRequest* request = new FrameBufferRequest();
	TextureRequest* textureRequest = new TextureRequest(4, 4, catalog->getFormatRGB(), catalog->getFormatRGB());
	textureRequest->addFilterRequest(new TextureFilterRequest(catalog->getTextureMagFilterFlag(), catalog->getTextureNearestFilterFlag()));
	textureRequest->addFilterRequest(new TextureFilterRequest(catalog->getTextureMinFilterFlag(), catalog->getTextureNearestFilterFlag()));
	textureRequest->addFilterRequest(new TextureFilterRequest(catalog->getTextureWrapSFlag(), catalog->getTextureRepeatFlag()));
	textureRequest->addFilterRequest(new TextureFilterRequest(catalog->getTextureWrapTFlag(), catalog->getTextureRepeatFlag()));
	request->addTextureRequest(textureRequest);

	// Consigo un fbo, dibujo en el, y vuelvo al fbo anterior
	FrameBuffer* currentFbo = FrameBufferManager::getInstance()->getActiveFrameBuffer();
	static FrameBuffer* randomTextureFbo = NULL; 
	if(randomTextureFbo != NULL){
		randomTextureFbo->getOutputTextures()->at(0)->setWriteLock(false);
	}
	randomTextureFbo = FrameBufferManager::getInstance()->getFrameBufferAndBind(request, randomTextureFbo);	
	GraphicDevice::getInstance()->setViewport(0, 0, 4, 4);
	RandomTextureGeneratorShader::getInstance()->draw(vec2(4, 4), 0.0f, 1.0f);
	currentFbo->bind();
	vec2 size = currentFbo->getOutputTextures()->at(0)->getSize();
	GraphicDevice::getInstance()->setViewport(0, 0, size.x, size.y);

	// Me guardo la textura (no libero el writelock del fbo, porque sino se me borra la textura)
	this->randomTexture = randomTextureFbo->getOutputTextures()->at(0);
	delete request;
}

void SSAOShader::init(){		
	list<string> inputAttributes;
	inputAttributes.push_back("VertexPosition");
	inputAttributes.push_back("TextureCoords");
	
	list<string> outputAttributes;
	outputAttributes.push_back("OcclusionData");

	this->setVertexShaderName(VSHADER);
	this->setFragmentShaderName(FSHADER);
	this->initialize(&inputAttributes, &outputAttributes);

	// Identifico los uniforms a utilizar
	this->pvmMatrixUniform = new GLSLUniform("PVMmatrix", this);
	this->imageSizeUniform = new GLSLUniform("imageSize", this);
	this->depthTexUniform = new GLSLUniform("DepthTex", this);
	this->sceneProjUniform = new GLSLUniform("sceneProj", this);
	this->sampleKernelUniform = new GLSLUniform("sampleKernel", this);
	this->samplesCountUniform = new GLSLUniform("samplesCount", this);
	this->randomTextureUniform = new GLSLUniform("randomTexture", this);
	this->randomTextureSizeUniform = new GLSLUniform("randomTextureSize", this);
	this->normalTexUniform = new GLSLUniform("NormalTex", this);
}
