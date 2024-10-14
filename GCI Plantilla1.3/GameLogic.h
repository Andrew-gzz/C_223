#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

#include "openglclass.h"
#include "Scene.h"
#include "inputclass.h"
#include "VectorRR.h"

class GameLogic
{
private:
	struct informacionDColision
	{
		float Position[3];
		float Address[3];
		float Speed[3];
	};
	OpenGLClass* m_OpenGL;
	Scene* Escena1;

	bool Render();

public:
	GameLogic();
	GameLogic(OpenGLClass* OpenGL, HWND hwnd);
	GameLogic(const GameLogic&);
	~GameLogic();

	bool Initialize(OpenGLClass*, HWND);
	bool UpdateByFrame(InputClass* input, float deltaTime);
	void Shutdown();
};

#endif