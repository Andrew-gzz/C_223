#include "Scene.h"

using namespace std;

Scene::Scene(OpenGLClass* OpenGLObject, HWND hwnd) {
	handlerWindow = hwnd;
	OpenGL = OpenGLObject;
	Camera = 0;
	LightShader = 0;
	ColorShader = 0;
	ShaderBounding = 0;
	Triangulo = 0;
	Light = 0;
	speedAxisX = 0.0f;
	speedAxisY = 0.0f;
	speedTAxisX = 0.0f;
	speedTAxisZ = 0.0f;
	speed = 0.0f;
	angulo = 0.0f;
	angulo_Y = 0.0f;
	deLorean = 0;
	LoaderTexture = new TextureClass(OpenGL);
}

Scene::~Scene() {

}

bool Scene::Initialize() {
	bool result = true;
	
	string resultDetail = "";

	angulo = 0.0f;
	angulo_Y = 0.0f;
	speedAxisX = SPEED_AXIS_X;
	speedAxisY = SPEED_AXIS_Y;
	speedTAxisX = SPEEDT_AXIS_X;
	speedTAxisZ = SPEEDT_AXIS_Z;
	speed = SPEED_CAMERA;

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
		"recursos/Modelos/Edificios(EnUso)/Tienda.obj",
		"recursos/Modelos/Edificios(EnUso)/Tienda_Diffuse.png");


	if (!deLorean || !Bochido || !Municion || !Casa || !Pistola || !MedKit || !Fogata || !Estanteria || !Tienda) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the GameObject.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		result = deLorean->Initialize();
		result = Bochido->Initialize(); 
		result = Municion->Initialize();
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
		Municion->SetShaders(ShaderModel, ShaderBounding);
		Casa->SetShaders(ShaderModel, ShaderBounding);
		Pistola->SetShaders(ShaderModel, ShaderBounding);
		MedKit->SetShaders(ShaderModel, ShaderBounding);
		Fogata->SetShaders(ShaderModel, ShaderBounding);
		Estanteria->SetShaders(ShaderModel, ShaderBounding);
		
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

	Skydome = new Dome("recursos/sky.jpg", OpenGL, LoaderTexture, 800);
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

	// Collision Boxes
	box = new BoundingBoxRenderer(OpenGL,
		BoundingBox::GLFLOAT3{ 10.0f, 10.0f, 10.0f }, BoundingBox::GLFLOAT3{ -10.0f, -10.0f, -10.0f });
	if (!box) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the box.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		box->SetShader(ShaderBounding);
	}

	box2 = new BoundingBoxRenderer(OpenGL,
		BoundingBox::GLFLOAT3{ 30.0f, 10.0f, 10.0f }, BoundingBox::GLFLOAT3{ 10.0f, -10.0f, -10.0f });
	if (!box) {
		result = false;
		MessageBoxA(handlerWindow, "Could not initialize the box.", "Error", MB_OK);
		_RPT1(0, "Alert! GameObject has an error on start. \n", 0);
		return result;
	}
	else {
		box2->SetShader(ShaderBounding);
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
	//Skydome->CicloInterpolaciones();
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
	// Renderizamos las cajas de colisión
	box->Draw(viewMatrix, projectionMatrix);
	box2->Draw(viewMatrix, projectionMatrix);

	// Renderizamos los billboards
	arbol2D->Render(viewMatrix, projectionMatrix, 
		0.0f, Terreno->Superficie(0.0f, 0.0f), 0.0f, 
		DeltaPosition->X, DeltaPosition->Z);

	arbol2D->Render(viewMatrix, projectionMatrix, 
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
		DeltaPosition->X, DeltaPosition->Z);

	// Damos la instruccion de que termino la escena para que nos muestre frame a frame.
	OpenGL->EndScene();

	return result;
}

bool Scene::Update(InputClass* input, float deltaTime) {
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

	float* matrixTriangle = Triangulo->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixTriangle, -20.0f, 20.0f, 0.0f);

	float* matrixGameObject = deLorean->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixGameObject, -20.0f, 20.0f, -10.0f);

	float* matrixGameObject1 = Bochido->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixGameObject1, -30.0f, 20.0f, -10.0f);

	float* matrixGameObject2 = Municion->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixGameObject2, -40.0f, 20.0f, -10.0f);

	float* matrixGameObject3 = Casa->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixGameObject3, -20.0f, 20.0f, 20.0f);
	
	float* matrixGameObject4 = Pistola->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixGameObject4, -20.0f, 20.0f, -15.0f);
	OpenGL->MatrixObjectScale(matrixGameObject4, 8, 8, 8);

	float* matrixGameObject5 = MedKit->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixGameObject5, -20.0f, 20.0f, -17.0f);

	float* matrixGameObject6 = Fogata->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixGameObject6, -22.0f, 20.0f, -17.0f);

	float* matrixGameObject7 = Estanteria->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixGameObject7, -24.0f, 20.0f, -17.0f);

	float* matrixGameObject8 = Tienda->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixGameObject8, -40.0f, 20.0f, 20.0f);
	//Tranformaciones de cajas de colisión
	float* auxMatrix = new float[16]{ 0.0f };
	OpenGL->BuildIdentityMatrix(auxMatrix);

	float* matrixBox = box->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixBox, -40.0f, 10.0f, 40.0f);

	OpenGL->MatrixScale(auxMatrix, 1.0f, 1.0f, 0.3f);
	OpenGL->MatrixMultiply(matrixBox, matrixBox, auxMatrix);

	float* matrixBox2 = box2->GetWorldMatrix();
	OpenGL->MatrixTranslation(matrixBox2, -40.0f, 10.0f, 40.0f);

	OpenGL->MatrixScale(auxMatrix, 0.3f, 1.0f, 1.0f);
	OpenGL->MatrixMultiply(matrixBox2, matrixBox2, auxMatrix);

	//Colisión por esfera
	/*if (deLorean->GetSphericalCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z, 2)) {
		MessageBox(handlerWindow, L"Colisionando", L"Aviso", MB_OK);
	}*/

	// rotacion del skydome

	/*Skydome->MatrixRotationY(matrixSkydome, angulo_Y);*/

	static bool Dlor = false;

	//Colisión por caja
	if (deLorean->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		// desactiva la restriccon de movimiento del HitBox
		if (Dlor == false) {
			DeltaPosition->X = LastDeltaPosition->X;
			DeltaPosition->Y = LastDeltaPosition->Y;
			DeltaPosition->Z = LastDeltaPosition->Z;
		}
		if (input->GetKey(KeyCode.E)) {
			Dlor = true; 
		}
		// este if hace que al pegarte al Dlorian y piques a la "E" el modelo estara en la posicion de la camara
		// pero unicamente se podra mover el modelo del Dlorian en el "hitbox" dodne se cargo el modelo
		/*if (Dlor == true) {
			OpenGL->MatrixTranslation(matrixGameObject, DeltaPosition->X, DeltaPosition->Y - 1.3f, DeltaPosition->Z);
			OpenGL->MatrixObjectRotationY(matrixGameObject, (angulo_Y * 0.0174532925f) - 92.67);
		}*/
	}
	
	// este if hace que el modelo este en la posicion de la camara en todo el mapa picando "E"
	
	if (Dlor == true) {
		static float DlorianAltura = 0.0f;
		// me bajo del carro
		if (input->GetKey(KeyCode.Q)) {
			Dlor = false;
			DlorianAltura = 0.0f;
		}
		// aumento la altura ( volando )
		if (input->GetKey(KeyCode.Space)) {
			DlorianAltura += 0.1f;
		}
		// deciendo
		if (input->GetKey(KeyCode.Enter)) {
			DlorianAltura -= 0.1f;
		}	

		DeltaPosition->Y += DlorianAltura;
		OpenGL->MatrixTranslation(matrixGameObject, DeltaPosition->X, DeltaPosition->Y - 1.3f, DeltaPosition->Z);
		OpenGL->MatrixObjectRotationY(matrixGameObject, (angulo_Y * 0.0174532925f) - 92.67);

	}

	//OpenGL->MatrixObjectRotationZ(matrixGameObject, angulo);

	if (Municion->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
	}
	if (Casa->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
	}
	if (Pistola->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
	}
	if (MedKit->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
	}
	if (Fogata->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
	}
	if (Estanteria->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
	}
	if (Tienda->GetBoxCollision(DeltaPosition->X, DeltaPosition->Y, DeltaPosition->Z)) {
		DeltaPosition->X = LastDeltaPosition->X;
		DeltaPosition->Y = LastDeltaPosition->Y;
		DeltaPosition->Z = LastDeltaPosition->Z;
	}
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

	static bool Activar = false;

	if (input->GetKey(KeyCode.Shift) == true) {
		speed += speed * .03;
	}
	else{ speed = SPEED_CAMERA; }

	/*if (input->GetKey(KeyCode.Shift) ) {
		if (Activar == true) { Activar = false; }
		else if (Activar == false) { Activar = true; }

		if (Activar == true) { speed += speed * 1.2; }
		else if (Activar == false) { speed = SPEED_CAMERA; }
	}*/
	
	return result;
}

bool Scene::ManageCommands() {
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


	if (input->GetKey(KeyCode.One)) {
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
	}

	// angulo_Y < 360 ? angulo_Y += 0.0f : angulo_Y = 0;
	angulo < 360 ? angulo += 0.1f : angulo = 0;

	angulo_Y = DeltaRotation->Y;

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