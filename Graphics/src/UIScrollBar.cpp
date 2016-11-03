/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "UIScrollBar.h"

#define BAR_DEF_LENGTH vec2(0.4f, 0.06f)
#define UP_BUTTON_DEF_LENGTH vec2(0.04f, 0.06f)
#define DOWN_BUTTON_DEF_LENGTH vec2(0.04f, 0.06f)
#define MOVE_BUTTON_DEF_LENGTH vec2(0.06f, 0.06f)

#define DOWN_BUTTON_DEF_POS vec2(0.36f, 0.0f)
#define MOVE_BUTTON_DEF_POS_INI vec2(0.035f, 0.0f)
#define MOVE_BUTTON_DEF_POS_END vec2(0.301f, 0.0f)

#define TEXT_DEF_SIZE 0.07f
#define MAX_FACTOR_VALUE 1.0f

UIScrollBar::UIScrollBar(const char* filename, UIController* uiController) : UIElement(uiController){
	//La ruta de la imagen a cargar
	this->filename = filename;
	this->scrollFactor = 0.0f;
	this->scrollFactorIncrement = 0.1f;
	this->scrollBarTop = MOVE_BUTTON_DEF_POS_END.x;
	this->motionDiference = 0.0f;

	//Como no es un form, tengo que inicializar a mano
	this->initiate();
}

//Incializacion de los estados del elemento 
void UIScrollBar::initiateStates(){
	this->basicState = UIFactory::createState(vec2(0.0f), 0.0f, vec2(1.0f), 1.0f, this->filename);		
}
//Inicializacion de los subelementos
void UIScrollBar::initiateElements(){
	//Los valores de tamaño y separacion entre letras no son importantes aqui
	vec2 scale1 = vec2(0.04f, 0.06f);
	vec2 scale2 = vec2(0.06f, 0.06f);
	this->upButton = UIFactory::createButton(vec2(0.0f, 0.0f), 0.0f, UP_BUTTON_DEF_LENGTH, "<", TEXT_DEF_SIZE, "./Textures/UI/buttonSmall.png", this->uiController);
	this->downButton = UIFactory::createButton(DOWN_BUTTON_DEF_POS, 0.0f, DOWN_BUTTON_DEF_LENGTH, ">", TEXT_DEF_SIZE, "./Textures/UI/buttonSmall.png", this->uiController);
	this->moveButton = UIFactory::createButton(MOVE_BUTTON_DEF_POS_INI, 0.0f, MOVE_BUTTON_DEF_LENGTH, "", TEXT_DEF_SIZE, "./Textures/UI/buttonSmall.png", this->uiController);	
}

void UIScrollBar::bindElementsToStates(){
	this->basicState->addElement(this->upButton);
	this->basicState->addElement(this->downButton);
	this->basicState->addElement(this->moveButton);
	
	this->internalState->makeStateTransition(this->basicState);	
}

//Maneja los eventos que se producen cuando un elemento consigue el foco
void UIScrollBar::handleEvents(){
	//Empiezo evitando verificar nada si no hay eventos
	if(this->onFocusEvent.getStateCode() == NOEVENT_UIEVENTCODE) return;
	if(this->onFocusEvent.getStateCode() == WAITING_UIEVENTCODE) return;
		
	//Compruebo de que elemento se trata el evento y lo proceso
	if(this->onFocusEvent.getAfectedElement() == this->upButton)
		this->processUpButtonEvent();
	else if(this->onFocusEvent.getAfectedElement() == this->downButton)
		this->processDownButtonEvent();
	else if(this->onFocusEvent.getAfectedElement() == this->moveButton)
		this->processMoveButtonEvent();		
	
	//Vacio el evento (No me interesa en este caso mantener estados de hold ni nada de eso)
	this->onFocusEvent.setStateCode(WAITING_UIEVENTCODE);
}

void UIScrollBar::processUpButtonEvent(){
	if(this->onFocusEvent.getStateCode() == ONFOCUS_UIEVENTCODE){
		//Modifico el factor de scroll
		float factor = this->getScrollFactor() - this->scrollFactorIncrement;
		this->setScrollFactor(factor);
	}	
	else if(this->onFocusEvent.getStateCode() == ONFOCUSHOLD_UIEVENTCODE){
		//TODO: aca va la logica por si se quiere que al mantener apretado siga scrolleando
	}
}
void UIScrollBar::processDownButtonEvent(){
	if(this->onFocusEvent.getStateCode() == ONFOCUS_UIEVENTCODE){
		//Modifico el factor de scroll
		float factor = this->getScrollFactor() + this->scrollFactorIncrement;
		this->setScrollFactor(factor);
	}	
	else if(this->onFocusEvent.getStateCode() == ONFOCUSHOLD_UIEVENTCODE){
		//TODO: aca va la logica por si se quiere que al mantener apretado siga scrolleando
	}
}
void UIScrollBar::processMoveButtonEvent(){
	if(this->onFocusEvent.getStateCode() == ONFOCUS_UIEVENTCODE){
		//Guardo la diferencia entre el comienzo de la barra de desplazamiento y la pos del mouse
		this->motionDiference = this->moveButton->getOnFocusEvent()->getCoords().x;	
	}
	if(this->onFocusEvent.getStateCode() == ONFOCUSHOLD_UIEVENTCODE){
		//Calculo el factor que corresponde a la pos del mouse, si es valida modifico el control y el factor
		float factor = this->calculateFactorFromPosition(this->onFocusEvent.getCoords() - vec2(this->motionDiference, 0.0f));
		this->setScrollFactor(factor);		
	}
}

//Calcula el factor correspondiente a una posicion determinada (puede ser menor a cero o mayor a uno)
float UIScrollBar::calculateFactorFromPosition(vec2 coords){
	return (coords.x - MOVE_BUTTON_DEF_POS_INI.x) / (float)(this->scrollBarTop - MOVE_BUTTON_DEF_POS_INI.x);
}

void UIScrollBar::setScrollFactor(float factor){
	//Normalizo el factor
	if(factor < 0.0f)
		factor = 0.0f;	
	else if(factor > MAX_FACTOR_VALUE)
		factor = MAX_FACTOR_VALUE;	
	this->scrollFactor = factor;
	//Modifico la posicion del deslizador de scroll
	float finalPosX = MOVE_BUTTON_DEF_POS_INI.x * (1 - factor) + this->scrollBarTop * factor; 
	this->moveButton->getExternalState()->getActualState()->setPosition(vec2(finalPosX, MOVE_BUTTON_DEF_POS_INI.y));	
}

UIScrollBar::~UIScrollBar(){
	if(this->basicState != NULL){
		delete this->basicState;
		this->basicState = NULL;
	}
	if(this->upButton != NULL){
		delete this->upButton;
		this->upButton = NULL;
	}
	if(this->downButton != NULL){
		delete this->downButton;
		this->downButton = NULL;
	}
	if(this->moveButton != NULL){
		delete this->moveButton;
		this->moveButton = NULL;
	}
}

//Setea el ancho de TODO el control
void UIScrollBar::setWidth(float value){
	vec2 scale = this->getExternalState()->getActualState()->getScale();
	this->getExternalState()->getActualState()->setScale(vec2(scale.x, value * BAR_DEF_LENGTH.y));
	
	scale = this->upButton->getExternalState()->getActualState()->getScale();
	this->upButton->getExternalState()->getActualState()->setScale(vec2(scale.x, value * UP_BUTTON_DEF_LENGTH.y));

	scale = this->downButton->getExternalState()->getActualState()->getScale();
	this->downButton->getExternalState()->getActualState()->setScale(vec2(scale.x, value * DOWN_BUTTON_DEF_LENGTH.y));

	scale = this->moveButton->getExternalState()->getActualState()->getScale();
	this->moveButton->getExternalState()->getActualState()->setScale(vec2(scale.x, value * MOVE_BUTTON_DEF_LENGTH.y));
		
	//Modifico el tamaño de los labels
	this->upButton->getLabel()->setSize(TEXT_DEF_SIZE * value);
	this->downButton->getLabel()->setSize(TEXT_DEF_SIZE * value);
	this->moveButton->getLabel()->setSize(TEXT_DEF_SIZE * value);
}

//Setea el largo de la barra
void UIScrollBar::setHeight(float value){
	vec2 scale = this->getExternalState()->getActualState()->getScale();
	scale.x = value * BAR_DEF_LENGTH.x;
	this->getExternalState()->getActualState()->setScale(scale);
	float posIncrementX = scale.x - BAR_DEF_LENGTH.x;  

	//Recalculo la posicion del boton down
	vec2 pos = this->downButton->getExternalState()->getActualState()->getPosition();
	this->downButton->getExternalState()->getActualState()->setPosition(vec2(pos.x + posIncrementX, pos.y));

	//Recalculo la posicion del boton central
	this->scrollBarTop = MOVE_BUTTON_DEF_POS_END.x + posIncrementX;
	this->setScrollFactor(0.0f);
}
