#pragma once
#ifndef _DOME_H_
#define _DOME_H_

#include "openglclass.h"
#include "Imagenes.h"
#include <math.h>
#include <fstream>
#include <vector>

#define PI 3.141516

#include "lightshaderclass.h"
#include "TextureLoaderForOpenGL.h"

using namespace std;

class Dome : public Imagenes
{
private:
	int slices;
	int slides;
	float radius;
	string pathTexture;

	unsigned int m_vertexArrayId, m_vertexBufferId, m_indexBufferId;
	int m_indexCount, m_vertexCount;
	float* myMatrix;
	float XTranslate, YTranslate, ZTranslate;
	float XScale, YScale, ZScale;
	float XFactorRotation, YFactorRotation, ZFactorRotation;
	float AngleRotation;
	float inicio;
	float final;

	float ColorInicial[4] = { 1.00f, 1.00f, 1.00f, 1.00f };
	float* DeltaColor;

	void ConstructDome();
	OpenGLClass* OpenGL;
	TextureClass* LoaderTexRef;
	Shader* SkyShader;
public:

	bool interpolacionAutomatica = false; //Bandera
	bool interpolacionNocheAmanecer = false; //Bandera
	bool interpolacionAmanecerTarde = false; //Bandera
	bool interpolacionTardeNoche = false; //Bandera
	bool Interpolaciones[3] = { 0,1,0 };
	int	cicloLuz = 360; //Segundos en que permanece la textura actual
	int cicloInterpolacionSeg = 200; // Lo que me demoro en cambiar a la siguiente textura
	float coeficienteDeCambio = 0;
	int elapsedTime = 0;
	// 25 + 10 Segundos = 60Seg = 1 min

	struct VertexType {
		float Posx, Posy, Posz;
		float ColorR, ColorG, ColorB;
		float Normx, Normy, Normz;
		float u, v;
		float Tx, Ty, Tz;
		float Bx, By, Bz;
	};

	VertexType* vertices;
	unsigned int* indices;

	Dome();
	Dome(string pathTexture, OpenGLClass* OpenGL, TextureClass* LoaderTextureRef, float radius);
	~Dome();

	float* GetWorldMatrix();

	bool Redraw();

	void RedefineTransformsTSR(float toXTranslate, float toYTranslate, float toZTranslate,
		float toXScale, float toYScale, float toZScale,
		float toXFactorRotation, float toYFactorRotation, float toZFactorRotation, float Angle);

	void RedefineTransformsTS(float toXTranslate, float toYTranslate, float toZTranslate,
		float toXScale, float toYScale, float toZScale);

	void RedefineTransformsT(float toXTranslate, float toYTranslate, float toZTranslate);

	void RedefineColor(float changeRColor, float changeGColor, float changeBColor, float changeAColor);

	float* GetDeltaColor();

	void SetIterpolationDay(bool Auto, bool TreeSteps);

	void MatrixTranslation();
	void MatrixRotationY();
	void MatrixScale();

	float* colorDomo;
	float ColorAnterior[3] = { 0.0f, 0.0f, 0.0f };
	float ColorTarget[3] = { 0.0f, 0.0f, 0.0f };

	bool Tarde();
	bool Noche();
	bool Amanecer();

	void CicloInterpolaciones();

	bool Render(float* viewMatrix, float* projectionMatrix);

	void ChancheTexture(string);

	bool Initialize();
	void Update();
	void Shutdown();

	void SetShader(Shader*);

	GLuint Texturas;
	int TextureID1, TextureID2, TextureID3, TextureID4;
	int textureNormalID;

	unsigned int m_textureID1;
	unsigned int m_textureID2;
	unsigned int m_textureID3;
	unsigned int m_textureID4;
};

#endif