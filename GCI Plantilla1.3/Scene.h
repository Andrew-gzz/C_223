#ifndef _Scene_
#define _Scene_

#include "openglclass.h"
#include "cameraclass.h"
#include "lightshaderclass.h"
#include "lightclass.h"
#include "Land.h"
#include "inputclass.h"
#include "KeyCode.h"
#include "VectorRR.h"
#include "Model.h"
#include "Triangle.h"
#include "colorshaderclass.h"
#include "ShaderBoundingBox.h"
#include "GameObject.h"
#include "Shader.h"
#include "TextureShader.h"
#include "TextureLoaderForOpenGL.h"
#include <string>

#include "Dome.h"
#include "ShaderSkydome.h"
#include "BoundingBoxRenderer.h"
#include "Billboard.h"
#include "ShaderBillboard.h"

// GLOBALS //
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;
const float SPEED_AXIS_Y = 0.05f;
const float SPEED_AXIS_X = 0.05f;
const float SPEEDT_AXIS_Z = 0.01f;
const float SPEEDT_AXIS_X = 0.01f;
const float SPEED_CAMERA = 0.01f;
const float InitialPosition[3] = { 0.0f, 11.0f, -10.0f };
const float InitialRotation[3] = { 0.0f, 0.0f, 0.0f };

using std::vector;
using namespace std;

class Scene {
private:
	//Estructuras
	struct informacionDColision
	{
		float Position[3];
		float Address[3];
		float Speed[3];
	};
	struct Vector {
		float X;
		float Y;
		float Z;
	};

	// Variables de movimiento y rotacion del personaje
	Vector* DeltaPosition = new Vector();
	Vector* DeltaRotation = new Vector();
	Vector* LastDeltaPosition = new Vector();

	//Dependencias Externas
	HWND handlerWindow = NULL;

	//Motor Grafico
	OpenGLClass* OpenGL;

	//Elementos Primarios
	CameraClass* Camera;
	Land* Terreno;
	informacionDColision* prevColision;
	TriangleClass* Triangulo;
	Dome* Skydome;

	//Shaders
	LightShaderClass* LightShader;
	ColorShaderClass* ColorShader;
	ShaderBoundingBox* ShaderBounding;
	ShaderTexture* ShaderModel;
	SkydomeShaderClass* ShaderSky;
	BillboardShaderClass* ShaderBill;

	//Lights
	LightClass* Light;

	//Input
	KeyBoard KeyCode;
	InputClass* input;
	float deltaTime;

	TextureClass* LoaderTexture;

	//Objetos
	GameObject* deLorean;
	GameObject* Bochido;
	GameObject* Municion;
	GameObject* Casa;
	GameObject* Pistola;
	GameObject* MedKit;
	GameObject* Fogata;
	GameObject* Estanteria;
	GameObject* Tienda;

	//CollisionBox
	BoundingBoxRenderer* box;
	BoundingBoxRenderer* box2;

	//Billboards
	Billboard* arbol2D;

	//Variables de Movimiento (Traslacion)
	float speedAxisX;
	float speedAxisY;
	float speedTAxisX;
	float speedTAxisZ;
	float speed;

	//Variables de Rotacion
	float angulo;
	float angulo_Y;
	float angulo_Z;
	float scale;
	bool Reset;

	//Variables de representacion (Color)
	float color[3];

	//Banderas de habilitacion
	bool RenderObject3d = true;
	bool RenderBilboard = true;

public:
	Scene(OpenGLClass* OpenGLObject, HWND hwnd);
	~Scene();

	bool Initialize();
	// Funcion cuya informacion de teclado o gamepad NO se le transfiere
	bool Render();
	bool Update(InputClass* input, float deltaTime);
	bool ManageCommands();
	bool Shutdown();

protected:


};

#endif //_Scene_