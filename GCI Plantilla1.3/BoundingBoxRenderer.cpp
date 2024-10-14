#include "BoundingBoxRenderer.h"

BoundingBoxRenderer::BoundingBoxRenderer(OpenGLClass* OpenGL, GLFLOAT3 max, GLFLOAT3 min) {
	this->OpenGL = OpenGL;

	GLCOLOR color = { 1.0f, 1.0f, 1.0f, 1.0f };

	Initialize(OpenGL, max, min, color);

	worldmatrix = new float[16]{ 0.0f };
	OpenGL->BuildIdentityMatrix(worldmatrix);
}

bool BoundingBoxRenderer::SetShader(Shader* BoxShader) {
	this->ShaderOfBox = BoxShader;

	return true;
}

float* BoundingBoxRenderer::GetCenter() {
	return centerUpdated;
}

float* BoundingBoxRenderer::GetWorldMatrix() {
	return this->worldmatrix;
}

void BoundingBoxRenderer::Draw(float* viewMatrix, float* projectionMatrix) {
	ShaderOfBox->SetShader();
	ShaderOfBox->PonMatriz4x4((char*)"worldMatrix", worldmatrix);
	ShaderOfBox->PonMatriz4x4((char*)"viewMatrix", viewMatrix);
	ShaderOfBox->PonMatriz4x4((char*)"projectionMatrix", projectionMatrix);

	float* color = new float[3]{ 1.0, 1.0, 1.0 };
	ShaderOfBox->PonVec3((char*)"colorShader", color);

	Render(OpenGL);

	delete[] color;
	color = NULL;
}

void BoundingBoxRenderer::UpdateBox() {
	// Obtener el centro de la caja
	centerUpdated[0] = center.x;
	centerUpdated[1] = center.y;
	centerUpdated[2] = center.z;
	centerUpdated[3] = 1.0f;

	// Actualizar el centro
	MultiplyVectorMatrix(worldmatrix, centerUpdated);

	// Actualizar los puntos
	for (int i = 0; i < 8; i++) {
		pointsUpdated[i] = MultiplyFloat3Matrix(worldmatrix, points[i]);
	}
}

bool BoundingBoxRenderer::GetSphericalCollision(float posCamX, float posCamY, float posCamZ, float radio) {
	bool isPointInsideSphere = false;

	// Actualizar la posición de la BoundingBox
	UpdateBox();

	// Calculo la distancia entre el centro de la caja y la cámara
	float d = sqrtf(pow(centerUpdated[0] - posCamX, 2) +
		pow(centerUpdated[1] - posCamY, 2) +
		pow(centerUpdated[2] - posCamZ, 2));

	// Compruebo si la distancia es menor al radio de la esfera, generando la colisión
	if (d <= radio) {
		isPointInsideSphere = true;
	}

	return isPointInsideSphere;
}

bool BoundingBoxRenderer::GetBoxCollision(float posCamX, float posCamY, float posCamZ) {
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

	u[0] = pointsUpdated[6].x - pointsUpdated[2].x; u[1] = pointsUpdated[6].y - pointsUpdated[2].y; u[2] = pointsUpdated[6].z - pointsUpdated[2].z;
	v[0] = pointsUpdated[6].x - pointsUpdated[7].x; v[1] = pointsUpdated[6].y - pointsUpdated[7].y; v[2] = pointsUpdated[6].z - pointsUpdated[7].z;
	w[0] = pointsUpdated[6].x - pointsUpdated[4].x; w[1] = pointsUpdated[6].y - pointsUpdated[4].y; w[2] = pointsUpdated[6].z - pointsUpdated[4].z;

	// Calculamos el producto punto de los vectore U, V y W con la posicón de la cámara
	float dotUX = Dot(u, posCam); // U.X
	float dotVX = Dot(v, posCam); // V.X
	float dotWX = Dot(w, posCam); // W.X

	// Calcumos los producto punto de los vectores U, V y W con los puntos listados como
	float dotUP7 = Dot(u, pointsUpdated[6]); // U.P7
	float dotVP7 = Dot(v, pointsUpdated[6]); // V.P7
	float dotWP7 = Dot(w, pointsUpdated[6]); // W.P7
	float dotUP3 = Dot(u, pointsUpdated[2]); // U.P3
	float dotVP8 = Dot(v, pointsUpdated[7]); // V.P8
	float dotWP5 = Dot(w, pointsUpdated[4]); // W.P5

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

float BoundingBoxRenderer::Dot(float* point1, float* point2) {
	float k = (point1[0] * point2[0]) + (point1[1] * point2[1]) + (point1[2] * point2[2]);

	return k;
}

float BoundingBoxRenderer::Dot(float* point1, BoundingBox::GLFLOAT3 point2) {
	float k = (point1[0] * point2.x) + (point1[1] * point2.y) + (point1[2] * point2.z);

	return k;
}