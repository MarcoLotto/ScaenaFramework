/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "UIForm.h"
#include "UIStaticImage.h"
#include "UIButton.h"
#include "Ui3dDrawable.h"
#include "TimeParameterInterpolator.h"
#include "Object.h"
#include "UIGraphicList.h"
#include "TeamBuilderDataTransfer.h"

class TeamBuilderForm : public UIForm{

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
	void clearResources();
	void saveBuilderData();

	void readBuilderConfigFile();
	void processObjectToShow(string objectFilename);
	void processTemplateList(vector<string> &templateIconsFilenames);
	void processColorLists(vector<vec3> &colors);
	void onTeamObjectDataChange();

	//Estados
	UIState* showedState;
	UIState* hidedState;
		
	//Agrego los subelementos
	UiLabel* titleLabel;
	Ui3dDrawable* drawer;
	UiButton* continueButton;

	UiButton* rotateLeftButton;
	UiButton* rotateRightButton;

	UIGraphicList* itemTemplatesGraphicList;
	vector<UIGraphicList*> colorGraphicLists;

	// Se definen los manejadores de eventos
	void processContinueButtonEvent();
	void processRotateRightButtonEvent();
	void processRotateLeftButtonEvent();

	TimeParameterInterpolator* rotationInterpolator;
	bool goingToTeamDetailsScreen;
	Object* objectToDraw;
	vector<int> lastSelectedColorIndexes;
	int lastTemplateIndex;
	TeamBuilderForm* nextBuilder;
	string builderConfigFilename;
	float manualRotationAngle;

	struct TeamTemplate{
		string templateFilename;
		vector<vec3> replacementColors;
	};
	vector<TeamTemplate> templates;	
	vector<vec3> colors;
	TeamBuilderDataTransfer dataTransfer;

public:
	TeamBuilderForm(UIController* uiController, string builderConfigFilename, TeamBuilderForm* nextBuilder);
	virtual ~TeamBuilderForm();

	virtual void update(UIState* externalState);

	void setVisible(bool visible);

	virtual void onCallback(int callbackId);

	// Usado para pasar datos de un builder a otro
	void setTeamBuilderDataTransfer(TeamBuilderDataTransfer dataTransfer){ this->dataTransfer = dataTransfer; }
};