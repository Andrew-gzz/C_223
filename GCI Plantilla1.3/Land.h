#ifndef _Land
#define _Land
#include "Geometrias.h"
#include "openglclass.h"
#include <gl/glu.h>
#include "TextureLoaderForOpenGL.h"

class Land: public Primitivos
{

private:
	float anchoFinal;
	float profunidadFinal;
	float deltax, deltaz;
	unsigned int m_vertexArrayId, m_vertexBufferId, m_indexBufferId;

	Shader* LandShader;

	struct Vertex {
		float Posx, Posy, Posz;
		float Normx, Normy, Normz;
		float u, v;
		float Tx, Ty, Tz;
		float Bx, By, Bz;
		float alterU, alterV;
	};

	Vertex* vertices;
	unsigned int* indices;

public:
	int verx, verz;
	//el nombre numerico de la indiceTextura1 en cuestion, por lo pronto una
	unsigned int planoTextura;
	unsigned int m_textureID;
	unsigned int m_textureID2;
	unsigned int m_textureID3;

	Land(HWND hWnd, OpenGLClass* OpenGL, TextureClass* LoaderTextureReference, const wchar_t* mapaDeAlturaHeightmap, 
		string indiceTextura1, string indiceTextura2, string indiceTextura3, float anchoDeseado, float profundidadDeseada)
	{
		anchoFinal = anchoDeseado;
		profunidadFinal = profundidadDeseada;
		// Cargamos el mapa de alturas(Heightmap) de la figura
		Carga(mapaDeAlturaHeightmap);
		//en caso del puntero de la imagen sea nulo se brinca esta opcion
		this->Plano(Ancho(), Alto(), anchoDeseado, profundidadDeseada, Dir_Imagen(), 80);
		deltax = anchoFinal / Ancho();
		deltaz = profunidadFinal / Alto();
		verx = Ancho();
		verz = Alto();
		// Disponemos la Textura1 del gdi.
		Descarga();

		// Aloja un objeto de vertices para OpenGL.
		OpenGL->glGenVertexArrays(1, &m_vertexArrayId);

		// Bind the vertex array object to store all the buffers and vertex attributes we create here.
		OpenGL->glBindVertexArray(m_vertexArrayId);

		// Generate an ID for the vertex buffer.
		OpenGL->glGenBuffers(1, &m_vertexBufferId);

		// Bind the vertex buffer and load the vertex (position, texture, and normal) data into the vertex buffer.
		OpenGL->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
		OpenGL->glBufferData(GL_ARRAY_BUFFER, cantVert * sizeof(Vertex), vertices, GL_STATIC_DRAW);

		// Enable the three vertex array attributes.
		OpenGL->glEnableVertexAttribArray(0);  // Vertex position.
		OpenGL->glEnableVertexAttribArray(1);  // Normals.
		OpenGL->glEnableVertexAttribArray(2);  // Texture coordinates.
		OpenGL->glEnableVertexAttribArray(3);  // Tangent.
		OpenGL->glEnableVertexAttribArray(4);  // BiNormals.
		OpenGL->glEnableVertexAttribArray(5);  // Texture coordinates Alt.

		// Specify the location and format of the position portion of the vertex buffer.
		OpenGL->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
		OpenGL->glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), 0);

		// Specify the location and format of the normal vector portion of the vertex buffer.
		OpenGL->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
		OpenGL->glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertex), (unsigned char*)NULL + (3 * sizeof(float)));

		// Specify the location and format of the texture coordinate portion of the vertex buffer.
		OpenGL->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
		OpenGL->glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(Vertex), (unsigned char*)NULL + (6 * sizeof(float)));

		// Specify the location and format of the tangent portion of the vertex buffer.
		OpenGL->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
		OpenGL->glVertexAttribPointer(3, 3, GL_FLOAT, false, sizeof(Vertex), (unsigned char*)NULL + (8 * sizeof(float)));

		// Specify the location and format of the binormal portion of the vertex buffer.
		OpenGL->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
		OpenGL->glVertexAttribPointer(4, 3, GL_FLOAT, false, sizeof(Vertex), (unsigned char*)NULL + (11 * sizeof(float)));

		// Specify the location and format of the texture coordinate alternative portion of the vertex buffer.
		OpenGL->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
		OpenGL->glVertexAttribPointer(5, 2, GL_FLOAT, false, sizeof(Vertex), (unsigned char*)NULL + (14 * sizeof(float)));

		// Generate an ID for the index buffer.
		OpenGL->glGenBuffers(1, &m_indexBufferId);

		// Bind the index buffer and load the index data into it.
		OpenGL->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId);
		OpenGL->glBufferData(GL_ELEMENT_ARRAY_BUFFER, cantIndices * sizeof(unsigned int), indices, GL_STATIC_DRAW);

		m_textureID = LoaderTextureReference->Initialize(indiceTextura1, true);

		m_textureID2 = LoaderTextureReference->Initialize(indiceTextura2, true);

		m_textureID3 = LoaderTextureReference->Initialize(indiceTextura3, true);
	}

	Land()
	{}

	~Land()
	{
		// Now that the buffers have been loaded we can release the array data.
		if (vertices) {
			delete[] vertices;
			vertices = 0;
		}
		if (indices) {
			delete[] indices;
			indices = 0;
		}
		//nos aseguramos de disponer de los recursos previamente reservados			
		glDeleteTextures(1, &planoTextura);
	}
	
	void Shutdown() {

	}

	float Superficie(float x, float z)
	{
		//obtenemos el indice pero podria incluir una fraccion
		float indicefx = (x + anchoFinal / 2) / deltax;
		float indicefz = (z + profunidadFinal / 2) / deltaz;
		//nos quedamos con solo la parte entera del indice
		int indiceix = (int)indicefx;
		int indiceiz = (int)indicefz;
		//nos quedamos con solo la fraccion del indice
		float difx = indicefx - indiceix;
		float difz = indicefz - indiceiz;

		float altura;
		float D;

		//el cuadro del land esta formado por dos triangulos, si difx es mayor que dify 
		//entonces estamos en el triangulo de abajo en caso contrario arriba
		if (difx > difz)
		{
			//obtenemos el vector 1 de dos que se necesitan
			VectorRR v1(vertices[indiceix + 1 + (indiceiz + 1) * verx].Posx - vertices[indiceix + indiceiz * verx].Posx,
				vertices[indiceix + 1 + (indiceiz + 1) * verx].Posy - vertices[indiceix + indiceiz * verx].Posy,
				vertices[indiceix + 1 + (indiceiz + 1) * verx].Posz - vertices[indiceix + indiceiz * verx].Posz);
			//obtenemos el vector 2 de dos
			VectorRR v2(vertices[indiceix + 1 + indiceiz * verx].Posx - vertices[indiceix + indiceiz * verx].Posx,
				vertices[indiceix + 1 + indiceiz * verx].Posy - vertices[indiceix + indiceiz * verx].Posy,
				vertices[indiceix + 1 + indiceiz * verx].Posz - vertices[indiceix + indiceiz * verx].Posz);

			//con el producto punto obtenemos la normal y podremos obtener la ecuacion del plano
			//la parte x de la normal nos da A, la parte y nos da B y la parte z nos da C
			VectorRR normalPlano = Cruz(v1, v2);
			//entonces solo falta calcular D
			D = -1 * (normalPlano.X * vertices[indiceix + indiceiz * verx].Posx +
				normalPlano.Y * vertices[indiceix + indiceiz * verx].Posy +
				normalPlano.Z * vertices[indiceix + indiceiz * verx].Posz);
			//sustituyendo obtenemos la altura de contacto en el land
			altura = ((-normalPlano.X * x - normalPlano.Z * z - D) / normalPlano.Y);
		}
		else
		{
			VectorRR v1(vertices[indiceix + (indiceiz + 1) * verx].Posx - vertices[indiceix + indiceiz * verx].Posx,
				vertices[indiceix + (indiceiz + 1) * verx].Posy - vertices[indiceix + indiceiz * verx].Posy,
				vertices[indiceix + (indiceiz + 1) * verx].Posz - vertices[indiceix + indiceiz * verx].Posz);

			VectorRR v2(vertices[indiceix + 1 + (indiceiz + 1) * verx].Posx - vertices[indiceix + indiceiz * verx].Posx,
				vertices[indiceix + 1 + (indiceiz + 1)* verx].Posy - vertices[indiceix + indiceiz * verx].Posy,
				vertices[indiceix + 1 + (indiceiz + 1)* verx].Posz - vertices[indiceix + indiceiz * verx].Posz);


			VectorRR normalPlano = Cruz(v1, v2);

			D = -1 * (normalPlano.X * vertices[indiceix + indiceiz * verx].Posx +
				normalPlano.Y * vertices[indiceix + indiceiz * verx].Posy +
				normalPlano.Z * vertices[indiceix + indiceiz * verx].Posz);

			altura = ((-normalPlano.X * x - normalPlano.Z * z - D) / normalPlano.Y);
		}

		return altura;

	}

	///////////////////////////////////////////////////////////////////////////////////////////
	   

	void Shutdown(OpenGLClass* OpenGL)
	{

		// Release the vertex and index buffers.
		ShutdownBuffers(OpenGL);

		return;
	}


	void Render(OpenGLClass* OpenGL)
	{
		// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
		RenderBuffers(OpenGL);

		return;
	}

	void Render(OpenGLClass* OpenGL, float* worldMatrix, float* viewMatrix, float* projectionMatrix) {
		// Seleccionar el shader para el dibujo del terreno
		LandShader->SetShader();
		LandShader->PonMatriz4x4((char*)"worldMatrix", worldMatrix);
		LandShader->PonMatriz4x4((char*)"viewMatrix", viewMatrix);
		LandShader->PonMatriz4x4((char*)"projectionMatrix", projectionMatrix);

		LandShader->Pon1Entero((char*)"shaderTexture", m_textureID);
		LandShader->Pon1Entero((char*)"shaderTexture2", m_textureID2);
		LandShader->Pon1Entero((char*)"shaderTexture3", m_textureID3);

		// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
		RenderBuffers(OpenGL);

		return;
	}

	void ShutdownBuffers(OpenGLClass* OpenGL)
	{
		// Disable the two vertex array attributes.
		OpenGL->glDisableVertexAttribArray(0);
		OpenGL->glDisableVertexAttribArray(1);

		// Release the vertex buffer.
		OpenGL->glBindBuffer(GL_ARRAY_BUFFER, 0);
		OpenGL->glDeleteBuffers(1, &m_vertexBufferId);

		// Release the index buffer.
		OpenGL->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		OpenGL->glDeleteBuffers(1, &m_indexBufferId);

		// Release the vertex array object.
		OpenGL->glBindVertexArray(0);
		OpenGL->glDeleteVertexArrays(1, &m_vertexArrayId);

		return;
	}
	
	void RenderBuffers(OpenGLClass* OpenGL)
	{
		// Bind the vertex array object that stored all the information about the vertex and index buffers.
		OpenGL->glBindVertexArray(m_vertexArrayId);

		// Render the vertex buffer using the index buffer.
		glDrawElements(GL_TRIANGLES, cantIndices, GL_UNSIGNED_INT, 0);

		return;
	}	

	void SetLandShader(Shader* LandShader) {
		this->LandShader = LandShader;
	}

	Shader* GetLandShader() {
		return this->LandShader;
	}

	void Plano(int vertx, int vertz, float anchof, float profz, unsigned char* altura, float tile = 1)
	{
		//Cargamos la estructura con los espacios de memoria necesarios
		vertices = new Vertex[vertx * vertz];
		indices = new unsigned int[(vertx - 1) * (vertz - 1) * 6];

		cantVert = vertx * vertz;
		cantIndices = (vertx - 1) * (vertz - 1) * 6;

		//es la separacion entre vertices, se le resta 1 para que el lado correcto
		//imagine que el ancho es de 10 y tiene 10 vertices, entonces le daria un deltax
		//de 1, si los vertices van de 0 a 9 entonces la posicion del ultimo vertice
		//seria 9, si le divide entre vertx -1 le dara 1.1111, y el ultimo vertice sera 10
		float deltax = anchof / (vertx - 1);
		float deltaz = profz / (vertz - 1);

		//crea los vertices
		for (int z = 0; z < vertz; z++)
		{
			for (int x = 0; x < vertx; x++)
			{
				vertices[z * vertx + x].Posx = (float)x * deltax - anchof / 2.0;
				vertices[z * vertx + x].Posy = (float)altura[z * vertx * 4 + x * 4] / 10.0;
				vertices[z * vertx + x].Posz = (float)z * deltaz - profz / 2.0;

				//carga las normales con cero
				vertices[z * vertx + x].Normx = 0.0;
				vertices[z * vertx + x].Normy = 0.0;
				vertices[z * vertx + x].Normz = 0.0;

				//carga las tangent con cero
				vertices[z * vertx + x].Tx = 0.0;
				vertices[z * vertx + x].Ty = 0.0;
				vertices[z * vertx + x].Tz = 0.0;

				//carga las binormales con cero
				vertices[z * vertx + x].Bx = 0.0;
				vertices[z * vertx + x].By = 0.0;
				vertices[z * vertx + x].Bz = 0.0;
			}
		}

		//calcula los uv's
		for (int z = 0; z < vertz; z++)
		{
			for (int x = 0; x < vertx; x++)
			{
				vertices[z * vertx + x].u = (float)(x * tile) / (vertx - 1);
				vertices[z * vertx + x].v = (float)(z * tile) / (vertz - 1);
			}
		}

		float uStep = 1.0f / vertx;
		float vStep = 1.0f / vertz;

		for (int z = 0; z < vertz; z++)
		{
			for (int x = 0; x < vertx; x++)
			{
				vertices[z * vertx + x].alterU = (float)x * uStep;
				vertices[z * vertx + x].alterV = (float)z * vStep;
			}
		}

		VectorRR aux;
		//crea las normales
		for (int z = 0; z < (vertz - 1); z++)
		{
			for (int x = 0; x < (vertx - 1); x++)
			{
				Copia(aux, GenNormal(&vertices[z * vertx + x].Posx, &vertices[z * vertx + (x + 1)].Posx,
					&vertices[(z + 1) * vertx + (x + 1)].Posx));

				SumaNormal(&vertices[z * vertx + x].Normx, &aux.X);
				SumaNormal(&vertices[(z + 1) * vertx + (x + 1)].Normx, &aux.X);
				SumaNormal(&vertices[z * vertx + (x + 1)].Normx, &aux.X);

				Copia(aux, GenNormal(&vertices[z * vertx + x].Posx, &vertices[(z + 1) * vertx + x + 1].Posx,
					&vertices[(z + 1) * vertx + x].Posx));

				SumaNormal(&vertices[z * vertx + x].Normx, &aux.X);
				SumaNormal(&vertices[(z + 1) * vertx + x].Normx, &aux.X);
				SumaNormal(&vertices[(z + 1) * vertx + (x + 1)].Normx, &aux.X);
			}
		}

		VectorRR tangente;
		//crea las  tangentes
		for (int z = 0; z < (vertz - 1); z++)
		{
			for (int x = 0; x < (vertx - 1); x++)
			{
				/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				// primer triangulo
				/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				tangente.X = vertices[z * vertx + (x + 1)].Posx - vertices[z * vertx + x].Posx;
				tangente.Y = vertices[z * vertx + (x + 1)].Posy - vertices[z * vertx + x].Posy;
				tangente.Z = vertices[z * vertx + (x + 1)].Posz - vertices[z * vertx + x].Posz;

				SumaNormal(&vertices[z * vertx + x].Tx, &tangente.X);
				SumaNormal(&vertices[(z + 1) * vertx + (x + 1)].Tx, &tangente.X);
				SumaNormal(&vertices[z * vertx + (x + 1)].Tx, &tangente.X);

				/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				// segundo triangulo
				/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				tangente.X = vertices[(z + 1) * vertx + (x + 1)].Posx - vertices[z * vertx + x].Posx;
				tangente.Y = vertices[(z + 1) * vertx + (x + 1)].Posy - vertices[z * vertx + x].Posy;
				tangente.Z = vertices[(z + 1) * vertx + (x + 1)].Posz - vertices[z * vertx + x].Posz;

				SumaNormal(&vertices[z * vertx + x].Tx, &tangente.X);
				SumaNormal(&vertices[(z + 1) * vertx + x].Tx, &tangente.X);
				SumaNormal(&vertices[(z + 1) * vertx + (x + 1)].Tx, &tangente.X);
			}
		}

		//Normaliza las normales
		for (int z = 0; z < vertz; z++)
		{
			for (int x = 0; x < vertx; x++)
			{
				Normaliza(&vertices[z * vertx + x].Normx);
				Normaliza(&vertices[z * vertx + x].Tx);
			}
		}

		VectorRR bitangente, tan, norm;
		//crea las  tangentes
		for (int z = 0; z < (vertz - 1); z++)
		{
			for (int x = 0; x < (vertx - 1); x++)
			{
				bitangente.X = -(vertices[z * vertx + x].Ty * vertices[z * vertx + x].Normz - vertices[z * vertx + x].Tz *
					vertices[z * vertx + x].Normy);
				bitangente.Y = -(vertices[z * vertx + x].Tz * vertices[z * vertx + x].Normx - vertices[z * vertx + x].Tx *
					vertices[z * vertx + x].Normz);
				bitangente.Z = -(vertices[z * vertx + x].Tx * vertices[z * vertx + x].Normy - vertices[z * vertx + x].Ty *
					vertices[z * vertx + x].Normx);

				SumaNormal(&vertices[z * vertx + x].Bx, &bitangente.X);
			}
		}

		//Normaliza las normales
		for (int z = 0; z < vertz; z++)
		{
			for (int x = 0; x < vertx; x++)
			{
				Normaliza(&vertices[z * vertx + x].Bx);
			}
		}

		//ahora la parte mas importante de crear vertices es el algoritmo para unirlos, en este caso sustituiremos
		//a un algoritmo con un un grupo de indices
		int indice = 0;
		for (int i = 0; i < vertz - 1; i++)
		{
			for (int j = 0; j < vertx - 1; j++)
			{
				indices[indice++] = i * vertz + j;
				indices[indice++] = (i + 1) * vertz + j + 1;
				indices[indice++] = i * vertz + j + 1;

				indices[indice++] = i * vertz + j;
				indices[indice++] = (i + 1) * vertz + j;
				indices[indice++] = (i + 1) * vertz + j + 1;
			}
		}
	}
};

#endif 