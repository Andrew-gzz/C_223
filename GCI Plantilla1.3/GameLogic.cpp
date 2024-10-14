#include "GameLogic.h"

GameLogic::GameLogic()
{
	m_OpenGL = 0;
	Escena1 = 0;
}

GameLogic::GameLogic(OpenGLClass* OpenGL, HWND hwnd)
{
	m_OpenGL = 0;
	Escena1 = 0;
	Initialize(OpenGL, hwnd);
}


GameLogic::GameLogic(const GameLogic& other)
{
}

GameLogic::~GameLogic()
{
}

bool GameLogic::Initialize(OpenGLClass* OpenGL, HWND hwnd)
{
	bool result;
	string resultDetail = "";

	Escena1 = new Scene(OpenGL, hwnd);
	result = Escena1->Initialize();
	if (!result){
		result = false;
		_RPT1(0, "Alert! Scene Process was not initialized. \n", 0);
		return result;
	}

	return true;
}

bool GameLogic::Render()
{
	bool result;

	Escena1->Render();

	return true;
}

bool GameLogic::UpdateByFrame(InputClass* input,float deltaTime){
	bool result;
	string resultDetail = "";

	result = Escena1->Update(input, deltaTime);
	if (!result) {
		result = false;
		_RPT1(0, "Alert! Update Scene Process was crashed. \n", 0);
		return result;
	}
	result = Render();
	if(!result){
		result = false;
		_RPT1(0, "Alert! Render GameLogic Process was crashed. \n", 0);
		return result;
	}

	return true;
}

void GameLogic::Shutdown()
{
	// Liberamos el puntero de OpenGL.
	m_OpenGL = 0;

	// Para aquellos elementos que tengan un Shutdown como la escena
	// mandamos llamarla antes de igualarla a 0
	Escena1->Shutdown();
	Escena1 = 0;

	return;
}
