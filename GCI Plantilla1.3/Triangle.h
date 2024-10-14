#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include "openglclass.h"
#include <string>
#include "lightshaderclass.h"

class TriangleClass
{
private:
	struct VertexType
	{
		float x, y, z;
		float r, g, b;
	};
	OpenGLClass* OpenGL;
	Shader* ShaderTriangle;
	float* worldmatrix;
	bool InitializeBuffers();
	void ShutdownBuffers();
	int m_vertexCount, m_indexCount;
	unsigned int m_vertexArrayId, m_vertexBufferId, m_indexBufferId;

public:
	TriangleClass(OpenGLClass* OpenGL);
	TriangleClass(const TriangleClass&);
	~TriangleClass();
	bool Initialize();
	float* GetWorldMatrix();
	bool SetShaderTriangle(Shader* TriangleShader);
	void Render();
	void Render(float* viewMatrix, float* projectionMatrix);
	void Shutdown();
};
#endif // !_TRIANGLE_H_
