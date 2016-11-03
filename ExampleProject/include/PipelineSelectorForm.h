#pragma once
#include "UIForm.h"
#include "UIStaticImage.h"
#include "UIButton.h"

enum PipelineSelectionIndex { p1=1, p2=2, p3=3, p4=4, p5=5, p6=6 };

class PipelineSelectorForm : public UIForm{

protected:
	//Maneja los eventos que se producen cuando un elemento consigue el foco
	void handleEvents();

private:
	//Incializacion de los estados del elemento 
	void initiateStates();
	//Inicializacion de los subelementos
	void initiateElements();
	//Linkea los elementos con los estados (cuales deben ser dibujados en cada estado)
	void bindElementsToStates();

	void onScreenChange();
	vec2 getFormSize();

	//Estados
	UIState* showedState;
	UIState* hidedState;
		
	//Agrego los subelementos
	UiButton* button1;
	UiButton* button2;
	UiButton* button3;
	UiButton* button4;
	UiButton* button5;
	UiButton* button6;

	// Se definen los manejadores de eventos
	void processButton1Event();
	void processButton2Event();
	void processButton3Event();
	void processButton4Event();
	void processButton5Event();
	void processButton6Event();

	// Indice de pipeline elegido
	PipelineSelectionIndex selectedPipeline;

public:
	PipelineSelectorForm(UIController* uiController);
	virtual ~PipelineSelectorForm();

	void setVisible(bool value);
	void onCallback(int callbackId);

	// Devuelve el indice de pipeline seleccionado actualmente
	PipelineSelectionIndex getSelectedPipeline(){ return this->selectedPipeline; }
};