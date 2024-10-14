#ifndef _LIGHTCLASS_H_
#define _LIGHTCLASS_H_

#include "openglclass.h"

class LightClass
{
private:
	OpenGLClass* OpenGL;
	float m_diffuseColor[4];
	float m_direction[3];
public:
	//LightClass(OpenGLClass* OpenGL);
	LightClass();
	LightClass(const LightClass&);
	~LightClass();

	void SetDiffuseColor(float, float, float, float);
	void SetDirection(float, float, float);

	void GetDiffuseColor(float*);
	void GetDirection(float*);
};

#endif