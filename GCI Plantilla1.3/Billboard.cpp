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
	vertices[0].u = uI;     // Mant�n en 0 (izquierda)
	vertices[0].v = 1;     // Mant�n en 1 (parte inferior)

	vertices[1].posx = 0;
	vertices[1].posy = 2 * escala;
	vertices[1].posz = -1 * escala;
	vertices[1].u = uI;     // Mant�n en 0 (izquierda)
	vertices[1].v = 0;     // Mant�n en 0 (parte superior)

	vertices[2].posx = 0;
	vertices[2].posy = 2 * escala;
	vertices[2].posz = 1 * escala;
	vertices[2].u = uEn;   // Cambia a 0.5 para el borde derecho de los primeros 96 p�xeles
	vertices[2].v = 0;     // Mant�n en 0 (parte superior)

	vertices[3].posx = 0;
	vertices[3].posy = 0;
	vertices[3].posz = 1 * escala;
	vertices[3].u = uEn;   // Cambia a 0.5 para el borde derecho de los primeros 96 p�xeles
	vertices[3].v = 1;     // Mant�n en 1 (parte inferior)

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

void Billboard::DrawImagen(GLdouble PAncho, GLdouble PAlto) {

	// Configuraci�n inicial
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, PAncho, 0, PAlto, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	// Desactivar prueba de profundidad
	glDisable(GL_DEPTH_TEST);

	// Habilitar mezcla para transparencia
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Configuraci�n de dimensiones de la textura
	GLfloat BillboardAncho = 480.0f;
	GLfloat BillboardAlto = 384.0f;
	GLfloat BillboarX = (PAncho - BillboardAncho) / 2.0f;
	GLfloat BillboarY = (PAlto - BillboardAlto) / 2.0f;

	// Dibujo del cuadrado texturizado
	glBindTexture(GL_TEXTURE_2D, TextureID);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1); glVertex2f(BillboarX, BillboarY);
	glTexCoord2f(1, 1); glVertex2f(BillboarX + BillboardAncho, BillboarY);
	glTexCoord2f(1, 0); glVertex2f(BillboarX + BillboardAncho, BillboarY + BillboardAlto);
	glTexCoord2f(0, 0); glVertex2f(BillboarX, BillboarY + BillboardAlto);
	glEnd();

	// Restauraci�n del estado
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

}

bool Billboard::SetShader(Shader* BillboardShader) {
	ShaderOfBillboard = BillboardShader;
	return true;
}

float* Billboard::GetWorldMatrix() {
	return this->worldmatrix;
}

bool Billboard::Render(float* viewMatrix, float* projectionMatrix, float x, float y, float z, float posCamX, float posCamZ) {
	// Rotaci�n en direcci�n de la c�mara
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