#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

#include "openglclass.h"
#include <math.h>
#include <fstream>
#include <vector>

#define PI 3.141516

#include "TextureLoaderForOpenGL.h"
#include "Shader.h"

using namespace std;

class Billboard {
private:
	OpenGLClass* OpenGL;
	TextureClass* LoaderTexRef;
	HWND hwnd;

	string TexturePath;
	int TextureID;

	float* worldmatrix;

	Shader* ShaderOfBillboard;

	struct Vertex
	{
		float posx, posy, posz;
		float u, v;
	};

	Vertex* vertices;
	unsigned int* indices;

	unsigned int m_vertexArrayId, m_vertexBufferId, m_indexBufferId;
	int m_indexCount, m_vertexCount;
public:
	Billboard(OpenGLClass* OpenGL, HWND handler, TextureClass* LoaderTexRef, string texturePath);
	~Billboard();

	bool Initialize(float escala);
	bool SetShader(Shader* BillboardShader);
	
	float* GetWorldMatrix();

	bool Render(float* viewMatrix, float* projectionMatrix, float x, float y, float z, float posCamX, float posCamZ);

	void Shutdown();
};

#endif
