#include "UIList.h"
#include "UIScrollBar.h"

#define BASE_SCROLLBAR_LENGTH 0.4f

UIList::UIList(const char* filename, int displayElementsCount, float listHeight, UIController* uiController) : UIElement(uiController){
	//La ruta de la imagen a cargar
	this->filename = filename;
	this->displayElementsCount = displayElementsCount;
	this->selectedElement = NULL;
	this->selectedElementId = -1;
	this->items = new vector<UIElement*>();
	this->listHeight = listHeight;
	this->selectedButton = NULL;

	this->initializeButtonItems(displayElementsCount, listHeight);

	//Como no es un form, tengo que inicializar a mano
	this->initiate();
}

void UIList::initializeButtonItems(int displayElementsCount, float listHeight){
	//Creo y agrego todos los botones para los items, calculando su posicion y escala
	vec2 scale = vec2(1.0f,  1.0f / (float)displayElementsCount);
	this->buttonItems = new list<UICheckBox*>();
	for(int i=0; i < displayElementsCount; i++){
		vec2 pos = vec2(0.0f, i * scale.y * listHeight);		
		UICheckBox* button = UIFactory::createCheckBox(pos, 0.0f, scale, "./Textures/UI/listItemBack.png", this->uiController); 
		button->setParentMergeAllowed(true);
		this->buttonItems->push_back(button);
	}
}

//Incializacion de los estados del elemento 
void UIList::initiateStates(){
	this->basicState = UIFactory::createState(vec2(0.0f), 0.0f, vec2(1.0f, 1.0f), 1.0f, this->filename);
	this->withScrollBarState = UIFactory::createState(vec2(0.0f), 0.0f, vec2(1.0f, 1.0f), 1.0f, this->filename);		
}

//Inicializacion de los subelementos
void UIList::initiateElements(){
	vec2 listScale = this->externalState->getActualState()->getScale();
	float barLength = this->listHeight / BASE_SCROLLBAR_LENGTH;
	this->scrollbar = UIFactory::createScrollBar(vec2(0.4f * listScale.x, 0.0f), 90.0f, listScale.x * 0.4f, barLength, "./Textures/UI/textbox1.png", this->uiController);
}

void UIList::bindElementsToStates(){
	//Asigno los botones disponibles a los estados
	std::list<UICheckBox*>::iterator it = this->buttonItems->begin();
	while(it != this->buttonItems->end()){
		UICheckBox* button = (*it);
		this->basicState->addElement(button);
		this->withScrollBarState->addElement(button);
		++it;
	}

	//Asigno la scrollBar
	this->withScrollBarState->addElement(this->scrollbar);
	
	this->internalState->makeStateTransition(this->basicState);	
}

void UIList::draw(){	
	//Dibujo el fondo del control	
	this->internalState->draw();

	//Calculo el primer elemento a mostrar
	float factor = this->scrollbar->getScrollFactor();
	int startIndex = (int)(factor * (this->items->size() - this->displayElementsCount));
	
	//Me fijo si tengo menos elementos de los que puede mostrar el control
	int elementsToDraw = this->displayElementsCount;
	if(elementsToDraw > this->items->size())
		elementsToDraw = this->items->size();
	
	//Dibujo los elementos!!
	int posIndex = 0;
	for(int i=startIndex; i < elementsToDraw + startIndex; i++){
		this->items->at(i)->draw();
		posIndex++;
	}	
}

void UIList::update(UIState* parentState){	
	//Mergeo los estados
	UIState finalState = this->mergeParameters(parentState);

	//Update them all
	this->internalState->update(&finalState);
	this->externalState->update(&finalState);
	
	//Ahora voy identificar que elementos de la lista mostrar y actualizarlos
	//Calculo el primer elemento a mostrar
	float factor = this->scrollbar->getScrollFactor();
	int startIndex = (int)(factor * (this->items->size() - this->displayElementsCount));
	
	//Me fijo si tengo menos elementos de los que puede mostrar el control
	int elementsToDraw = this->displayElementsCount;
	if(elementsToDraw > this->items->size())
		elementsToDraw = this->items->size();

	//Hago calculos de posicion de los elementos
	float scale = 1.0f / (float)this->displayElementsCount;
	vec2 pos = vec2(0.0f, scale * this->listHeight);
	
	//Actualizo los elementos
	int posIndex = 0;
	for(int i=startIndex; i < elementsToDraw + startIndex; i++){
		UIElement* element = this->items->at(i);
		element->getExternalState()->getActualState()->setPosition(vec2(posIndex * pos.x, posIndex * pos.y));
		element->update(&finalState);
		posIndex++;
	}		
}

//Maneja los eventos que se producen cuando un elemento consigue el foco
void UIList::handleEvents(){
	//Empiezo evitando verificar nada si no hay eventos
	if(this->onFocusEvent.getStateCode() == NOEVENT_UIEVENTCODE) return;
	if(this->onFocusEvent.getStateCode() == WAITING_UIEVENTCODE) return;
		
	//Compruebo de que elemento se trata el evento y lo proceso
	if((this->onFocusEvent.getStateCode() == ONFOCUSRELEASE_UIEVENTCODE)){
		this->findClickedElementEvent();	
	}
	//Vacio el evento (No me interesa en este caso mantener estados de hold ni nada de eso)
	this->onFocusEvent.setStateCode(WAITING_UIEVENTCODE);
}

void UIList::findClickedElementEvent(){
	//Recorro la lista de botones de item tratando de encontrar de que elemento se trata
	//Es ineficiente, pero nunca van a ser muchos botones y solo se ejecuta al hacer click, no es un gran impacto
	UIElement* elementToFind = this->onFocusEvent.getAfectedElement();
	unsigned int index = 0;
	for(list<UICheckBox*>::iterator it = this->buttonItems->begin(); it != this->buttonItems->end(); it++){
		UIElement* element = (*it);
		if(element == elementToFind){
			//Encontre el boton que se presiono, me traigo el elemento que le corresponde
			if(index < this->items->size()){
				//Descelecciono el boton anterior y selecciono el actual
				if(this->selectedButton != NULL) this->selectedButton->setChecked(false);
				this->selectedButton = (*it);
				this->selectedButton->setChecked(true);
				//Consigo el item asociado a ese boton
				float factor = this->scrollbar->getScrollFactor();
				int startIndex = (int)(factor * (this->items->size() - this->displayElementsCount));
				this->selectedElementId = startIndex + index;
				this->selectedElement = this->items->at(this->selectedElementId);
				break;
			}
			//Es un click invalido
			(*it)->setChecked(false);
		}
		index++;
	}
}

void UIList::clear(){	
	//Primero borro los elementos en si
	for(unsigned int i=0; i < this->items->size(); i++){		
		this->deleteElement(this->items->at(i));
	}
	//Luego los borro de la lista
	this->items->clear();
	this->selectedElement = NULL;
	this->selectedElementId = -1;
	//Saco la barra de scroll
	this->internalState->makeStateTransition(this->basicState);
}

UIList::~UIList(){
	//Borro los elementos contenidos
	this->clear();

	//Borro todo el resto
	if(this->basicState != NULL){
		delete this->basicState;
		this->basicState = NULL;
	}
	if(this->withScrollBarState != NULL){
		delete this->withScrollBarState;
		this->withScrollBarState = NULL;
	}
	if(this->scrollbar != NULL){
		delete this->scrollbar;
		this->scrollbar = NULL;
	}
	//Borro todos los botones de items y la lista
	std::list<UICheckBox*>::iterator it = this->buttonItems->begin();
	while(it != this->buttonItems->end()){
		delete (*it);
		it = this->buttonItems->erase(it);
	}
	delete this->buttonItems;	
}

void UIList::addElement(UIElement* element){
	//Escalo el elemento
	float scale = this->listHeight / (float)this->displayElementsCount;
	UIState* state = element->getExternalState()->getActualState();
	state->setScale(vec2(scale) * state->getScale());
	
	//Agrego el elemento
	this->items->push_back(element);

	//Verifico si es necesario poner o sacar el scrollBar
	if(this->items->size() > this->displayElementsCount){
		this->internalState->makeStateTransition(this->withScrollBarState);
		//Modifico la precision del scrollbar
		this->scrollbar->setScrollFactorIncrement(1.0f/(this->items->size() - this->displayElementsCount));
	}
	else{
		this->internalState->makeStateTransition(this->basicState);
	}
	
}

void UIList::setScrollBarPositionX(float value){ 
	this->scrollbar->getExternalState()->getActualState()->setPosition(vec2(value, 0.0f)); 
}

void UIList::setSelectedElementById(int position){
	if(position < this->items->size()){
		// Me guardo cual es el item seleccionado
		this->selectedElementId = position;

		// Deselecciono el anterior item, y selecciono el nuevo item
		if(this->selectedButton != NULL){
			this->selectedButton->setChecked(false);
		}
		list<UICheckBox*>::iterator it = this->buttonItems->begin();
		unsigned int i = 0;
		while(i < position && it != this->buttonItems->end()){
			++it;
			++i;
		}
		if(it != this->buttonItems->end()){
			this->selectedButton = (*it);
			this->selectedButton->setChecked(true);
		}
	}
}