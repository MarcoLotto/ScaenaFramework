/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "TeamBuilderForm.h"
#include "Logger.h"
#include "CamaraTerceraPersona.h"
#include "SceneLoader.h"
#include "GenericUtils.h"
#include "JsonUtils.h"
#include "ObjectFactory.h"
#include "ProgramParameters.h"
#include "FormsManager.h"
#include "TeamBuilderDataForm.h"
#include "CustomizableTexture.h"
#include "MenuStatusBarForm.h"
#include "WindowConnector.h"

#define HIDE_TRANSITION_END_CALLBACK_ID 0

#define SHIRT_ROTATION_TIME 3500
#define SHIRT_MAX_ROTATION_ANGLE 10.0f
#define MANUAL_ROTATION_ANGLE_INCREMENT 30.0f
#define COLOR_GRAPHIC_LISTS_COUNT 4  // Solo con modificar este valor, ya se modifica la cantidad maxima de graphiclists para colores

TeamBuilderForm::TeamBuilderForm(UIController* uiController, string builderConfigFilename, TeamBuilderForm* nextBuilder) : UIForm(uiController){
	for(unsigned int i=0; i < COLOR_GRAPHIC_LISTS_COUNT; i++){
		this->lastSelectedColorIndexes.push_back(i);
	}
	this->lastTemplateIndex = 0;	
	this->goingToTeamDetailsScreen = false;
	this->visible = false;
	this->manualRotationAngle = 0.0f;
	this->nextBuilder = nextBuilder;
	this->builderConfigFilename = builderConfigFilename;
	this->rotationInterpolator = new TimeParameterInterpolator(-SHIRT_MAX_ROTATION_ANGLE, SHIRT_MAX_ROTATION_ANGLE, SHIRT_ROTATION_TIME);
	this->rotationInterpolator->setInverseAnimationEnabled(true);
	this->rotationInterpolator->setPlay();
	this->initiate();	
}

//Incializacion de los estados del elemento 
void TeamBuilderForm::initiateStates(){		
	this->showedState = UIFactory::createState(vec2(0.05f, 0.15f), 0.0f, vec2(0.9f, 0.8f), 1.0f, "Textures/UI/FormBackground1.png");
	this->hidedState = UIFactory::createState(vec2(1.0f, 0.15f), 0.0f, vec2(0.9f, 0.8f), 0.0f, "Textures/UI/FormBackground1.png");
}

//Inicializacion de los subelementos
void TeamBuilderForm::initiateElements(){
	this->titleLabel = UIFactory::createLabel("", vec2(0.05f, 0.05f), 0.2f, "Textures/UI/font2.png", this->uiController);
	this->continueButton = UIFactory::createButton(vec2(0.695, 0.69f), 0.0f, vec2(0.2f, 0.1f), "Continuar", 0.13f, vec2(0.032f, 0.026f), "./Textures/UI/Button1.png", "Textures/UI/font2.png", this->uiController);
	
	this->rotateRightButton = UIFactory::createButton(vec2(0.32f, 0.62f), 0.0f, vec2(0.08f, 0.1f), "", 0.0f, vec2(0.0f, 0.0f), "./Textures/UI/Button1.png", "Textures/UI/font2.png", this->uiController);
	this->rotateLeftButton = UIFactory::createButton(vec2(0.41f, 0.62f), 0.0f, vec2(0.08f, 0.1f), "", 0.0f, vec2(0.0f, 0.0f), "./Textures/UI/Button1.png", "Textures/UI/font2.png", this->uiController);
	
	this->itemTemplatesGraphicList = UIFactory::createGraphicList(vec2(0.01f, 0.01f), 0.0f, vec2(0.13f, 0.78f), 4, "./Textures/UI/Button1.png", "./Textures/UI/Button1.png",
		"./Textures/UI/transparent.png", vec2(0.017f, 0.005f), vec2(0.09f, 0.11f), this->uiController);

	float colorListHorizontalPosition = 0.82f;
	for(unsigned int i=0; i < COLOR_GRAPHIC_LISTS_COUNT; i++){
		this->colorGraphicLists.push_back(UIFactory::createGraphicList(vec2(colorListHorizontalPosition, 0.01f), 0.0f, vec2(0.07f, 0.66f), 6, "./Textures/UI/Button1.png", 
			"./Textures/UI/Button1.png", "./Textures/UI/transparent.png", vec2(0.01f, 0.01f), vec2(0.05f, 0.06f), this->uiController));
		colorListHorizontalPosition -= 0.075f;
	}

	// Cargamos la escena para mostrar la remera
	Scene* scene = SceneLoader::loadFromXmlFile("Definitions/Scenes/TeamShirtShower.xml");
	CamaraTerceraPersona* camera = new CamaraTerceraPersona();
	camera->setDistanceToPoint(-2.1f);
	camera->setLookAtPoint(vec3(0.0f, 0.0f, 0.15f));	
	camera->setRotationPitch(-70.8f);	

	// Recalculamos la perspectiva de la camara
	vec2 screen = this->uiController->getScreenSize();
	camera->setPerspective(80.0f, screen.x/screen.y, 0.1f, 1000.0f);

	camera->update();
	scene->setCurrentCamera(camera);
	this->drawer = UIFactory::create3dDrawable(vec2(0.2, 0.2f), 0.0f, vec2(0.4f, 0.4f), scene, this->uiController);
}

void TeamBuilderForm::bindElementsToStates(){
	// Asigno a cada estado los elementos que contendran	
	this->showedState->addElement(this->titleLabel);	
	this->showedState->addElement(this->continueButton);
	this->showedState->addElement(this->itemTemplatesGraphicList);
	this->showedState->addElement(this->drawer);
	this->showedState->addElement(this->rotateRightButton);
	this->showedState->addElement(this->rotateLeftButton);
	
	this->hidedState->addElement(this->drawer);
	this->hidedState->addElement(this->continueButton);
	this->hidedState->addElement(this->itemTemplatesGraphicList);
	this->hidedState->addElement(this->titleLabel);
	this->hidedState->addElement(this->rotateRightButton);
	this->hidedState->addElement(this->rotateLeftButton);

	for(unsigned int i=0; i < this->colorGraphicLists.size(); i++){
		this->showedState->addElement(this->colorGraphicLists[i]);
		this->hidedState->addElement(this->colorGraphicLists[i]);
	}		
	//Inicializo el estado inicial
	this->internalState->makeStateTransition(this->hidedState);	
}

//Maneja los eventos que se producen cuando un elemento consigue el foco
void TeamBuilderForm::handleEvents(){
	//Empiezo evitando verificar nada si no hay eventos
	if(this->onFocusEvent.getStateCode() == NOEVENT_UIEVENTCODE) return;
	if(this->onFocusEvent.getStateCode() == WAITING_UIEVENTCODE) return;

	//Compruebo de que elemento se trata el evento y lo proceso	
	if( this->onFocusEvent.getStateCode() == ONFOCUSRELEASE_UIEVENTCODE ){

		if(this->onFocusEvent.getAfectedElement() == this->rotateRightButton){
			this->processRotateRightButtonEvent();	
		}
		else if(this->onFocusEvent.getAfectedElement() == this->rotateLeftButton){
			this->processRotateLeftButtonEvent();	
		}
		
		this->onFocusEvent.setStateCode(WAITING_UIEVENTCODE);
		
		if(this->onFocusEvent.getAfectedElement() == this->continueButton){
			this->processContinueButtonEvent();	
		}
		else if(this->onFocusEvent.getAfectedElement() == this->itemTemplatesGraphicList && this->itemTemplatesGraphicList->getSelectedItemPosition() >= 0){				
			// Actualizamos la textura mostrada
			this->onTeamObjectDataChange();	
		}
		else{
			for(unsigned int i=0; i < this->colorGraphicLists.size(); i++){
				if(this->onFocusEvent.getAfectedElement() == this->colorGraphicLists[i] && this->colorGraphicLists[i]->getSelectedItemPosition() >= 0){				
					// Actualizamos la textura mostrada
					this->onTeamObjectDataChange();	
				}
			}	
		}
	}
	//Vacio el evento
	this->onFocusEvent.setStateCode(WAITING_UIEVENTCODE);
}

TeamBuilderForm::~TeamBuilderForm(){
	delete this->showedState;
	delete this->hidedState;
	delete this->titleLabel;
	delete this->drawer;
	delete this->continueButton;
	delete this->itemTemplatesGraphicList;
	for(unsigned int i=0; i < this->colorGraphicLists.size(); i++){
		delete this->colorGraphicLists[i];
	}
	delete this->rotationInterpolator;
}

void TeamBuilderForm::saveBuilderData(){
	TeamTemplate* customTemplate = &this->templates[this->lastTemplateIndex];
	string usedTemplate = customTemplate->templateFilename;
	vector<vec3> usedColors;
	for(unsigned int i=0; i < this->lastSelectedColorIndexes.size() && i < customTemplate->replacementColors.size(); i++){
		usedColors.push_back(this->colors[this->lastSelectedColorIndexes[i]]);		
	}

	// REVIEW: Ver de hacerlo mas generico, si no por cada archivo de configuracion hay que declararlo aca
	if(this->builderConfigFilename.compare(SHIRTS_BUILDER_CONFIG_FILENAME)==0){
		this->dataTransfer.shirtTemplate = usedTemplate;
		this->dataTransfer.shirtColors = usedColors;
	}
	else if(this->builderConfigFilename.compare(PANTS_BUILDER_CONFIG_FILENAME)==0){
		this->dataTransfer.pantsTemplate = usedTemplate;
		this->dataTransfer.pantsColors = usedColors;
	}
	else if(this->builderConfigFilename.compare(SOCKS_BUILDER_CONFIG_FILENAME)==0){
		this->dataTransfer.socksTemplate = usedTemplate;
		this->dataTransfer.socksColors = usedColors;
	}
}

void TeamBuilderForm::processContinueButtonEvent(){
	// Juntamos en el data transfer los datos que conseguimos en este builder
	this->saveBuilderData();

	if(this->nextBuilder != NULL){
		// Invocamos al siguiente builder y le pasamos la data que tenemos hasta ahora
		this->nextBuilder->setTeamBuilderDataTransfer(this->dataTransfer);
		((MenuStatusBarForm*) FormsManager::getInstance()->menuStatusBarForm)->addFormStackRegister(this, this->nextBuilder);		
		this->nextBuilder->setVisible(true);	
		this->setVisible(false);
	}
	else{
		// Vamos al form de conseguir data para el builder
		TeamBuilderDataForm* teamBuilderDataForm = (TeamBuilderDataForm*) FormsManager::getInstance()->teamBuilderDataForm;	
		((MenuStatusBarForm*) FormsManager::getInstance()->menuStatusBarForm)->addFormStackRegister(this, teamBuilderDataForm);
		teamBuilderDataForm->setTeamBuilderDataTransfer(this->dataTransfer);
		teamBuilderDataForm->setVisible(true);
		this->setVisible(false);		
	}
}

void TeamBuilderForm::processRotateRightButtonEvent(){
	this->manualRotationAngle -= MANUAL_ROTATION_ANGLE_INCREMENT;
	if(this->manualRotationAngle >= 360.0f || this->manualRotationAngle <= -360.0f){
		this->manualRotationAngle = 0.0f;
	}
}
void TeamBuilderForm::processRotateLeftButtonEvent(){
	this->manualRotationAngle += MANUAL_ROTATION_ANGLE_INCREMENT;
	if(this->manualRotationAngle >= 360.0f || this->manualRotationAngle <= -360.0f){
		this->manualRotationAngle = 0.0f;
	}
}

void TeamBuilderForm::clearResources(){
	// Sacamos todos los objetos del drawer
	ObjectManager* objectManager = this->drawer->getSceneToDraw()->getObjectManager();			
	objectManager->clear();
	
	// Borramos el objeto customizable
	if(this->objectToDraw != NULL){
		delete this->objectToDraw;
		this->objectToDraw = NULL;
	}
	// Limpiamos las graphicLists, con eso se borraran las texturas que contengan
	this->itemTemplatesGraphicList->clear();
	for(unsigned int i=0; i < this->colorGraphicLists.size(); i++){
		this->colorGraphicLists[i]->clear();
	}

	// Limpiamos las listas, indices, etc
	this->templates.clear();
	this->colors.clear();
	this->lastTemplateIndex = 0;
	for(unsigned int i=0; i < COLOR_GRAPHIC_LISTS_COUNT; i++){
		this->lastSelectedColorIndexes.push_back(i);
	}
}

void TeamBuilderForm::update(UIState* parentState){

	// Actualizo la rotacion del objeto mostrado	
	if(this->objectToDraw != NULL){
		float currentAngle = this->rotationInterpolator->getCurrentValue();
		if(GenericUtils::modulo(currentAngle) < SHIRT_MAX_ROTATION_ANGLE){			
			this->objectToDraw->getTransformation()->setRotation(vec3(0.0f, 0.0f, this->manualRotationAngle + currentAngle));
		}
	}
	// Actualizo la camara
	this->drawer->getSceneToDraw()->getCurrentCamera()->update();
	
 	//Mergeo los estados
	UIState finalState = this->mergeParameters(parentState);

	//Update all
	this->internalState->update(&finalState);
	this->externalState->update(&finalState);	
}

void TeamBuilderForm::setVisible(bool visible){ 
	if(visible && !this->visible){
		// Al hacer visible el form, cargamos toda la info necesaria
		this->readBuilderConfigFile();
	}
	if(visible){
		this->visible = true;
		this->internalState->makeStateTransition(this->showedState, 550, false);
	}
	else{
		this->internalState->makeStateTransition(this->hidedState, 550, false, this, HIDE_TRANSITION_END_CALLBACK_ID);
	}
}

void TeamBuilderForm::onScreenChange(){
	// Limpiamos la data de dibujado
	this->clearResources();
}

void TeamBuilderForm::onCallback(int callbackId){
	switch(callbackId){	
	case HIDE_TRANSITION_END_CALLBACK_ID:
		this->visible = false;
		this->onScreenChange();	
		break;
	}
}

void TeamBuilderForm::onTeamObjectDataChange(){
	if(this->templates.empty()) return;
	if(this->colors.empty()) return;

	// Desabilitamos las listas de colores para habilitar solo las que el template requiere
	for(unsigned int i=0; i < this->colorGraphicLists.size(); i++){
		this->colorGraphicLists[i]->getExternalState()->getActualState()->setTransparency(0.0f);	
	}

	// Conseguimos la informacion del template activo
	int templateIndex = this->itemTemplatesGraphicList->getSelectedItemPosition();
	if(templateIndex < 0) templateIndex = this->lastTemplateIndex;
	this->lastTemplateIndex = templateIndex;

	// Conseguimos la informacion de los colores activos
	for(unsigned int i=0; i < this->colorGraphicLists.size(); i++){
		int colorIndex = this->colorGraphicLists[i]->getSelectedItemPosition();		
		if(colorIndex < 0 || colorIndex >= this->colors.size()) colorIndex = this->lastSelectedColorIndexes[i];		
		this->lastSelectedColorIndexes[i] = colorIndex;
	}	
		
	// A partir de la info de template y colores, generamos la textura final para el objeto
	TeamTemplate* teamTemplate = &this->templates[templateIndex];
	CustomizableTexture* customTexture = new CustomizableTexture(teamTemplate->templateFilename);
	for(unsigned int i=0; (i < teamTemplate->replacementColors.size()) && (i < this->colorGraphicLists.size()); i++){	
		customTexture->addReplacementColor(teamTemplate->replacementColors[i], this->colors[this->lastSelectedColorIndexes[i]]);
		this->colorGraphicLists[i]->getExternalState()->getActualState()->setTransparency(1.0f);
	}

	// Le aplicamos la textura al objeto
	if(this->objectToDraw->getMesh()->getChilds()->size() > 0){
		Texture* colorTexture = customTexture->getCustomizedTexture();
		this->objectToDraw->getMesh()->getChilds()->at(0)->getDiffuseMap()->setMap(colorTexture);			
	}
	delete customTexture;	
}

void TeamBuilderForm::readBuilderConfigFile(){
	string objectFilename, value1, value2, value3;
	vector<string> templateIconsFilenames;	
	this->colors.clear();
	this->templates.clear();
	Document jsonDestination;
	Value jsonArray, jsonColorsArray;
	string filename = WindowConnector::getBaseApplicationPath() + this->builderConfigFilename;
	try{
		JsonUtils::parseJsonFromFileWExc(jsonDestination, filename.c_str());
		JsonUtils::getJsonStringWExc(jsonDestination, "object", objectFilename);
		JsonUtils::getJsonArrayWExc(jsonDestination, "templates", &jsonArray);
		for(int i=0; i < jsonArray.Size(); i++){
			TeamTemplate teamTemplate;
			JsonUtils::getJsonStringWExc(jsonArray[i], "icon", value1);
			templateIconsFilenames.push_back(value1);
			JsonUtils::getJsonStringWExc(jsonArray[i], "template", teamTemplate.templateFilename);			
			JsonUtils::getJsonArrayWExc(jsonArray[i], "replacementColors", &jsonColorsArray);
			for(int index=0; index < jsonColorsArray.Size(); index++){
				JsonUtils::getJsonStringWExc(jsonColorsArray[index], "r", value1);
				JsonUtils::getJsonStringWExc(jsonColorsArray[index], "g", value2);
				JsonUtils::getJsonStringWExc(jsonColorsArray[index], "b", value3);
				teamTemplate.replacementColors.push_back(vec3(atof(value1.c_str())/255.0f, atof(value2.c_str())/255.0f, atof(value3.c_str())/255.0f));
			}
			this->templates.push_back(teamTemplate);
		}
		JsonUtils::getJsonArrayWExc(jsonDestination, "colors", &jsonArray);
		for(int i=0; i < jsonArray.Size(); i++){
			JsonUtils::getJsonStringWExc(jsonArray[i], "r", value1);
			JsonUtils::getJsonStringWExc(jsonArray[i], "g", value2);
			JsonUtils::getJsonStringWExc(jsonArray[i], "b", value3);
			this->colors.push_back(vec3(atof(value1.c_str())/255.0f, atof(value2.c_str())/255.0f, atof(value3.c_str())/255.0f));
		}
	}
	catch(DataConformationException e){
		Logger::getInstance()->logError(new Log("ERROR FATAL: No se ha encontrado el archivo de team builder " + filename + " o esta corrupto"));
	}
	// Ya procesamos todo el archivo de configuracion, ahora vamos a empezar a cargar los archivos
	this->processObjectToShow(objectFilename);
	this->processTemplateList(templateIconsFilenames);
	this->processColorLists(this->colors);

	// Actualizamos los datos actuales del objeto mostrado
	this->onTeamObjectDataChange();
}

void TeamBuilderForm::processObjectToShow(string objectFilename){
	objectFilename = OBJECTS_DIR + objectFilename;
	this->objectToDraw = ObjectFactory::loadObjectFromFile(objectFilename.c_str());
	if(objectToDraw != NULL){
		this->drawer->getSceneToDraw()->getObjectManager()->addObject(objectToDraw);
	}
	else{
		Logger::getInstance()->logError(new Log("No se pudo cargar el objeto de team builder " + string(objectFilename)));
	}
}

void TeamBuilderForm::processTemplateList(vector<string> &templateIconsFilenames){
	vector<UIGraphicListItem> items;
	// Cargamos cada una de las imagenes de templates (ej de remeras o pantalones) y las asignamos al graphiclist
	for(unsigned int i=0; i < templateIconsFilenames.size(); i++){
		// No hay problema con estas texturas, las texturas se borran con el UIGraphicList (si hace falta)
		Texture* templateTexture = new FileTexture((TEXTURES_DIFFUSE_DIR + templateIconsFilenames[i]).c_str());  
		items.push_back(UIGraphicListItem("", templateTexture)); 
	}
	this->itemTemplatesGraphicList->setListItems(items);
}

void TeamBuilderForm::processColorLists(vector<vec3> &colors){
	vector<vector<UIGraphicListItem>> collectionItems;
	for(unsigned int i=0; i < COLOR_GRAPHIC_LISTS_COUNT; i++){
		vector<UIGraphicListItem> items;
		collectionItems.push_back(items);
	}
	// A partir de la informacion de cada color, generamos las texturas y se las asignamos a las listas de colores
	for(unsigned int i=0; i < colors.size(); i++){	 	
		CustomizableTexture* customTexture = new CustomizableTexture("black");  // Usamos la textura "black.png" de base
		customTexture->addReplacementColor(vec3(0.0f), colors[i]);
		Texture* colorTexture = customTexture->getCustomizedTexture();

		// No hay problema con estas texturas, las texturas se borran con el UIGraphicList (si hace falta)			
		for(unsigned int i=0; i < collectionItems.size(); i++){
			collectionItems[i].push_back(UIGraphicListItem("", colorTexture->clone())); 
		}
		delete colorTexture;		
	}
	// Asignamos a las graphic lists de colores, las listas de colores
	for(unsigned int i=0; i < COLOR_GRAPHIC_LISTS_COUNT; i++){
		this->colorGraphicLists[i]->setListItems(collectionItems[i]);
	}	
}