#include "Billboard.h"
#include <GL/GL.h>
#include <GL/GLU.h>

Billboard::Billboard(OpenGLClass* OpenGL, HWND handler, TextureClass* LoaderTexRef, string texturePath) {
	this->OpenGL = OpenGL;
	this->hwnd = hwnd;
	this->LoaderTexRef = LoaderTexRef;
	this->TexturePath = texturePath;
}

Billboard::~Billboard() {
}

float Billboard::getMatrixPosX(float* matrix) {
	return matrix[12];
}
float Billboard::getMatrixPosY(float* matrix) {
	return matrix[13];
}
float Billboard::getMatrixPosZ(float* matrix) {
	return matrix[14];
}


bool Billboard::Initialize(float escala) {
	// Se inicializa y guarda el ID de la textura
	if (TexturePath != "") {
		TextureID = LoaderTexRef->Initialize(TexturePath, true);
	}

	// Se calculan los vertices e indices
	m_vertexCount = 4;
	m_indexCount = 6;

	vertices = new Vertex[m_vertexCount];
	indices = new unsigned int[m_indexCount];

	vertices[0].posx = 0;
	vertices[0].posy = 0;
	vertices[0].posz = -1 * escala;
	vertices[0].u = 0;
	vertices[0].v = 1;

	vertices[1].posx = 0;
	vertices[1].posy = 2 * escala;
	vertices[1].posz = -1 * escala;
	vertices[1].u = 0;
	vertices[1].v = 0;

	vertices[2].posx = 0;
	vertices[2].posy = 2 * escala;
	vertices[2].posz = 1 * escala;
	vertices[2].u = 1;
	vertices[2].v = 0;

	vertices[3].posx = 0;
	vertices[3].posy = 0;
	vertices[3].posz = 1 * escala;
	vertices[3].u = 1;
	vertices[3].v = 1;

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

	indices[3] = 0;
	indices[4] = 2;
	indices[5] = 3;

	// Inicializar los buffers
	OpenGL->glGenVertexArrays(1, &m_vertexArrayId);
	OpenGL->glBindVertexArray(m_vertexArrayId);

	OpenGL->glGenBuffers(1, &m_vertexBufferId);
	OpenGL->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
	OpenGL->glBufferData(GL_ARRAY_BUFFER, m_vertexCount * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	OpenGL->glEnableVertexAttribArray(0); //Position
	OpenGL->glEnableVertexAttribArray(1); // Texcoord

	OpenGL->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
	OpenGL->glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), 0);
	OpenGL->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
	OpenGL->glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(Vertex), (unsigned char*)NULL + (3 * sizeof(float)));

	OpenGL->glGenBuffers(1, &m_indexBufferId);
	OpenGL->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId);
	OpenGL->glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	// Release the arrays now that the vertex and index buffers have been created and loaded
	delete[]vertices;
	vertices = 0;

	delete[]indices;
	indices = 0;

	// Inicializamos la matriz de mundo
	worldmatrix = new float[16]{ 0.0f };
	OpenGL->BuildIdentityMatrix(worldmatrix);

	return true;
}

bool Billboard::Initialize2(float escala, float uI, float uEn) {
	// Se inicializa y guarda el ID de la textura
	if (TexturePath != "") {
		TextureID = LoaderTexRef->Initialize(TexturePath, true);
	}

	// Se calculan los vertices e indices
	m_vertexCount = 4;
	m_indexCount = 6;

	vertices = new Vertex[m_vertexCount];
	indices = new unsigned int[m_indexCount];

	vertices[0].posx = 0;
	vertices[0].posy = 0;
	vertices[0].posz = -1 * escala;
	vertices[0].u = uI;     // Mantén en 0 (izquierda)
	vertices[0].v = 1;     // Mantén en 1 (parte inferior)

	vertices[1].posx = 0;
	vertices[1].posy = 2 * escala;
	vertices[1].posz = -1 * escala;
	vertices[1].u = uI;     // Mantén en 0 (izquierda)
	vertices[1].v = 0;     // Mantén en 0 (parte superior)

	vertices[2].posx = 0;
	vertices[2].posy = 2 * escala;
	vertices[2].posz = 1 * escala;
	vertices[2].u = uEn;   // Cambia a 0.5 para el borde derecho de los primeros 96 píxeles
	vertices[2].v = 0;     // Mantén en 0 (parte superior)

	vertices[3].posx = 0;
	vertices[3].posy = 0;
	vertices[3].posz = 1 * escala;
	vertices[3].u = uEn;   // Cambia a 0.5 para el borde derecho de los primeros 96 píxeles
	vertices[3].v = 1;     // Mantén en 1 (parte inferior)

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

	indices[3] = 0;
	indices[4] = 2;
	indices[5] = 3;

	// Inicializar los buffers
	OpenGL->glGenVertexArrays(1, &m_vertexArrayId);
	OpenGL->glBindVertexArray(m_vertexArrayId);

	OpenGL->glGenBuffers(1, &m_vertexBufferId);
	OpenGL->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
	OpenGL->glBufferData(GL_ARRAY_BUFFER, m_vertexCount * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	OpenGL->glEnableVertexAttribArray(0); //Position
	OpenGL->glEnableVertexAttribArray(1); // Texcoord

	OpenGL->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
	OpenGL->glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), 0);
	OpenGL->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
	OpenGL->glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(Vertex), (unsigned char*)NULL + (3 * sizeof(float)));

	OpenGL->glGenBuffers(1, &m_indexBufferId);
	OpenGL->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId);
	OpenGL->glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	// Release the arrays now that the vertex and index buffers have been created and loaded
	delete[]vertices;
	vertices = 0;

	delete[]indices;
	indices = 0;

	// Inicializamos la matriz de mundo
	worldmatrix = new float[16] { 0.0f };
	OpenGL->BuildIdentityMatrix(worldmatrix);

	return true;
}

bool Billboard::SetShader(Shader* BillboardShader) {
	ShaderOfBillboard = BillboardShader;
	return true;
}

float* Billboard::GetWorldMatrix() {
	return this->worldmatrix;
}

bool Billboard::Render(float* viewMatrix, float* projectionMatrix, float x, float y, float z, float posCamX, float posCamZ) {
	// Rotación en dirección de la cámara
	float difz = posCamZ - z;
	float difx = posCamX - x;
	float dist = sqrtf(difz * difz + difx * difx);
	float angle = 0.0f;
	if (difz < 0) {
		angle = acos(difx / dist);
	}
	else {
		angle = -1.0 * acos(difx / dist);
	}

	float* auxMatrix = new float[16]{ 0.0f };
	OpenGL->BuildIdentityMatrix(auxMatrix);
	OpenGL->MatrixRotationY(worldmatrix, angle);
	OpenGL->MatrixTranslation(auxMatrix, x, y, z);
	OpenGL->MatrixMultiply(worldmatrix, worldmatrix, auxMatrix);

	// Seteo del Shader
	ShaderOfBillboard->SetShader();
	ShaderOfBillboard->PonMatriz4x4((char*)"worldMatrix", worldmatrix);
	ShaderOfBillboard->PonMatriz4x4((char*)"viewMatrix", viewMatrix);
	ShaderOfBillboard->PonMatriz4x4((char*)"projectionMatrix", projectionMatrix);
	ShaderOfBillboard->Pon1Entero((char*)"shaderTexture", TextureID);

	// Bind the vertex array object that stored all the information about the vertex and index buffers.
	OpenGL->glBindVertexArray(m_vertexArrayId);

	// Render the vertex buffer using the index buffer.
	glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);

	return true;
}

void Billboard::Shutdown() {
	OpenGL->glDisableVertexAttribArray(0);
	OpenGL->glDisableVertexAttribArray(1);

	OpenGL->glBindBuffer(GL_ARRAY_BUFFER, 0);
	OpenGL->glDeleteBuffers(1, &m_vertexBufferId);

	OpenGL->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	OpenGL->glDeleteBuffers(1, &m_indexBufferId);

	OpenGL->glBindVertexArray(0);
	OpenGL->glDeleteVertexArrays(1, &m_vertexArrayId);

	if (worldmatrix) {
		delete worldmatrix;
		worldmatrix = 0;
	}
	return;
}