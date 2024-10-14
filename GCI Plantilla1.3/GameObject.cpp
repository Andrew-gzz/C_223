#include "GameObject.h"


GameObject::GameObject(OpenGLClass* OpenGL, HWND hwnd, TextureClass* LoaderTextureRef) {
	this->OpenGL = OpenGL;
	this->hwnd = hwnd;
	this->LoaderTexRef = LoaderTextureRef;
	this->worldmatrix = 0;
	ShaderOfModel = 0;
}

GameObject::GameObject(OpenGLClass* OpenGL, HWND hwnd, TextureClass* LoaderTextureRef, const char* objPath, string texturePath, string normalMappingPath) {
	this->OpenGL = OpenGL;
	this->LoaderTexRef = LoaderTextureRef;
	this->worldmatrix = 0;
	this->hwnd = hwnd;
	ObjectPath = objPath;
	TexturePath1 = texturePath;
	NormalMapPath = normalMappingPath;
	ShaderOfModel = 0;
}

GameObject::GameObject(OpenGLClass* OpenGL, HWND hwnd, TextureClass* LoaderTextureRef, const char* objPath, string texturePath) {
	this->OpenGL = OpenGL;
	this->hwnd = hwnd;
	this->LoaderTexRef = LoaderTextureRef;
	this->worldmatrix = 0;
	ObjectPath = objPath;
	TexturePath1 = texturePath;
	ShaderOfModel = 0;
}

GameObject::~GameObject() {
}

float * GameObject::GetWorldMatrix() {
	return this->worldmatrix;
}

bool GameObject::SetShaderModel(Shader* ModelShader) {
	ShaderOfModel = ModelShader;
	return true;
}

bool GameObject::SetShaders(Shader* ModelShader, Shader* BoxShader) {
	ShaderOfModel = ModelShader;
	ShaderOfBox = BoxShader;

	return true;
}

bool GameObject::AddTexture(string texturePath) {
	try {;
	model->AddTexture(texturePath, true);
		return true;
	}
	catch (exception e) {
		printf("Error,&s", e);
		return false;
	}
}

bool GameObject::Initialize() {
	bool result = true;
	string resultDetail = "";

	worldmatrix = new float[16] { 0.0f };
	if (!OpenGL) {
		result = false;
		_RPT1(0, "Alert! GameObject OpenGL does not defined. \n", 0);
		return result;
	}
	else
		OpenGL->BuildIdentityMatrix(worldmatrix);
	model = new Model(OpenGL, hwnd, LoaderTexRef);
	model->Initialize(ObjectPath, TexturePath1);
	if (!result) {
		_RPT1(0, "Alert! GameObject Model does not initializdd. \n", 0);
		return result;
	}

	return result;
}

void GameObject::ChangeTexture(int From, int To) {
	model->ChangeTexture(From, To);
}

void GameObject::Render(float* viewMatrix,float* projectionMatrix, bool showBox) {
	
	if (model) {
		ShaderOfModel->SetShader();
		ShaderOfModel->PonMatriz4x4((char*)"worldMatrix", worldmatrix);
		ShaderOfModel->PonMatriz4x4((char*)"viewMatrix", viewMatrix);
		ShaderOfModel->PonMatriz4x4((char*)"projectionMatrix", projectionMatrix);
		model->Render(ShaderOfModel);

		if (showBox) {
			ShaderOfBox->SetShader();
			ShaderOfBox->PonMatriz4x4((char*)"worldMatrix", worldmatrix);
			ShaderOfBox->PonMatriz4x4((char*)"viewMatrix", viewMatrix);
			ShaderOfBox->PonMatriz4x4((char*)"projectionMatrix", projectionMatrix);

			float* color = new float[3]{ 1.0, 1.0, 1.0 };
			ShaderOfBox->PonVec3((char*)"colorShader", color);

			/*BoundingBox* box = model->GetBox();
			box->Render(OpenGL);*/
			model->GetBox()->Render(OpenGL);

			delete color;
			color = NULL;
		}
	}
}

float * GameObject::MultiplyVectorMatrix(float*matrix, float*vector) {
	float* vr = new float[4] {0.0f};
	
	// Matriz de 4x4 por véctice/vector (Matriz de 1x4)
	vr[0] = matrix[0] * vector[0] + matrix[4] * vector[1] + matrix[8] * vector[2] + matrix[12] * vector[3];
	vr[1] = matrix[1] * vector[0] + matrix[5] * vector[1] + matrix[9] * vector[2] + matrix[13] * vector[3];
	vr[2] = matrix[2] * vector[0] + matrix[6] * vector[1] + matrix[10] * vector[2] + matrix[14] * vector[3];
	vr[3] = matrix[3] * vector[0] + matrix[7] * vector[1] + matrix[11] * vector[2] + matrix[15] * vector[3];

	return vr;
}

void GameObject::Shutdown() {
	if (worldmatrix) {
		delete worldmatrix;
		worldmatrix = 0;
	}
	if (model)
		model = NULL;
}

float* GameObject::GetCenter() {
	return center;
}

void GameObject::UpdateBox() {
	// Obtener el centro de la caja
	center[0] = model->GetBox()->center.x;
	center[1] = model->GetBox()->center.y;
	center[2] = model->GetBox()->center.z;
	center[3] = 1.0f;

	// Actualizar el centro
	model->GetBox()->MultiplyVectorMatrix(worldmatrix, this->center);

	// Actualizar los puntos
	for (int i = 0; i < 8; i++) {
		points[i] = model->GetBox()->MultiplyFloat3Matrix(worldmatrix, model->GetBox()->points[i]);
	}
}

bool GameObject::GetSphericalCollision(float posCamX, float posCamY, float posCamZ, float radio) {
	bool isPointInsideSphere = false;

	// Actualizar la posición de la BoundingBox
	UpdateBox();

	// Calculo la distancia entre el centro de la caja y la cámara
	float d = sqrtf(pow(center[0] - posCamX, 2) + 
					pow(center[1] - posCamY, 2) + 
					pow(center[2] - posCamZ, 2));

	// Compruebo si la distancia es menor al radio de la esfera, generando la colisión
	if (d <= radio) {
		isPointInsideSphere = true;
	}

	return isPointInsideSphere;
}

bool GameObject::GetBoxCollision(float posCamX, float posCamY, float posCamZ) {
	bool isPointInsideBox = false;

	// Actualizar la posición de la BoundingBox
	UpdateBox();

	// Pasamos la posición de la cámara a un arreglo de flotantes para referenciarlo más facilmente
	float posCam[3] = { posCamX , posCamY, posCamZ };

	// Calculamos los vectores U, V y W como la distancia entre los puntos listados de la caja de colisión
	// U = P7 - P3
	// V = P7 - P8
	// W = P7 - P5
	float u[3];
	float v[3];
	float w[3];

	u[0] = points[6].x - points[2].x; u[1] = points[6].y - points[2].y; u[2] = points[6].z - points[2].z;
	v[0] = points[6].x - points[7].x; v[1] = points[6].y - points[7].y; v[2] = points[6].z - points[7].z;
	w[0] = points[6].x - points[4].x; w[1] = points[6].y - points[4].y; w[2] = points[6].z - points[4].z;

	// Calculamos el producto punto de los vectore U, V y W con la posicón de la cámara
	float dotUX = Dot(u, posCam); // U.X
	float dotVX = Dot(v, posCam); // V.X
	float dotWX = Dot(w, posCam); // W.X

	// Calcumos los producto punto de los vectores U, V y W con los puntos listados como
	float dotUP7 = Dot(u, points[6]); // U.P7
	float dotVP7 = Dot(v, points[6]); // V.P7
	float dotWP7 = Dot(w, points[6]); // W.P7
	float dotUP3 = Dot(u, points[2]); // U.P3
	float dotVP8 = Dot(v, points[7]); // V.P8
	float dotWP5 = Dot(w, points[4]); // W.P5
	
	// La solición será verdadera si:
	// - U.X se encuentra entre U.P7 y U.P3
	// - V.X se encuentra entre V.P7 y V.P8
	// - W.X se encuentra entre W.P7 y W.P5
	if (dotUX < dotUP7 && dotUX > dotUP3) {
		if (dotVX < dotVP7 && dotVX > dotVP8) {
			if (dotWX < dotWP7 && dotWX > dotWP5) {
				isPointInsideBox = true;
			}
		}
	}

	return isPointInsideBox;
}

float GameObject::Dot(float* point1, float* point2) {
	float k = (point1[0] * point2[0]) + (point1[1] * point2[1]) + (point1[2] * point2[2]);

	return k;
}

float GameObject::Dot(float* point1, BoundingBox::GLFLOAT3 point2) {
	float k = (point1[0] * point2.x) + (point1[1] * point2.y) + (point1[2] * point2.z);

	return k;
}