#ifndef _BoundingBox_
#define _BoundingBox_

#include "openglclass.h";
#include <vector>
#include <string>
//#include "ShaderColor.h"
using std::vector;
using namespace std;

class BoundingBox
{
protected:
	struct VertexType
	{
		float posX;
		float posY;
		float posZ;
	};

public:
	struct GLFLOAT3 {
		float x;
		float y;
		float z;
	};
	struct GLCOLOR {
		float r;
		float g;
		float b;
		float a;
	};
	BoundingBox();
	BoundingBox(const BoundingBox&);
	~BoundingBox();
	//contextGL,hwnd,float3,float3,float4
	bool Initialize(OpenGLClass*, GLFLOAT3, GLFLOAT3, GLCOLOR);
	//contextGL
	bool Render(OpenGLClass*);
	void Shutdown(OpenGLClass*);
	void RenderBuffers(OpenGLClass*);
	GLFLOAT3 center;
	GLFLOAT3 points[8];
	
	GLFLOAT3 float3ToGLFLOAT3(float,float,float);
	void GFLOAT3_GLCOLOR_to_VertexType(VertexType*, GLFLOAT3*, GLCOLOR*);
	float r;
private:
	unsigned int m_vertexArrayId, m_vertexBufferId, m_indexBufferId;
	int m_indexCount, m_vertexCount;
	//Shader* m_shaderColor;
public:
	GLFLOAT3 min, max;
	void MultiplyVectorMatrix(float*, float*);
	GLFLOAT3 MultiplyFloat3Matrix(float*, GLFLOAT3);
};

#endif // !_BoundingBox_
