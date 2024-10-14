#include "BoundingBox.h"

BoundingBox::BoundingBox()
{
	/*m_vertexBuffer = 0;
	m_indexBuffer = 0;*/
	//m_shaderColor = 0;
}

BoundingBox::BoundingBox(const BoundingBox& other)
{

}

BoundingBox::~BoundingBox()
{

}

bool BoundingBox::Initialize(OpenGLClass *OpenGL, GLFLOAT3 max, GLFLOAT3 min, GLCOLOR color)
{
	this->min = min;
	this->max = max;
	//Read in the vertex count
	m_vertexCount = 24;

	//Set the number of indices to be the same as the vertex count
	m_indexCount = m_vertexCount;
	/*---------------front vertex ----------------*/
	//P1
	//(-x,y,z)
	GLFLOAT3 p1 = float3ToGLFLOAT3(min.x, max.y, max.z);
	points[0] = p1;
	//(x,y,z)
	GLFLOAT3 p2 = float3ToGLFLOAT3(max.x, max.y, max.z);
	points[1] = p2;
	//(-x,-y,z)
	GLFLOAT3 p3 = float3ToGLFLOAT3(min.x, min.y, max.z);
	points[2] = p3;
	//(x,-y,z)
	GLFLOAT3 p4 = float3ToGLFLOAT3(max.x, min.y, max.z);
	points[3] = p4;
	/*--------------back vertex-------------------*/
	//(-x,y,-z)
	GLFLOAT3 p5 = float3ToGLFLOAT3(min.x, max.y, min.z);
	points[4] = p5;
	//(x,y,-z)
	GLFLOAT3 p6 = float3ToGLFLOAT3(max.x, max.y, min.z);
	points[5] = p6;
	//(-x,-y,-z)
	GLFLOAT3 p7 = float3ToGLFLOAT3(min.x, min.y, min.z);
	points[6] = p7;
	//(x,-y,-z)
	GLFLOAT3 p8 = float3ToGLFLOAT3(max.x, min.y, min.z);
	points[7] = p8;

	VertexType* vertices;
	unsigned long* indices;
	/*
	COREGIIR OPENGL
	*/
	//D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	//D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	string resultS;
	//Create the vertex array
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
		return false;

	//Create the index array
	indices = new unsigned long[m_indexCount];
	if (!indices)
		return false;

	int indexCount = 0;
	/*-------------------Front Cube---------------*/
	//Linea1
	GFLOAT3_GLCOLOR_to_VertexType(&vertices[indexCount],&p1,&color);
	indices[indexCount] = indexCount;
	indexCount++;

	GFLOAT3_GLCOLOR_to_VertexType(&vertices[indexCount], &p2, &color);
	indices[indexCount] = indexCount;
	indexCount++;

	//Linea2
	GFLOAT3_GLCOLOR_to_VertexType(&vertices[indexCount], &p2, &color);
	indices[indexCount] = indexCount;
	indexCount++;

	GFLOAT3_GLCOLOR_to_VertexType(&vertices[indexCount], &p4, &color);
	indices[indexCount] = indexCount;
	indexCount++;
	//Linea3
	GFLOAT3_GLCOLOR_to_VertexType(&vertices[indexCount], &p3, &color);
	indices[indexCount] = indexCount;
	indexCount++;

	GFLOAT3_GLCOLOR_to_VertexType(&vertices[indexCount], &p4, &color);
	indices[indexCount] = indexCount;
	indexCount++;
	//Linea 4
	GFLOAT3_GLCOLOR_to_VertexType(&vertices[indexCount], &p1, &color);
	indices[indexCount] = indexCount;
	indexCount++;

	GFLOAT3_GLCOLOR_to_VertexType(&vertices[indexCount], &p3, &color);
	indices[indexCount] = indexCount;
	indexCount++;

	/*-----------Back Cube----------*/
	//Linea5
	GFLOAT3_GLCOLOR_to_VertexType(&vertices[indexCount], &p5, &color);
	indices[indexCount] = indexCount;
	indexCount++;

	GFLOAT3_GLCOLOR_to_VertexType(&vertices[indexCount], &p6, &color);
	indices[indexCount] = indexCount;
	indexCount++;
	//Linea 6
	GFLOAT3_GLCOLOR_to_VertexType(&vertices[indexCount], &p6, &color);
	indices[indexCount] = indexCount;
	indexCount++;

	GFLOAT3_GLCOLOR_to_VertexType(&vertices[indexCount], &p8, &color);
	indices[indexCount] = indexCount;
	indexCount++;
	//Linea 7
	GFLOAT3_GLCOLOR_to_VertexType(&vertices[indexCount], &p7, &color);
	indices[indexCount] = indexCount;
	indexCount++;

	GFLOAT3_GLCOLOR_to_VertexType(&vertices[indexCount], &p8, &color);
	indices[indexCount] = indexCount;
	indexCount++;
	//Linea 8
	GFLOAT3_GLCOLOR_to_VertexType(&vertices[indexCount], &p5, &color);
	indices[indexCount] = indexCount;
	indexCount++;

	GFLOAT3_GLCOLOR_to_VertexType(&vertices[indexCount], &p7, &color);
	indices[indexCount] = indexCount;
	indexCount++;

	/*----------Sildes---------------*/
	//Linea 9
	GFLOAT3_GLCOLOR_to_VertexType(&vertices[indexCount], &p1, &color);
	indices[indexCount] = indexCount;
	indexCount++;

	GFLOAT3_GLCOLOR_to_VertexType(&vertices[indexCount], &p5, &color);
	indices[indexCount] = indexCount;
	indexCount++;
	//Linea 10
	GFLOAT3_GLCOLOR_to_VertexType(&vertices[indexCount], &p3, &color);
	indices[indexCount] = indexCount;
	indexCount++;

	GFLOAT3_GLCOLOR_to_VertexType(&vertices[indexCount], &p7, &color);
	indices[indexCount] = indexCount;
	indexCount++;
	//Linea 11
	GFLOAT3_GLCOLOR_to_VertexType(&vertices[indexCount], &p2, &color);
	indices[indexCount] = indexCount;
	indexCount++;

	GFLOAT3_GLCOLOR_to_VertexType(&vertices[indexCount], &p6, &color);
	indices[indexCount] = indexCount;
	indexCount++;
	//Linea 12
	GFLOAT3_GLCOLOR_to_VertexType(&vertices[indexCount], &p4, &color);
	indices[indexCount] = indexCount;
	indexCount++;

	GFLOAT3_GLCOLOR_to_VertexType(&vertices[indexCount], &p8, &color);
	indices[indexCount] = indexCount;

	OpenGL->glGenVertexArrays(1,&m_vertexArrayId);
	OpenGL->glBindVertexArray(m_vertexArrayId);

	OpenGL->glGenBuffers(1,&m_vertexBufferId);
	OpenGL->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
	OpenGL->glBufferData(GL_ARRAY_BUFFER,m_vertexCount*sizeof(VertexType), vertices, GL_STATIC_DRAW);

	OpenGL->glEnableVertexAttribArray(0); //Position
	OpenGL->glEnableVertexAttribArray(1); // Color

	OpenGL->glBindBuffer(GL_ARRAY_BUFFER,m_vertexBufferId);
	OpenGL->glVertexAttribPointer(0,3,GL_FLOAT,false,sizeof(VertexType),0);
	OpenGL->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
	OpenGL->glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(VertexType), (unsigned char*) NULL + (3*sizeof(float)));

	OpenGL->glGenBuffers(1,&m_indexBufferId);
	OpenGL->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_indexBufferId);
	OpenGL->glBufferData(GL_ELEMENT_ARRAY_BUFFER,m_indexCount*sizeof(unsigned int), indices, GL_STATIC_DRAW);

	//Release the arrays now that the vertex and index buffers have been created and loaded
	delete[]vertices;
	vertices = 0;

	delete[]indices;
	indices = 0;

	//check center
	center.x = (p1.x + p2.x + p3.x + p4.x + p5.x + p6.x + p7.x + p8.x) / 8.0f;
	center.y = (p1.y + p2.y + p3.y + p4.y + p5.y + p6.y + p7.y + p8.y) / 8.0f;
	center.z = (p1.z + p2.z + p3.z + p4.z + p5.z + p6.z + p7.z + p8.z) / 8.0f;
	float r1 = sqrtf(powf(float(p1.x - center.x), 2) + powf(float(p1.z - center.z), 2) + powf(float(p1.z - center.z), 2));
	float r2 = sqrtf(powf(float(p2.x - center.x), 2) + powf(float(p2.z - center.z), 2) + powf(float(p2.z - center.z), 2));
	float r3 = sqrtf(powf(float(p3.x - center.x), 2) + powf(float(p3.z - center.z), 2) + powf(float(p3.z - center.z), 2));
	float r4 = sqrtf(powf(float(p4.x - center.x), 2) + powf(float(p4.z - center.z), 2) + powf(float(p4.z - center.z), 2));
	float r5 = sqrtf(powf(float(p5.x - center.x), 2) + powf(float(p5.z - center.z), 2) + powf(float(p5.z - center.z), 2));
	float r6 = sqrtf(powf(float(p6.x - center.x), 2) + powf(float(p6.z - center.z), 2) + powf(float(p6.z - center.z), 2));
	float r7 = sqrtf(powf(float(p7.x - center.x), 2) + powf(float(p7.z - center.z), 2) + powf(float(p7.z - center.z), 2));
	float r8 = sqrtf(powf(float(p8.x - center.x), 2) + powf(float(p8.z - center.z), 2) + powf(float(p8.z - center.z), 2));
	r = r1;
	if (r < r2)
		r = r2;
	if (r < r3)
		r = r3;
	if (r < r4)
		r = r4;
	if (r < r5)
		r = r5;
	if (r < r6)
		r = r6;
	if (r < r7)
		r = r7;
	if (r < r8)
		r = r8;

	return true;
}

bool BoundingBox::Render(OpenGLClass* OpenGL)
{
	RenderBuffers(OpenGL);
	//Render shader
	return true;
}

void BoundingBox::Shutdown(OpenGLClass*OpenGL)
{
	OpenGL->glDisableVertexAttribArray(0);
	OpenGL->glDisableVertexAttribArray(1);

	OpenGL->glBindBuffer(GL_ARRAY_BUFFER,0);
	OpenGL->glDeleteBuffers(1,&m_vertexBufferId);

	OpenGL->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	OpenGL->glDeleteBuffers(1,&m_indexBufferId);

	OpenGL->glBindVertexArray(0);
	OpenGL->glDeleteVertexArrays(1,&m_vertexArrayId);
	return;
}

void BoundingBox::RenderBuffers(OpenGLClass* OpenGL)
{

	OpenGL->glBindVertexArray(m_vertexArrayId);
	glDrawElements(GL_LINES, m_indexCount, GL_UNSIGNED_INT, 0);
	
	return;
}
BoundingBox::GLFLOAT3 BoundingBox::float3ToGLFLOAT3(float x, float y, float z) {
	GLFLOAT3 pos;
	pos.x = x;
	pos.y = y;
	pos.z = z;
	return pos;
}
void BoundingBox::GFLOAT3_GLCOLOR_to_VertexType(VertexType* vertex, GLFLOAT3* pos , GLCOLOR*color) {
	vertex->posX = pos->x;
	vertex->posY = pos->y;
	vertex->posZ = pos->z;
	//vertex->colorR = color->r;
	//vertex->colorG = color->g;
	//vertex->colorB = color->b;
	//vertex->colorA = color->a;
}

void BoundingBox::MultiplyVectorMatrix(float* matrix, float* vector) {
	float x = vector[0];
	float y = vector[1];
	float z = vector[2];

	// Matriz de 4x4 por véctice/vector (Matriz de 1x4)
	vector[0] = matrix[0] * x + matrix[4] * y + matrix[8] * z + matrix[12] * vector[3];
	vector[1] = matrix[1] * x + matrix[5] * y + matrix[9] * z + matrix[13] * vector[3];
	vector[2] = matrix[2] * x + matrix[6] * y + matrix[10] * z + matrix[14] * vector[3];
	vector[3] = matrix[3] * x + matrix[7] * y + matrix[11] * z + matrix[15] * vector[3];
}

BoundingBox::GLFLOAT3 BoundingBox::MultiplyFloat3Matrix(float* matrix, GLFLOAT3 point) {
	GLFLOAT3 resultPoint;

	resultPoint.x = matrix[0] * point.x + matrix[4] * point.y + matrix[8] * point.z + matrix[12];
	resultPoint.y = matrix[1] * point.x + matrix[5] * point.y + matrix[9] * point.z + matrix[13];
	resultPoint.z = matrix[2] * point.x + matrix[6] * point.y + matrix[10] * point.z + matrix[14];

	return resultPoint;
}