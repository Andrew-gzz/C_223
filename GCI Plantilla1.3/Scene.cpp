#include "Scene.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <cmath>

using namespace std;

bool animation = false;

Scene::Scene(OpenGLClass* OpenGLObject, HWND hwnd) {
	handlerWindow = hwnd;
	OpenGL = OpenGLObject;
	Camera = 0;
	LightShader = 0;
	ColorShader = 0;
	ShaderBounding = 0;
	Triangulo = 0;
	Light = 0;
	SenIdalX = 0;
	SenIdalY = 0;
	SenIdalZ = 0;
	speedAxisX = 0.0f;
	speedAxisY = 0.0f;
	speedTAxisX = 0.0f;
	speedTAxisZ = 0.0f;
	speed = 0.0f;
	angulo = 0.0f;
	angulo_Y = 0.0f;
	RObjX = 0.0f;
	RObjY = 0.0f;
	RObjZ = 0.0f;
	deLorean = 0;
	noticias = 0;
	Wather = 0;
	Bote = 0;
	Almacen = 0;
	Elevador = 0;
	Puelta = 0;
	OXXO_in = 0;
	CASA_in = 0;
	Gloria = 0;
	Bate = 0;
	Katana = 0;
	Sarten = 0;
	Emi = 0;
	C = 0;
	Player1 = 0;
	Enemigo = 0;
	melee = 0;
	Guns = 0;
	LoaderTexture = new TextureClass(OpenGL);
}

Scene::~Scene() {

}

bool Scene::Initialize() {
	bool result = true;
	
	string resultDetail = "";

	Polacion = true;
	Pers = false;
	anguloB = 0.0f;
	angulo = 0.0f;
	angulo_Y = 0.0f;
	RObjX = 0.0f;
	RObjY = 0.0f;
	RObjZ = 0.0f;
	PosX = 0.0f;
	PosZ = 0.0f;
	speedAxisX = SPEED_AXIS_X;
	speedAxisY = SPEED_AXIS_Y;
	speedTAxisX = SPEEDT_AXIS_X;
	speedTAxisZ = SPEEDT_AXIS_Z;
	speed = SPEED_CAMERA;

	pos->X = InitialPosition[0]; pos->Y = InitialPosition[1]; pos->Z = InitialPosition[2];
	pos->X = 70.0f; pos->Z = 197.0f;
	// Crea un objeto camara.
	Camera = new CameraClass;
	if (!Camera) {
		result = false;
		_RPT1(0, "Alert! Camera has not been initialized. \n", 0);
		return result;
	}
	else {
		// Asigna las coordenadas de posicion y rotacion por defecto a los arreglos delta.
		DeltaPosition->X = InitialPosition[0]; DeltaPosition->Y = InitialPosition[1]; DeltaPosition->Z = InitialPosition[2];
		DeltaRotation->X = InitialRotation[0]; DeltaRotation->Y = InitialRotation[1]; DeltaRotation->Z = InitialRotation[2];
		// Luego se los pasamos a la camara.
		Camera->SetPosition(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z);
		Camera->SetRotation(DeltaRotation->X, DeltaRotation->Y, DeltaRotation->Z);
		// Con esto podriamos aislar siempre el punto de partida inicial de la escena para evitar teletrasportarse sin querer.
	}	

	niebla = new Fog2();
	if (!niebla) {
		result = false;
		MessageBoxA(handlerWindow, "Could not declared and initialized the light shader object.", "Error", MB_OK);
		_RPT1(0, "Alert! LightShader has an error on declare and not been initialized. \n", 0);
		return result;
	}

	Player1 = new Jugador(Camera->GetPositionX(), Camera->GetPositionY(), Camera->GetPositionZ(), 100, 10);
	if (!Player1) {
		result = false;
		MessageBoxA(handlerWindow, "Could not declared and initialized the light shader object.", "Error", MB_OK);
		_RPT1(0, "Alert! LightShader has an error on declare and not been initialized. \n", 0);
		return result;
	}

	Enemigo = new Jugador(pos->X, pos->Y, pos->Z, 100, 5);
	if (!Enemigo) {
		result = false;
		MessageBoxA(handlerWindow, "Could not declared and initialized the light shader object.", "Error", MB_OK);
		_RPT1(0, "Alert! LightShader has an error on declare and not been initialized. \n", 0);
		return result;
	}

	melee = new Armas("Melee");
	if (!melee) {
		result = false;
		MessageBoxA(handlerWindow, "Could not declared and initialized the light shader object.", "Error", MB_OK);
		_RPT1(0, "Alert! LightShader has an error on declare and not been initialized. \n", 0);
		return result;
	}

	Guns = new Armas("Pistola");
	if (!Guns) {
		result = false;
		MessageBoxA(handlerWindow, "Could not declared and initialized the light shader object.", "Error", MB_OK);
		_RPT1(0, "Alert! LightShader has an error on declare and not been initialized. \n", 0);
		return result;
	}

	LightShader = new LightShaderClass(OpenGL, handlerWindow, "shaders/terrain.vs", "shaders/terrain.ps");
	if (!LightShader) {
		result = false;
		MessageBoxA(handlerWindow, "Could not declared and initialized the light shader object.", "Error", MB_OK);
		_RPT1(0, "Alert! LightShader has an error on declare and not been initialized. \n", 0);
		return result;
	}

	ColorShader = new ColorShaderClass(OpenGL, handlerWindow, "shaders/color.vs", "shaders/color.ps");
	if (!ColorShader) {
		result = false;
		MessageBoxA(handlerWindow, "Could not declared and initialized the color shader object.", "Error", MB_OK);
		_RPT1(0, "Alert! ColorShader has an error on declare and not been initialized. \n", 0);
		return result;
	}

	//Terreno = new Land(handlerWindow, OpenGL, LoaderTexture, L"recursos/heightmap.jpg", "recursos/test.tga", "recursos/Piedras_normal.jpg",(float)400, (float)400, 0, 1);
	Terreno = new Land(handlerWindow, OpenGL, LoaderTexture, L"recursos/HMP.jpg", "recursos/texterr.jpg", "recursos/texterr2.jpg", "recursos/HMP.jpg", (float)1000, (float)1000);
	if (!Terreno){
		result = false;
		_RPT1(0, "Alert! Land has not been initialized. \n", 0);
		return result;
	}
	else {
		Terreno->SetLandShader(LightShader);
	}

	Triangulo = new TriangleClass(OpenGL);
	if (!Triangulo) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the triangle model object.", "Error", MB_OK);
		_RPT1(0, "Alert! Triangle has not been initialized. \n", 0);
		return result;
	}
	else {
		Triangulo->SetShaderTriangle(ColorShader);
	}

	ShaderModel = new ShaderTexture(OpenGL, handlerWindow, "shaders/light.vs", "shaders/light.ps");
	if (!ShaderModel) {
		result = false;
		MessageBoxA(handlerWindow, "Could not declared and initialized the shader model object.", "Error", MB_OK);
		_RPT1(0, "Alert! ShaderModel has an error on declare and not been initialized. \n", 0);
		return result;
	}

	ShaderBounding = new ShaderBoundingBox(OpenGL, handlerWindow, "shaders/vertexcolor.vs", "shaders/vertexcolor.ps");
	if (!ShaderBounding) {
		result = false;
		MessageBoxA(handlerWindow, "Could not declared and initialized the shader model object.", "Error", MB_OK);
		_RPT1(0, "Alert! ShaderModel has an error on declare and not been initialized. \n", 0);
		return result;
	}
	//Importación del modelo
	deLorean = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/Modelos/Carros(EnUso)/deLorean.obj",
		"recursos/Modelos/Carros(EnUso)/Color.png");
	Bochido = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/Modelos/Carros(EnUso)/Bochido.obj",
		"recursos/Modelos/Carros(EnUso)/Color.png");
	Municion = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/Modelos/Objetos(EnUso)/Ammo.obj",
		"recursos/Modelos/Objetos(EnUso)/Textures/Ammo.png");
	Casa = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/Modelos/Edificios(EnUso)/Casa_Blanca.obj",
		"recursos/Modelos/Edificios(EnUso)/Casa_Diffuse.png");
	Pistola = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/Modelos/Armas(EnUso)/Pistola.obj",
		"recursos/Modelos/Armas(EnUso)/Makarov_Base_Color.png");
	MedKit = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/Modelos/Medkit(EnUso)/Medkit.obj",
		"recursos/Modelos/Medkit(EnUso)/MedKit_Colors.png");
	Fogata = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/Modelos/Objetos(EnUso)/Fogata.obj",
		"recursos/Modelos/Objetos(EnUso)/Textures/Fogata.png");
	Estanteria = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/Modelos/Objetos(EnUso)/Estanteria.obj",
		"recursos/Modelos/Objetos(EnUso)/Estanterias_Diffuse.png");
	Tienda = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/Modelos/TiendayBote/Tienda.obj",
		"recursos/Modelos/TiendayBote/oxxo_color.png");
	Bote = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/Modelos/TiendayBote/Boat.obj",
		"recursos/Modelos/TiendayBote/boatColor.png");
	Almacen = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/Modelos/Edificios(EnUso)/Almacen.obj",
		"recursos/Modelos/Edificios(EnUso)/Almacen_color1.png");
	Elevador = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/Modelos/Edificios(EnUso)/Elevador.obj",
		"recursos/Modelos/Edificios(EnUso)/Elevador_Diffuse.png");

	if (!deLorean || !Bochido || !Casa || !Pistola || !MedKit || !Fogata || !Estanteria || !Tienda) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = deLorean->Initialize();
		result = Bochido->Initialize();
		result = Casa->Initialize();
		result = Pistola->Initialize();
		result = MedKit->Initialize();
		result = Fogata->Initialize();
		result = Estanteria->Initialize();
		

		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the model of Gameobject.", "Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		deLorean->SetShaders(ShaderModel, ShaderBounding);
		Bochido->SetShaders(ShaderModel, ShaderBounding);
		Casa->SetShaders(ShaderModel, ShaderBounding);
		Pistola->SetShaders(ShaderModel, ShaderBounding);
		MedKit->SetShaders(ShaderModel, ShaderBounding);
		Fogata->SetShaders(ShaderModel, ShaderBounding);
		Estanteria->SetShaders(ShaderModel, ShaderBounding);
		
	}

	if (!Municion) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "1Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = Municion->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the model of Gameobject.", "1Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		Municion->SetShaders(ShaderModel, ShaderBounding);
	}

	if (!Tienda) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "1Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = Tienda->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the model of Gameobject.", "1Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		Tienda->SetShaders(ShaderModel, ShaderBounding);
	}

	if (!Bote) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "1Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = Bote->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the model of Gameobject.", "1Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		Bote->SetShaders(ShaderModel, ShaderBounding);
	}
	if (!Almacen) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "1Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = Almacen->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the model of Gameobject.", "1Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		Almacen->SetShaders(ShaderModel, ShaderBounding);
	}
	if (!Elevador) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "1Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = Elevador->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the model of Gameobject.", "1Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		Elevador->SetShaders(ShaderModel, ShaderBounding);
	}

	Emi = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/Modelos/EasterEgg/EasterEgg.obj",
		"recursos/Modelos/EasterEgg/FotoFaceEmi.bmp");
	if (!Emi) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "1Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = Emi->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the model of Gameobject.", "1Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		Emi->SetShaders(ShaderModel, ShaderBounding);
	}

	Puelta = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/Modelos/Puerta/Puelta.obj",
		"recursos/Modelos/Puerta/Texture.png");
	if (!Puelta) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "1Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = Puelta->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the model of Gameobject.", "1Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		Puelta->SetShaders(ShaderModel, ShaderBounding);
	}

	Bate = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/Modelos/Bate/BatePoligon.obj",
		"recursos/Modelos/Bate/Wood048_2K-BMP_Color.bmp");
	if (!Bate) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "1Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = Bate->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the model of Gameobject.", "1Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		Bate->SetShaders(ShaderModel, ShaderBounding);
	}

	Sarten = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/Modelos/Sarten/Sarten.obj",
		"recursos/Modelos/Sarten/SartenDios.bmp");
	if (!Sarten) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "1Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = Sarten->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the model of Gameobject.", "1Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		Sarten->SetShaders(ShaderModel, ShaderBounding);
	}

	Katana = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/Modelos/catana/catana.obj",
		"recursos/Modelos/catana/ColorKatana.bmp");
	if (!Katana) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "1Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = Katana->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the model of Gameobject.", "1Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		Katana->SetShaders(ShaderModel, ShaderBounding);
	}

	Gloria = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/Modelos/Key/Gloria.obj",
		"recursos/Modelos/Key/Texture.png");
	if (!Gloria) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "1Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = Gloria->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the model of Gameobject.", "1Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		Gloria->SetShaders(ShaderModel, ShaderBounding);
	}

	OXXO_in = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/Modelos/Interior_OXXO/OXXO_int.obj",
		"recursos/Modelos/Interior_OXXO/oxxo_color.png");
	if (!OXXO_in) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "1Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = OXXO_in->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the model of Gameobject.", "1Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		OXXO_in->SetShaders(ShaderModel, ShaderBounding);
	}

	CASA_in = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/Modelos/Interior_CASA/Interior2.obj",
		"recursos/Modelos/Interior_CASA/Woods.png");
	if (!CASA_in) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "1Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = CASA_in->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the model of Gameobject.", "1Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		CASA_in->SetShaders(ShaderModel, ShaderBounding);
	}

	noticias = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/Modelos/News/Noticias.obj",
		"recursos/Modelos/News/Noticia.png");
	if (!noticias) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "1Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = noticias->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the model of Gameobject.", "1Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		noticias->SetShaders(ShaderModel, ShaderBounding);
	}

	Wather = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/Modelos/Agua/Aguas.obj",
		"recursos/Modelos/Agua/Agua_Texture.png");
	if (!Wather) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "1Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = Wather->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the model of Gameobject.", "1Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		Wather->SetShaders(ShaderModel, ShaderBounding);
	}
		

	C = new GameObject(OpenGL, handlerWindow, LoaderTexture,
		"recursos/Modelos/C-425/AlienCKF.obj",
		"recursos/Modelos/C-425/Texture.png");
	if (!C) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "1Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = C->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the model of Gameobject.", "1Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		C->SetShaders(ShaderModel, ShaderBounding);
	}


/*

	if (!Municion) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = Municion->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the model of Gameobject.", "Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}		
		Municion->AddTexture("recursos/Modelos/Objetos(EnUso)/Textures/Ammo.png");
		Municion->SetShaders(ShaderModel, ShaderBounding);
		
	}*/

	// Skydome
	ShaderSky = new SkydomeShaderClass(OpenGL, handlerWindow, "shaders/SkydomeShader.vs", "shaders/SkydomeShader.ps");
	if (!ShaderSky) {
		result = false;
		MessageBoxA(handlerWindow, "Could not declared and initialized the shader skydome.", "Error", MB_OK);
		_RPT1(0, "Alert! ShaderModel has an error on declare and not been initialized. \n", 0);
		return result;
	}

	Skydome = new Dome("recursos/Skydome/Skye.jpg", OpenGL, LoaderTexture, 800);
	if (!Skydome) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the Skydome.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = Skydome->Initialize();
		if (!result) {
			MessageBoxA(handlerWindow, "Could not initialize the Skydome.", "Error", MB_OK);
			_RPT1(0, "Alert! GameObject has an error on initialize. \n", 0);
			return result;
		}
		Skydome->SetIterpolationDay(false, true);
		Skydome->SetShader(ShaderSky);
	}

	// Caja de Colisiones
	// Caja de Colisiones Almacen
	box = new BoundingBoxRenderer(OpenGL,
		BoundingBox::GLFLOAT3{ 10.0f, 10.0f, 10.0f }, BoundingBox::GLFLOAT3{ -10.0f, -10.0f, -10.0f });
	box2 = new BoundingBoxRenderer(OpenGL,
		BoundingBox::GLFLOAT3{ 30.0f, 10.0f, 10.0f }, BoundingBox::GLFLOAT3{ 10.0f, -10.0f, -10.0f });
	box3 = new BoundingBoxRenderer(OpenGL,
		BoundingBox::GLFLOAT3{ 30.0f, 10.0f, 10.0f }, BoundingBox::GLFLOAT3{ 10.0f, -10.0f, -10.0f });
	box4 = new BoundingBoxRenderer(OpenGL,
		BoundingBox::GLFLOAT3{ 30.0f, 10.0f, 10.0f }, BoundingBox::GLFLOAT3{ 10.0f, -10.0f, -10.0f });

	if (!box|| !box2|| !box3 || !box4) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the box.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		box->SetShader(ShaderBounding);
		box2->SetShader(ShaderBounding);
		box3->SetShader(ShaderBounding);
		box4->SetShader(ShaderBounding);
	}		 
	// Caja de Colisones Elevador
	box5 = new BoundingBoxRenderer(OpenGL,
		BoundingBox::GLFLOAT3{ 1.0f, 1.0f, 1.0f }, BoundingBox::GLFLOAT3{ -1.0f, -1.0f, -1.0f });

	box6 = new BoundingBoxRenderer(OpenGL,
		BoundingBox::GLFLOAT3{ 1.0f, 1.0f, 1.0f }, BoundingBox::GLFLOAT3{ -1.0f, -1.0f, -1.0f });
	if (!box5 || !box6) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the box.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {

		box5->SetShader(ShaderBounding);
		box6->SetShader(ShaderBounding);
	}


	// Billboards
	ShaderBill = new BillboardShaderClass(OpenGL, handlerWindow, "shaders/billboard.vs", "shaders/billboard.ps");
	if (!ShaderBill) {
		result = false;
		MessageBoxA(handlerWindow, "Could not declared and initialized the shader billboard.", "Error", MB_OK);
		_RPT1(0, "Alert! ShaderModel has an error on declare and not been initialized. \n", 0);
		return result;
	}

	arbol2D = new Billboard(OpenGL, handlerWindow, LoaderTexture, "recursos/tree.png");
	if (!arbol2D) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the billboard.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		arbol2D->Initialize(5.0f);
		arbol2D->SetShader(ShaderBill);
	}

	arbol = new Billboard(OpenGL, handlerWindow, LoaderTexture, "recursos/tree2.png");
	if (!arbol) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the billboard.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		arbol->Initialize(5.0f);
		arbol->SetShader(ShaderBill);
	}

	hierba = new Billboard(OpenGL, handlerWindow, LoaderTexture, "recursos/hierba.png");
	if (!hierba) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the billboard.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		hierba->Initialize(1.0f);
		hierba->SetShader(ShaderBill);
	}

	Arbusto = new Billboard(OpenGL, handlerWindow, LoaderTexture, "recursos/Arbusto.png");
	if (!Arbusto) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the billboard.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		Arbusto->Initialize(1.0f);
		Arbusto->SetShader(ShaderBill);
	}

	/*PovDlorian = new Billboard(OpenGL, handlerWindow, LoaderTexture, "recursos/Imagenes/Pov_Dlorean.png");
	if (!PovDlorian) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the billboard.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		PovDlorian->Initialize(4.0f);
		PovDlorian->SetShader(ShaderBill);
	}*/

	Man = new Billboard(OpenGL, handlerWindow, LoaderTexture, "recursos/Sprites/Man_96x96.png");
	if (!Man) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the billboard.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		Man->Initialize2(1.0f, 0.0f, 0.5f); 
		Man->SetShader(ShaderBill);
	}

	return result;
}

bool Scene::Render() {
	bool result = true;

	// Limpia los buffers para iniciar la escena nueva.
	OpenGL->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Definimos nuestro espacio para las 3 matrices necesarias para renderizar.
	float worldMatrix[16];
	float viewMatrix[16];
	float projectionMatrix[16];

	// Actualiza la posicion y rotacion de la camara
	Camera->SetPosition(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z);
	Camera->SetRotation(DeltaRotation->X, DeltaRotation->Y, DeltaRotation->Z);

	// Genera la matriz de vista a partir de la posicion de la camara.
	Camera->Render();

	// Obtiene las matrices de vista, mundo y proyeccion de opengl y el objeto de la camara.
	OpenGL->GetWorldMatrix(worldMatrix);
	Camera->GetViewMatrix(viewMatrix);
	OpenGL->GetProjectionMatrix(projectionMatrix);

	// Renderizamos el Skydome
	Skydome->Render(viewMatrix, projectionMatrix);

	// Renderizamos terreno
	Terreno->Render(OpenGL, worldMatrix, viewMatrix, projectionMatrix);

	// Renderizamos Triangulo (ya se logra ver en escena)
	Triangulo->Render(viewMatrix, projectionMatrix);

	// Renderizamos nuestros objetos en la escena
	deLorean->Render(viewMatrix, projectionMatrix, true);
	Bochido->Render(viewMatrix, projectionMatrix, true);
	Municion->Render(viewMatrix, projectionMatrix, true);
	Casa->Render(viewMatrix, projectionMatrix, true);  
	Pistola->Render(viewMatrix, projectionMatrix, true);
	MedKit->Render(viewMatrix, projectionMatrix, true);
	Fogata->Render(viewMatrix, projectionMatrix, true);
	Estanteria->Render(viewMatrix, projectionMatrix, true);
	Tienda->Render(viewMatrix, projectionMatrix, true);
	Bote->Render(viewMatrix, projectionMatrix, true);
	Almacen->Render(viewMatrix, projectionMatrix, true);
	Elevador->Render(viewMatrix, projectionMatrix, true);
	noticias->Render(viewMatrix, projectionMatrix, true);
	Wather->Render(viewMatrix, projectionMatrix, true);
	OXXO_in->Render(viewMatrix, projectionMatrix, true);
	CASA_in->Render(viewMatrix, projectionMatrix, true);
	Gloria->Render(viewMatrix, projectionMatrix, true);
	Puelta->Render(viewMatrix, projectionMatrix, true);
	Bate->Render(viewMatrix, projectionMatrix, true);
	Sarten->Render(viewMatrix, projectionMatrix, true);
	Katana->Render(viewMatrix, projectionMatrix, true);
	Emi->Render(viewMatrix, projectionMatrix, true);
	C->Render(viewMatrix, projectionMatrix, true);
	// Renderizamos las cajas de colisión
	box->Draw(viewMatrix, projectionMatrix);
	box2->Draw(viewMatrix, projectionMatrix);
	box3->Draw(viewMatrix, projectionMatrix);
	box4->Draw(viewMatrix, projectionMatrix);
	box5->Draw(viewMatrix, projectionMatrix);
	box6->Draw(viewMatrix, projectionMatrix);
	// Renderizamos los billboards
	arbol2D->Render(viewMatrix, projectionMatrix, 
		0.0f, Terreno->Superficie(0.0f, 0.0f), 0.0f, 
		DeltaPosition->X, DeltaPosition->Z);

	Arbusto->Render(viewMatrix, projectionMatrix,
		0.1f, Terreno->Superficie(0.1f, -0.1f), -0.1f,
		DeltaPosition->X, DeltaPosition->Z);

	/*arbol2D->Render(viewMatrix, projectionMatrix, 
		-10.0f, Terreno->Superficie(-10.0f, -10.0f), -10.0f, 
		DeltaPosition->X, DeltaPosition->Z);

	arbol2D->Render(viewMatrix, projectionMatrix, 
		10.0f, Terreno->Superficie(10.0f, 10.0f), 10.0f, 
		DeltaPosition->X, DeltaPosition->Z);

	arbol2D->Render(viewMatrix, projectionMatrix, 
		-10.0f, Terreno->Superficie(-10.0f, 10.0f), 10.0f, 
		DeltaPosition->X, DeltaPosition->Z);

	arbol2D->Render(viewMatrix, projectionMatrix, 
		10.0f, Terreno->Superficie(10.0f, -10.0f), -10.0f, 
		DeltaPosition->X, DeltaPosition->Z);*/

	arbol->Render(viewMatrix, projectionMatrix,
		10.0f, Terreno->Superficie(10.0f, -10.0f), -10.0f,
		DeltaPosition->X, DeltaPosition->Z);

	hierba->Render(viewMatrix, projectionMatrix,
		2.0f, Terreno->Superficie(2.0f, -2.0f), -2.0f,
		DeltaPosition->X, DeltaPosition->Z);

	Arbusto->Render(viewMatrix, projectionMatrix,
		10.0f, Terreno->Superficie(10.0f, -10.0f), -10.0f,
		DeltaPosition->X, DeltaPosition->Z);

	//////////////////////////////////////////////////////

	/*PovDlorian->Render(viewMatrix, projectionMatrix,
		10.0f, Terreno->Superficie(20.0f, -20.0f), -10.0f,
		DeltaPosition->X, DeltaPosition->Z);*/	

	//////////////////////////////////////////////////////

	Man->Render(viewMatrix, projectionMatrix, -81.0f, Terreno->Superficie(-81.0f, 60.0f), 60.0f, DeltaPosition->X, DeltaPosition->Z);

	// no jala esta niebla ni la primera
	niebla->setColor(0.5f, 0.5f, 0.5f);
	niebla->setLinearRange(5.0f, 50.0f);
	niebla->setDensity(0.22f);
	niebla->setEquation(0);
	niebla->getFogFactorWithProjection(viewMatrix, projectionMatrix, worldMatrix);
	niebla->enableFog(GL_TRUE);

	niebla->applyFog();

	// skibiditoilet sigma popni chamba

	// Damos la instruccion de que termino la escena para que nos muestre frame a frame.
	OpenGL->EndScene();

	return result;
}

bool Scene::Update(InputClass* input, float deltaTime) {
	GAMEPAD_0;

	bool result = true;

	this->input = input;
	this->deltaTime = deltaTime;
	result = ManageCommands();
	if (!result) {
		result = false;
		_RPT1(0, "Alert! Error on key. \n", 0);
		return result;
	}

	/*float* matrixSkydome = Skydome->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixSkydome, 0.0f, 0.0f, 0.0f);*/
	Player1->setPosX(Camera->GetPositionX());
	Player1->setPosY(Camera->GetPositionY());
	Player1->setPosZ(Camera->GetPositionZ());

	float* matrixTriangle = Triangulo->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixTriangle, -20.0f, 20.0f, 0.0f);

	float* matrixGameObject = deLorean->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixGameObject, 79.0f, 28.0f, 214.0f);
	///////////////////////////////////////////////////////////////////

	//OpenGL->MatrixObjectRotationY(matrixGameObject, RObjY); // este si jala, solo lo tengo para provar cosas.
	//OpenGL->MatrixObjRotationMultiple(matrixGameObject, 0.0f, RObjY, 0.0f); // esto noe sta funcionando, despues lo arreglamos.

	/////////////////////////////////////////////////////
	
	float* matrixGameObject1 = Bochido->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixGameObject1, -41.0f, 21.0f, 9.0f);

	float* matrixGameObject2 = Municion->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixGameObject2, -40.0f, 10.0f, -10.0f);
	OpenGL->MatrixScale(matrixGameObject2, 2.0f, 2.0f, 2.0f);

	float* matrixGameObject3 = Casa->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixGameObject3, -43.0f, Terreno->Superficie(-43,95), 95.0f);

	float* matrixCASAint = CASA_in->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixCASAint, -47.0f, 10.0f, 101.0f);
	OpenGL->MatrixObjectRotationY(matrixCASAint, 11.0);

	float* matrixGameObject4 = Pistola->GetWorldMatrix(); // Pistolita
	OpenGL->MatrixTranslation(matrixGameObject4, -20.0f, 10.0f, -15.0f);
	OpenGL->MatrixObjectScale(matrixGameObject4, 8, 8, 8);

	float* matrixGameObject5 = MedKit->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixGameObject5, -20.0f, 10.0f, -17.0f);

	float* matrixGameObject6 = Fogata->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixGameObject6, -22.0f, 20.0f, -17.0f);

	float* matrixGameObject7 = Estanteria->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixGameObject7, -24.0f, 10.0f, -17.0f);

	float* matrixGameObject8 = Tienda->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixGameObject8, -40.0f, 20.0f, 20.0f);

	float* matrixBote = Bote->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixBote, -125.0f, 2.0f, 115.0f);
	OpenGL->MatrixObjectScale(matrixBote, 10.0f, 10.0f, 10.0f);

	float* matrixGameObject9 = Almacen->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixGameObject9, 79.0f, Terreno->Superficie(79.0f, 214.0f), 214.0f);

	float* matrixGameObject10 = Elevador->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixGameObject10, 79.0f, Terreno->Superficie(79.0f, 214.0f), 214.0f);

	float* matrixNoticias = noticias->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixNoticias, -75.0f, 19.8f, 62.0f);
	OpenGL->MatrixObjectRotationY(matrixNoticias, 70.0f);

	float* matrixAgua = Wather->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixAgua, -125.0f, -1.0f, -65.0f);
	OpenGL->MatrixObjectScale(matrixAgua, 150.0f, 0.0f, 150.0f);

	float* matrixKey = Gloria->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixKey, -45.0f, 10.0f, 100.0f);
	OpenGL->MatrixObjectScale(matrixKey, 0.5f, 0.8f, 0.5f);

	float* matrixPuelta = Puelta->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixPuelta, -34.8f, 20.0f, 15.9f);
	OpenGL->MatrixObjectScale(matrixPuelta, 1.0f, 0.7f, 1.0f);

	float* matrixOXXOint = OXXO_in->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixOXXOint, -35.8f, 10.0f, 15.0f);

	float* matrixBate = Bate->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixBate, -41.8f, 10.0f, 25.0f);
	OpenGL->MatrixObjectScale(matrixBate, 0.6f, 0.6f, 0.6f);
	
	float* matrixSarten = Sarten->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixSarten, -40.8f, 10.0f, 20.0f);
	OpenGL->MatrixObjectScale(matrixSarten, 0.5f, 0.5f, 0.5f);

	float* matrixKatana = Katana->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixKatana, -40.8f, 10.0f, 15.0f);
	OpenGL->MatrixObjectScale(matrixKatana, 0.5f, 0.5f, 0.5f);

	float* matrixEmi = Emi->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixEmi, -118.0f , -2.0f, 112.0f);
	OpenGL->MatrixObjectRotationY(matrixEmi, (DeltaRotation->Y / 180) * 3.14159265358979323846); // gira exactamente asia donde esta viendo el jugador
	//////////////////////////////////////////////////////////////
	float* matrixC425 = C->GetWorldMatrix(); 
	OpenGL->MatrixTranslation(matrixC425, pos->X, pos->Y, pos->Z);
	OpenGL->MatrixObjectScale(matrixC425, 0.3f, 0.3f, 0.3f);
	/////////////////////////////////////////////////////////////

	//Tranformaciones de cajas de colisión
	//Almacen colisiones
	float* matrixBox = box->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixBox, 63.8f, 19, 214);//Pared izq x,y,z
	OpenGL->MatrixObjectScale(matrixBox, 0.01f,1.0f,1.0f);
	
	float* matrixBox2 = box2->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixBox2, 93.8f, 19, 214);//Pared izq x,y,z
	OpenGL->MatrixObjectScale(matrixBox2, 0.01f, 1.0f, 1.0f);

	float* matrixBox3 = box3->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixBox3, 50.8f, 19, 224.2);//Pared izq x,y,z
	OpenGL->MatrixObjectScale(matrixBox3, 1.3f, 1.0f, 0.01f);

	float* matrixBox4 = box4->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixBox4, 72.8f, 19, 204.0f);//Pared izq x,y,z
	OpenGL->MatrixObjectScale(matrixBox4, 0.7f, 1.0f, 0.01f);
	//Elevador
	float* matrixBox5 = box5->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixBox5, 90.8f, 19, 222.0f);//Pared izq x,y,z
	OpenGL->MatrixObjectScale(matrixBox5, 2.8f, 2.8f, 2.8f);

	float* matrixBox6 = box6->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixBox6, 79.0f, 20.0f, 214.0f);//
	OpenGL->MatrixObjectScale(matrixBox6, 2.8f, 2.8f, 2.8f);

	/*float* auxMatrix = new float[16] { 0.0f };
	OpenGL->BuildIdentityMatrix(auxMatrix);

	OpenGL->MatrixScale(auxMatrix, 0.07f, 1.0f, 1.0f);
	OpenGL->MatrixMultiply(matrixBox, matrixBox, auxMatrix);

	float* matrixBox2 = box2->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixBox2, -40.0f, Terreno->Superficie(-40.0f, 40.0f), 40.0f);

	OpenGL->MatrixScale(auxMatrix, 1.0f, 1.0f, 0.07f);
	OpenGL->MatrixMultiply(matrixBox2, matrixBox2, auxMatrix);

	//Colisión por esfera
	if (deLorean->GetSphericalCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z, 2)) {
		MessageBox(handlerWindow, L"Colisionando", L"Aviso", MB_OK);
	}*/

	// banderas
	static bool Kat = false, Bat = false, Sar = false;
	static bool Tope = false;
	static bool Key = false;
	static bool Mons = false;
	static bool Dlor = false;
	static bool Boch = false;
	static bool UseElevador = false;
	static bool oxxo = false;
	static bool almacen = false;
	static int elevador = 0;
	static bool casa = false;
	static bool AGUAS = false;
	static int interaccioens = 0;

	//Colisión por caja

	// este if hace que el modelo este en la posicion de la camara en todo el mapa picando "E"
	if (Dlor == true) {
		static float DlorianAltura = 22.5f;

		// me bajo del carro
		if (input->GetKey(KeyCode.Q) || input->GetKeyXbox(KeyCode.XBOX_B)) {
			Dlor = false;
			DlorianAltura = 0.0f;
		}
		// aumento la altura ( volando )
		if (input->GetKey(KeyCode.Space) || input->GetKeyXbox(KeyCode.LT)) {
			DlorianAltura += 0.1f;
		}
		// deciendo
		if (input->GetKey(KeyCode.Enter) || input->GetKeyXbox(KeyCode.RT)) {
			DlorianAltura -= 0.1f;
		}

		DeltaPosition->Y += DlorianAltura; // agregamos altura

		OpenGL->MatrixTranslation(matrixGameObject, DeltaPosition->X, DeltaPosition->Y - 1.3f, DeltaPosition->Z); // acomodando modelo
		OpenGL->MatrixObjectRotationY(matrixGameObject, (angulo_Y * 0.0174532925f) - 92.67); // (modelo, (angulo * radianes) - grados)

	}

	if (Bochido->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		// desactiva la restriccon de movimiento del HitBox
		if (Boch == false) {
			DeltaPosition->X = LastDeltaPosition->X;
			DeltaPosition->Y = LastDeltaPosition->Y;
			DeltaPosition->Z = LastDeltaPosition->Z;
		}
		if (input->GetKey(KeyCode.E) || input->GetKeyXbox(KeyCode.XBOX_A)) {
			Boch = true;
		}
	}
	if (Boch == true) {
		// me bajo del carro
		if (input->GetKey(KeyCode.Q) || input->GetKeyXbox(KeyCode.XBOX_B)) {
			Boch = false;
		}
		OpenGL->MatrixTranslation(matrixGameObject1, DeltaPosition->X, DeltaPosition->Y -0.98f, DeltaPosition->Z); // acomodando modelo
		OpenGL->MatrixObjectRotationY(matrixGameObject1, (angulo_Y * 0.0174532925f) - 92.67); // (modelo, (angulo * radianes) - grados)
	}
	static int NUM = 0;

	if (NUM >= 6) {
		Tope = true;
	}
	if (Municion->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {

		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
		if (Tope == true) {
			MessageBoxA(handlerWindow, "Inventario Lleno", "No hay + espacio", MB_OK);
		}
		else if (input->GetKey(KeyCode.E) || input->GetKeyXbox(KeyCode.XBOX_A)) { // agarra la municion 
			Player1->setObjInventario("Municion", NUM);
			Guns->setCargas(Guns->getCargas() + 1);
			string comen = " Guardaste la " + Player1->getInventario().getObjeto(NUM).getNombreObj() + " en la posicion " + to_string(NUM) +
				"ahora tienes " + to_string(Guns->getCargas()) + " Cargas.";
			MessageBoxA(NULL, comen.c_str(), "INVENTARIO", MB_OK);
			NUM++;
		}
	}
	if(casa == false){

		Polacion = true;
		if (Casa->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
			if (casa == false) {
				DeltaPosition->X = LastDeltaPosition->X;
				DeltaPosition->Y = LastDeltaPosition->Y;
				DeltaPosition->Z = LastDeltaPosition->Z;
			}
			if (input->GetKey(KeyCode.E) || input->GetKeyXbox(KeyCode.XBOX_A)) { // entrar a la casa
				casa = true;
			}
		}
	}	
	if (casa == true) {
		Polacion = false;
		OpenGL->setMatrixPosY(matrixGameObject3, 10.0f);// casa
		OpenGL->setMatrixPosY(matrixCASAint, 20.0f);// interior
		OpenGL->setMatrixPosX(matrixNoticias, -45.0f);// noticias
		OpenGL->setMatrixPosY(matrixKey, 20.0f);// llave
		OpenGL->setMatrixPosZ(matrixNoticias, 99.0f);
	}
	if (Pistola->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
		if (input->GetKey(KeyCode.E) || input->GetKeyXbox(KeyCode.XBOX_A)) { // agarra el arma
			Guns->setArm("Pistola");
			Guns->setMunicion(15);
			Player1->setObjInvt(Guns->getArma(), NUM);	
			string comen = " Guardaste la " + Player1->getInventario().getObjeto(NUM).getNombreObj() + " en la posicion " + to_string(NUM);
			MessageBoxA(NULL, comen.c_str(), "INVENTARIO", MB_OK);
			NUM++; 
		}
	}
	if (Gloria->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
		if (input->GetKey(KeyCode.E) || input->GetKeyXbox(KeyCode.XBOX_A)) { // agarra la llave
			Key = true;
			string comen = "QUE GRAN NOTICIA!! desbloqueaste la lluvia y ahora el pozo se ah llenado de agua, abra algo debajo de ahi?";
			MessageBoxA(NULL, comen.c_str(), "Agarraste la llave", MB_OK);
		}		
	}
	if (Key == true) {
		OpenGL->setMatrixPosY(matrixKey, 10.0f);
		OpenGL->setMatrixPosY(matrixAgua, 18.0f);
		OpenGL->setMatrixPosY(matrixBote, 17.4f + SenIdalY);// bote
		OpenGL->setMatrixPosY(matrixEmi, 2.0f); // EasterEgg
		// no se mueve tan chido como pensaba
		//OpenGL->setMatrixPosX(matrixBote, OpenGL->getMatrixPosX(matrixBote) + SenIdalX);
		//OpenGL->setMatrixPosZ(matrixBote, OpenGL->getMatrixPosZ(matrixBote) + SenIdalZ);
	}
	if (MedKit->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
		if (input->GetKey(KeyCode.E) || input->GetKeyXbox(KeyCode.XBOX_A)) { // agarra el kit
			if (Player1->getVida() == 100) {
				string comen = " Vida: " + Player1->getVida();
				MessageBoxA(NULL, comen.c_str(), "INVENTARIO", MB_OK);
			}
			else {
				Player1->setVida(Player1->getVida() + 25);
				string comen = " Vida: " + Player1->getVida();
				MessageBoxA(NULL, comen.c_str(), "INVENTARIO", MB_OK);
			}
		}
	}
	if (Bote->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
		if (input->GetKey(KeyCode.E) || input->GetKeyXbox(KeyCode.XBOX_A)) { // pensamiento
			string comen = "Simplemente es un Bote...";
			MessageBoxA(NULL, comen.c_str(), "(Pensamiento)", MB_OK);
		}
	}
	if (Emi->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
		if (input->GetKey(KeyCode.E) || input->GetKeyXbox(KeyCode.XBOX_A)) { // pensamiento
			if (interaccioens == 0) {
				string comen = "Hola, soy Emi, Soy el ayudante de Cualquier Juego, siempre aparesco en los Juegos donde el supuesto Programador Mauricio esta involucrado.";
				MessageBoxA(NULL, comen.c_str(), "Emi El Ayudante", MB_OK);
				interaccioens++;
			}
			if (interaccioens >= 1) {
				string comen = "quieres que te ayude?";
				int resp = MessageBoxA(NULL, comen.c_str(), "Emi El Ayudante", MB_YESNO);
				if (resp == IDYES) {
					if (interaccioens == 1) {
						string comen = "bueno, El delorian se encuentra en el almacen, puedes encontrarlo en el piso Nro 4, puede que esten muchos enemigos por ahi.";
						MessageBoxA(NULL, comen.c_str(), "Emi El Ayudante", MB_OK);
						interaccioens++;
					}
					else {
						string comen = "ya te dije donde se encuentra, no hay problemas.";
						MessageBoxA(NULL, comen.c_str(), "Emi El Ayudante", MB_OK);
					}
				}
				else {
					string comen = "mmmm, esta bien.";
					MessageBoxA(NULL, comen.c_str(), "Emi El Ayudante", MB_OK);
				}
			}
		}
	}
	if (Fogata->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
		if (input->GetKey(KeyCode.E)) { // sepa que quieras hacer aqui xd
			
		}
	}
	if (Estanteria->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
	}
	if (Tienda->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		if (oxxo == false) {
			Polacion = true;
			DeltaPosition->X = LastDeltaPosition->X;
			DeltaPosition->Y = LastDeltaPosition->Y;
			DeltaPosition->Z = LastDeltaPosition->Z;
		}	
		if (input->GetKey(KeyCode.E) || input->GetKeyXbox(KeyCode.XBOX_A)) { // entrar a la tienda
			oxxo = true;
		}
		if (oxxo == true) {	
			Polacion = false;
			OpenGL->setMatrixPosY(matrixOXXOint, 20.0f);// interior
			OpenGL->setMatrixPosY(matrixGameObject8, 10.0f);// tienda
			OpenGL->setMatrixPosX(matrixGameObject7, -44.0f);// estanteria	
			OpenGL->setMatrixPosY(matrixGameObject7, 20.0f);
			OpenGL->setMatrixPosZ(matrixGameObject7, 24.0f);
			OpenGL->setMatrixPosX(matrixGameObject4, -44.0f);// Pistola	
			OpenGL->setMatrixPosY(matrixGameObject4, 21.0f);
			OpenGL->setMatrixPosZ(matrixGameObject4, 20.0f);
			OpenGL->setMatrixPosX(matrixGameObject5, -44.0f);// kit
			OpenGL->setMatrixPosY(matrixGameObject5, 20.0f);
			OpenGL->setMatrixPosZ(matrixGameObject5, 18.0f);
			OpenGL->setMatrixPosX(matrixGameObject2, -44.0f);// Munision
			OpenGL->setMatrixPosY(matrixGameObject2, 21.0f);
			OpenGL->setMatrixPosZ(matrixGameObject2, 15.0f);
			OpenGL->setMatrixPosX(matrixNoticias, -38.0f);// noticias
			OpenGL->setMatrixPosZ(matrixNoticias, 24.0f);
			OpenGL->MatrixObjectRotationY(matrixNoticias, 1.0f);
			if (Bat == false) {
				OpenGL->setMatrixPosY(matrixBate, 21.1f); // Bate
			}
			OpenGL->MatrixObjectRotationY(matrixBate, RObjY);
			if (Sar == false) {
				OpenGL->setMatrixPosY(matrixSarten, 21.1f); // Sarten
			}
			OpenGL->MatrixObjectRotationY(matrixSarten, RObjY);
			if (Kat == false) {
				OpenGL->setMatrixPosY(matrixKatana, 21.1f); // Katana
			}
			OpenGL->MatrixObjectRotationY(matrixKatana, RObjY);
			OpenGL->setMatrixPosX(matrixPuelta, -33.8f);// puelta			
		}
	}
	if (noticias->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z))
	{
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
		if (input->GetKey(KeyCode.E)) { // lee noticias
			// aqui se intento hacer que suene un audio, pero no se logro por que no se puede llamar el 
			// objeto al main, y fue un pedo hacer qeu funcione junto con la musica de fondo
			if(casa == true){
				if (Key == true) {
					string comen = "Noticia: Soy las noticias, solamente doy pistas, pensabas que te iba decir todo?, Tengo a Peñanieto en mi estante!!";
					MessageBoxA(NULL, comen.c_str(), "NOTICIA", MB_OK);
				}
				else {
					string comen = "Noticia: En esta casa se encuentra la llave de la gloria";
					MessageBoxA(NULL, comen.c_str(), "NOTICIA", MB_OK);
				}
			}
			else if (oxxo == true) {
				string comen = "Noticia: OXXO esta vendiendo armas, ES UNA LOCURA!!, y todo es GRATIS!!";
				MessageBoxA(NULL, comen.c_str(), "NOTICIA", MB_OK);
			}
			else {
				if (Key == true) {
					string comen = "Noticia: misteriosamente a regresado el agua, pero inundo todo el mapa, MILAGRO!!";
					MessageBoxA(NULL, comen.c_str(), "NOTICIA", MB_OK);
				}
				else {
					string comen = "Noticia: Se acabo toda el agua del pozo, alguien podra hacer que se llene de agua?";
					MessageBoxA(NULL, comen.c_str(), "NOTICIA", MB_OK);
				}
			}
		}
	}

	if (Bate->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
		if (Tope == true) {
			MessageBoxA(handlerWindow, "Inventario Lleno", "No hay + espacio", MB_OK);
		}
		else if (input->GetKey(KeyCode.E) || input->GetKeyXbox(KeyCode.XBOX_A)) { // Bate
			Bat = true;
			Player1->setObjInventario("Bate", NUM);
			string comen = " Guardaste el " + Player1->getInventario().getObjeto(NUM).getNombreObj() + " en la posicion " + to_string(NUM);
			MessageBoxA(NULL, comen.c_str(), "INVENTARIO", MB_OK);
			NUM++;
		}
	}
	if (Sarten->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
		if (Tope == true) {
			MessageBoxA(handlerWindow, "Inventario Lleno", "No hay + espacio", MB_OK);
		}
		else if (input->GetKey(KeyCode.E) || input->GetKeyXbox(KeyCode.XBOX_A)) { // Sarten
			Sar = true;
			Player1->setObjInventario("Sarten", NUM);
			string comen = " Guardaste el " + Player1->getInventario().getObjeto(NUM).getNombreObj() + " en la posicion " + to_string(NUM);
			MessageBoxA(NULL, comen.c_str(), "INVENTARIO", MB_OK);
			NUM++;
		}
	}
	if (Katana->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
		if (Tope == true) {
			MessageBoxA(handlerWindow, "Inventario Lleno", "No hay + espacio", MB_OK);
		}
		else if (input->GetKey(KeyCode.E) || input->GetKeyXbox(KeyCode.XBOX_A)) { // Katana
			Kat = true;
			Player1->setObjInventario("Katana", NUM);
			string comen = " Guardaste la " + Player1->getInventario().getObjeto(NUM).getNombreObj() + " en la posicion " + to_string(NUM);
			MessageBoxA(NULL, comen.c_str(), "INVENTARIO", MB_OK);
			NUM++;
		}
	}

	// PETATEADO
	if (Player1->getVida() <= 0.0f) {
		string comen = " Moriste, vida: " + to_string(Player1->getVida());
		MessageBoxA(NULL, comen.c_str(), "Game Over", MB_OK);
	}
	//

	static string OB;
	static bool Mantener = false;

	if(Mantener == true){
		if (OB == "Katana") {
			Player1->setDamage(80);
			OpenGL->setMatrixPosY(matrixKatana, (DeltaPosition->Y - 1) );
			OpenGL->setMatrixPosX(matrixKatana, DeltaPosition->X - 1.0f);
			OpenGL->setMatrixPosZ(matrixKatana, DeltaPosition->Z);
		}
		if (OB == "Bate") {
			Player1->setDamage(40);
			OpenGL->setMatrixPosY(matrixBate, DeltaPosition->Y - 1);
			OpenGL->setMatrixPosX(matrixBate, DeltaPosition->X - 1.0f);
			OpenGL->setMatrixPosZ(matrixBate, DeltaPosition->Z);
		}
		if (OB == "Sarten") {
			Player1->setDamage(50);
			OpenGL->setMatrixPosY(matrixSarten, DeltaPosition->Y - 1);
			OpenGL->setMatrixPosX(matrixSarten, DeltaPosition->X - 1.0f);
			OpenGL->setMatrixPosZ(matrixSarten, DeltaPosition->Z);
		}
		if (OB == "Pistola") {
			Player1->setDamage(60);
			OpenGL->setMatrixPosY(matrixGameObject4, DeltaPosition->Y - 1);
			OpenGL->setMatrixPosX(matrixGameObject4, DeltaPosition->X - 1.0f); 
			OpenGL->setMatrixPosZ(matrixGameObject4, DeltaPosition->Z); 
		}
	}
	if (input->GetKey(KeyCode.U) || input->GetKeyXbox(KeyCode.XBOX_X)) {
		Player1->getDaño();
	}

	if (input->GetKey(KeyCode.Zero)) {
		OB = "Manos";
	}
	if (input->GetKey(KeyCode.One)) {
		OB = "";
		OB = Player1->getInventario().getObjeto(0).getNombreObj();
		Mantener = true;
	}
	if (input->GetKey(KeyCode.Two)) {
		OB = "";
		OB = Player1->getInventario().getObjeto(1).getNombreObj();
		Mantener = true;
	}
	if (input->GetKey(KeyCode.Three)) {
		OB = "";
		OB = Player1->getInventario().getObjeto(2).getNombreObj();
		Mantener = true;
	}
	if (input->GetKey(KeyCode.Four)) {
		OB = "";
		OB = Player1->getInventario().getObjeto(3).getNombreObj();
		Mantener = true;
	}
	if (input->GetKey(KeyCode.Five)) {
		OB = "";
		OB = Player1->getInventario().getObjeto(4).getNombreObj();
		Mantener = true;
	}
	if (input->GetKey(KeyCode.Six)) {
		OB = "";
		OB = Player1->getInventario().getObjeto(5).getNombreObj();
		Mantener = true;
	}
	
	//No se que es pero Colision
	if (box->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
	}	
	if (box2->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
	}
	if (box3->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
	}
	if (box4->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
	}
	if (box5->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
		if (input->GetKey(KeyCode.E)) {
			altura = 30.0f;
			UseElevador = true;
			MessageBoxA(handlerWindow, "Subiendo", "Elevador dice", MB_OK);
		}
	}
	if (altura != 19.0f && Dlor != true) {
		DeltaPosition->Y = altura;
		Player1->setPosY(altura);
	}
	if (box6->GetBoxCollision(DeltaPosition->X, 19.0f, DeltaPosition->Z) && UseElevador == true) {
		if (Dlor == false) {
			DeltaPosition->X = LastDeltaPosition->X;
			DeltaPosition->Y = LastDeltaPosition->Y;
			DeltaPosition->Z = LastDeltaPosition->Z;
		}
		if (input->GetKey(KeyCode.E)) {
			Dlor = true;
		}
		if (input->GetKeyXbox(KeyCode.XBOX_A)) {
			Dlor = true;
		}
	}

	// aplicacion del movimiento senoidal del agua

	OpenGL->setMatrixPosX(matrixAgua, SenIdalX);
	//OpenGL->setMatrixPosY(matrixAgua, SenIdalY);
	OpenGL->setMatrixPosZ(matrixAgua, SenIdalZ);
	
	// Persiguiendo
	if (Pers == true) {
		OpenGL->MatrixTranslation(matrixC425, pos->X, pos->Y, pos->Z);
	}
	
	// LIMITES DEL ALMACEN
	

	// LIMITES DE LA CASA
	if (casa == true) {
		if (DeltaPosition->X < -48.0f || DeltaPosition->X > -29.0f) { // COLISION SIN LINEAS EN X
			DeltaPosition->X = LastDeltaPosition->X;
			DeltaPosition->Y = LastDeltaPosition->Y;
			DeltaPosition->Z = LastDeltaPosition->Z;

		}
		if (DeltaPosition->Z < 86.0f || DeltaPosition->Z > 101.0f) { // COLISON SIN LINEAS EN Z
			DeltaPosition->X = LastDeltaPosition->X;
			DeltaPosition->Y = LastDeltaPosition->Y;
			DeltaPosition->Z = LastDeltaPosition->Z;
			if (input->GetKey(KeyCode.Q)) { // salir de la tienda
				casa = false;
			}
			if (input->GetKeyXbox(KeyCode.XBOX_B)) {
				casa = false;
			}
		}
		//	P1	|	P2
		//	X1	|	X2
		//	Z1	|	Z2
	}

	// LIMITES DE LA TIENDA
	if (oxxo == true) {
		if (DeltaPosition->X > -34.2f || DeltaPosition->X < -45.0f) { // COLISION SIN LINEAS EN X
			DeltaPosition->X = LastDeltaPosition->X;
			DeltaPosition->Y = LastDeltaPosition->Y;
			DeltaPosition->Z = LastDeltaPosition->Z;
			if (input->GetKey(KeyCode.Q)) { // salir de la tienda
				oxxo = false;
			}
			if (input->GetKeyXbox(KeyCode.XBOX_B)) {
				oxxo = false;
			}

		}
		if (DeltaPosition->Z > 26.0f || DeltaPosition->Z < 14.0f) { // COLISON SIN LINEAS EN Z
			DeltaPosition->X = LastDeltaPosition->X;
			DeltaPosition->Y = LastDeltaPosition->Y;
			DeltaPosition->Z = LastDeltaPosition->Z;
		}
		//	P1	|	P2
		//	X1	|	X2
		//	Z1	|	Z2
	}

	// LIMITANTES DEL MUNDO
	if (DeltaPosition->X < -480.f || DeltaPosition->X > 480.f) { // COLISION SIN LINEAS EN X
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
	}
	if (DeltaPosition->Z < -480.f || DeltaPosition->Z > 480.f) { // COLISON SIN LINEAS EN Z
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
	}

	if (input->GetKey(KeyCode.Shift) == true) {
		speed += speed * .03;
	}
	else{ speed = SPEED_CAMERA; }

	
	return result;
}

bool Scene::ManageCommands() {
	GAMEPAD_0;

	bool result = true;
	
	//Guardar la posición de la Cámara antes de actualizarla
	LastDeltaPosition->X = DeltaPosition->X;
	LastDeltaPosition->Y = DeltaPosition->Y;
	LastDeltaPosition->Z = DeltaPosition->Z;

	float vr[2]{ 0,0 };
	float vrTXZ[2]{ 0,0 };
	float vrT90XZ[2]{ 0,0 };
	float radians = DeltaRotation->Y * 0.0174532925f;

	if((input->GetKey(KeyCode.W) || input->GetKey(KeyCode.Up))) {
		vr[0] += sinf(radians)*speed * deltaTime;
		vr[1] += cosf(radians) * speed * deltaTime;
	}
	if ((input->GetKey(KeyCode.S) || input->GetKey(KeyCode.Down))) {
		vr[0] -= sinf(radians) * speed * deltaTime;
		vr[1] -= cosf(radians) * speed * deltaTime;
	}
	if ((input->GetKey(KeyCode.A) || input->GetKey(KeyCode.Left))) {
		vr[0] -= sinf(radians + (M_PI * 0.5f)) * speed * deltaTime;
		vr[1] -= cosf(radians + (M_PI * 0.5f)) * speed * deltaTime;
	}
	if ((input->GetKey(KeyCode.D) || input->GetKey(KeyCode.Right))) {
		vr[0] += sinf(radians + (M_PI * 0.5f)) * speed * deltaTime;
		vr[1] += cosf(radians + (M_PI * 0.5f)) * speed * deltaTime;
	}

	if (input->GetLefttAxisY() > 0.1 || input->GetLefttAxisY() < -0.1) {

		vrTXZ[0] = (speedTAxisZ * deltaTime) * sinf((DeltaRotation->Y * M_PI) / 180);
		vrTXZ[0] = vrTXZ[0] * input->GetLefttAxisY();
		_RPT1(0, "AxisX: %f , Angle * PI / 180(AccelFront): %f  \n", input->GetLeftAxisX(), vrTXZ[0]);
		vrTXZ[1] = (speedTAxisX * deltaTime) * cosf((DeltaRotation->Y * M_PI) / 180);
		vrTXZ[1] = vrTXZ[1] * input->GetLefttAxisY();
		_RPT1(0, "AxisY: %f , Angle * PI / 180(AccelSides): %f  \n", input->GetLefttAxisY(), vrTXZ[1]);		

	}

	if (input->GetLeftAxisX() > 0.1 || input->GetLeftAxisX() < -0.1) {
		vrT90XZ[0] = (speedTAxisZ * deltaTime) * sinf(((DeltaRotation->Y + 90) * M_PI) / 180);
		vrT90XZ[0] = vrT90XZ[0] * input->GetLeftAxisX();
		_RPT1(0, "AxisX: %f , Angle * PI / 180(AccelFront): %f  \n", input->GetLeftAxisX(), vrT90XZ[0]);
		vrT90XZ[1] = (speedTAxisX * deltaTime) * cosf(((DeltaRotation->Y + 90) * M_PI) / 180);
		vrT90XZ[1] = vrT90XZ[1] * input->GetLeftAxisX();
		_RPT1(0, "AxisY: %f , Angle * PI / 180(AccelSides): %f  \n", input->GetLefttAxisY(), vrT90XZ[1]);
	}

	if (input->GetRightAxisY() || input->GetRightAxisX()) {
		float angleX = input->GetRightAxisY() * speedAxisX * deltaTime;
		float angleY = input->GetRightAxisX() * speedAxisY * deltaTime;
		DeltaRotation->X -= angleX;
		DeltaRotation->Y += angleY;
	}

	if (input->GetKey(KeyCode.I)) {
		if (DeltaRotation->X > -90)
		DeltaRotation->X -= speedAxisX * deltaTime * 1.5;
	}
	if (input->GetKey(KeyCode.K)) {
		if (DeltaRotation->X < 90)
		DeltaRotation->X += speedAxisX * deltaTime * 1.5;
	}
	if (input->GetKey(KeyCode.J)) {
		DeltaRotation->Y -= speedAxisY * deltaTime * 1.5;
	}
	if (input->GetKey(KeyCode.L)) {
		DeltaRotation->Y += speedAxisY * deltaTime * 1.5;
	}
	
	/*if (input->GetKey(KeyCode.One)) {
		Bochido->ChangeTexture(0,0);
	}
	if (input->GetKey(KeyCode.Two)) {
		Bochido->ChangeTexture(1, 1);
	}
	if (input->GetKey(KeyCode.Three)) {
		Bochido->ChangeTexture(2, 2);
	}
	if (input->GetKey(KeyCode.Four)) {
		Bochido->ChangeTexture(3, 3);
	}
	if (input->GetKey(KeyCode.Five)) {
		Bochido->ChangeTexture(4, 4);
	}
	if (input->GetKey(KeyCode.Six)) {
		Bochido->ChangeTexture(5, 5);
	}
	if (input->GetKey(KeyCode.Seven)) {
		Bochido->ChangeTexture(6, 6);
	}
	if (input->GetKey(KeyCode.Eight)) {
		Bochido->ChangeTexture(7, 7);
	}
	if (input->GetKey(KeyCode.Nine)) {
		Bochido->ChangeTexture(8, 8);
	}
	if (input->GetKey(KeyCode.Zero)) {
		Bochido->ChangeTexture(9, 9);
	}*/


	if (input->GetKey(KeyCode.P)) {
		string Cadena = "Camara Posicion: " + to_string(Camera->GetPositionX()) + " , " + to_string(Camera->GetPositionY()) + " , " + to_string(Camera->GetPositionZ());
		MessageBoxA(handlerWindow, Cadena.c_str(), "Coordenadas", MB_OK);
	}
	if (input->GetKey(KeyCode.O)) {
		string Cadena = "Jugador Posicion: " + to_string(Player1->getPosicionX()) + " , " + to_string(Player1->getPosicionY()) + " , " + to_string(Player1->getPosicionZ());
		MessageBoxA(handlerWindow, Cadena.c_str(), "Coordenadas", MB_OK);
	}

	//Skydome->transicion(dna, colorDia, colorAnochecer, 3000);

	if (Polacion == true) {
		Skydome->CicloInterpolaciones(); // comentar o descomentar para las interpolaciones
		Skydome->Redraw();
	}

	//Skydome->RedefineColor(0, 0, 0, 0);

	if (input->GetKey(KeyCode.One)) {
		//Skydome->RedefineColor(0, 0, 0, 250);
		//Skydome->Redraw();
	}


	// angulo_Y < 360 ? angulo_Y += 0.0f : angulo_Y = 0;
	RObjX < 360 ? RObjX += 0.1f : RObjX = 0;
	RObjY < 360 ? RObjY += 0.1f : RObjY = 0;
	RObjZ < 360 ? RObjZ += 0.1f : RObjZ = 0;

	angulo < 360 ? angulo += 0.1f : angulo = 0;


	static int SIoNO[3] = { 0,0,0 };

	// RANGO DE ATAQUE DEL BICHO (r = 10)
	if (SIoNO[0] == 0) {
		if (DeltaPosition->X > (pos->X - 10.0f) && DeltaPosition->X < (pos->X + 10.0f)) {
			if (DeltaPosition->Z > (pos->Z - 10.0f) && DeltaPosition->Z < (pos->Z + 10.0f)) { ATK = true; }
			else { ATK = false; }
		}
		else { ATK = false; }
	}
	else { ATK == false; }
	
	static double time = 0;

	// TIEMPO DE ATAQUE
	if (ATK == true) { 
		time += 1;
		// 5s , se tardaria 1 min en matarnos
		if (time >= 50) {
			Player1->setVida(Player1->getVida() - Enemigo->getDaño());
			time = 0;
		}
	}

	// RANGO PARA MATARLO (r = 18)
	if (DeltaPosition->X > (pos->X - 18.0f) && DeltaPosition->X < (pos->X + 18.0f)) {
		if (DeltaPosition->Z > (pos->Z - 18.0f) && DeltaPosition->Z < (pos->Z + 18.0f)) { 
			if (input->GetKey(KeyCode.U) || input->GetKeyXbox(KeyCode.XBOX_X)) {
				Enemigo->setVida(Enemigo->getVida() - Player1->getDaño());
				// se muere el enemigo
				if (Enemigo->getVida() <= 0) {
					SIoNO[0] = 1;
				}
			}
		}
	}

	// RANGO DE VISION DEL BICHO (r = 30)
	// bicho 1
	if (SIoNO[0] == 0) {
		if (DeltaPosition->X > (pos->X - 30.0f) && DeltaPosition->X < (pos->X + 30.0f)) {
			if (DeltaPosition->Z > (pos->Z - 30.0f) && DeltaPosition->Z < (pos->Z + 30.0f)) { Pers = true; }
			else { Pers = false; }
		}
		else { Pers = false; }
	}
	else { Pers = false; }
	if (Pers == true) {
		if (pos->X < DeltaPosition->X) {
			pos->X += .19;
		}
		if (pos->X > DeltaPosition->X) {
			pos->X -= .19;
		}
		if (pos->Z < DeltaPosition->Z) {
			pos->Z += .19;
		}
		if (pos->Z > DeltaPosition->Z) {
			pos->Z -= .19;
		}
	}

	// movimiento senoidal del agua
	///////////////////////////////////////////////////
	const float frecX = 1.0f; const float ampX = 10.0f;
	const float frecY = 0.5f; const float ampY = 1.2f;
	const float frecZ = 0.75f; const float ampZ = 8.0f;
	
	const float vel = 0.01f;
	static float tiempo = 0.0f;

	SenIdalX = ampX * sin(2.0f * M_PI * frecX * tiempo);
	SenIdalY = ampY * sin(2.0f * M_PI * frecY * tiempo);
	SenIdalZ = ampZ * sin(2.0f * M_PI * frecZ * tiempo);

	tiempo += vel;
	//////////////////////////////////////

	/*float* matrixEmi = Emi->GetWorldMatrix();
	OpenGL->MatrixObjectRotationY(matrixEmi, DeltaRotation->Y);*/

	angulo_Y = DeltaRotation->Y;

	pos->Y = Terreno->Superficie(pos->X, pos->Z);
	DeltaPosition->Y = Terreno->Superficie(DeltaPosition->X, DeltaPosition->Z) + 2;

	DeltaPosition->X += vr[0] + vrTXZ[0] + vrT90XZ[0];
	DeltaPosition->Z += vr[1] + vrTXZ[1] + vrT90XZ[1];

	return result;
}

bool Scene::Shutdown() {
	bool result = true;

	if (Camera)
	{
		delete Camera;
		Camera = 0;
	}

	if (LightShader)
	{
		LightShader->Shutdown();
		delete LightShader;
		LightShader = 0;
	}

	if (Triangulo) {
		Triangulo->Shutdown();
		delete Triangulo;
		Triangulo = 0;
	}

	if (deLorean) {
		deLorean->Shutdown();
		delete deLorean;
		deLorean = 0;
	}

	return result;
}