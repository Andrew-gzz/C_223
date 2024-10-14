#ifndef _BOUNDINGBOXRENDERER_H_
#define _BOUNDINGBOXRENDERER_H_

#include "BoundingBox.h";
#include "Shader.h";

class BoundingBoxRenderer : public BoundingBox {
private:
	float centerUpdated[4];
	GLFLOAT3 pointsUpdated[8];

	float* worldmatrix;

	Shader* ShaderOfBox;
	OpenGLClass* OpenGL;
public:
	BoundingBoxRenderer(OpenGLClass*, GLFLOAT3, GLFLOAT3);

	bool SetShader(Shader* BoxShader);

	float* GetCenter();
	float* GetWorldMatrix();

	void Draw(float*, float*);

	void UpdateBox();

	bool GetSphericalCollision(float, float, float, float);
	bool GetBoxCollision(float, float, float);

	float Dot(float*, float*);
	float Dot(float*, GLFLOAT3);
};

#endif