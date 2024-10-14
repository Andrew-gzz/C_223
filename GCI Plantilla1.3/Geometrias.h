#ifndef _Geom
#define _Geom
#define _USE_MATH_DEFINES
#include <math.h>
#include "VectorRR.h"
//#include "TextureLoaderForOpenGL.h"
#include "Imagenes.h"

//estructura para manejar primitivos con posicion, normal y uv's
struct Vertices {
	float Posx, Posy, Posz;
	float Normx, Normy, Normz;
	float u, v;
	float Tx, Ty, Tz;
	float Bx, By, Bz;
	float alterU, alterV;
};

//En honor a nuestros ancestros llamaremos "Maya" a la malla
//estructura que contiene datos de los vertices y sus indices
struct Maya {
	Vertices *maya;
	unsigned int *indices;
};




class Primitivos: public VectorRR, public Imagenes
{
public:
	int cantVert;
	int cantIndices;
public:

	Primitivos() //mis pensamientos!
	{
		cantVert = 0;
		cantIndices = 0;
	}

	//generamos los vertices a traves de coordenadas esfericas
	//conocimiento adquirido en la materia de matematicas para videjuegos I
	Maya Esfera(int stacks, int slices, float radio, float inicio, float final)
	{
		//Cargamos la estructura con los espacios de memoria necesarios
		Vertices *vertices = new Vertices[stacks*slices * 3];
		unsigned int *indices = new unsigned int[stacks*slices * 6];
		//generamos un objeto para poder transportar los punteros
		Maya salida;
		//a darle que es mole de olla!
		for (int i = 0; i < slices; i++)
		{
			for (int j = 0; j < stacks; j++)
			{
				int indice = (i * stacks + j);
				vertices[indice].Posx = radio * cos(((double)j / (stacks - 1))*(M_PI*(final - inicio)) + M_PI * inicio - M_PI / 2.0)*
					cos(2.0*M_PI*(double)i / (slices - 1));
				vertices[indice].Posy = radio * sin(((double)j / (stacks - 1))*(M_PI*(final - inicio)) + M_PI * inicio - M_PI / 2.0);
				vertices[indice].Posz = radio * cos(((double)j / (stacks - 1))*(M_PI*(final - inicio)) + M_PI * inicio - M_PI / 2.0)*
					sin(2.0*M_PI*(double)i / (slices - 1));

				vertices[indice].Normx = cos(((double)j / (stacks - 1))*(M_PI*(final - inicio)) + M_PI * inicio - M_PI / 2.0)*
					cos(2.0*M_PI*(double)i / (slices - 1));
				vertices[indice].Normy = sin(((double)j / (stacks - 1))*(M_PI*(final - inicio)) + M_PI * inicio - M_PI / 2.0);
				vertices[indice].Normz = cos(((double)j / (stacks - 1))*(M_PI*(final - inicio)) + M_PI * inicio - M_PI / 2.0)*
					sin(2.0*M_PI*(double)i / (slices - 1));

				vertices[indice].u = (float)1 * (1 - (float)i / (stacks - 1));
				vertices[indice].v = (float)1 * (1 - (float)j / (slices - 1));
			}
		}

		//ahora la parte mas importante de crear vertices es el algoritmo para unirlos, en este caso sustituiremos
		//a un algoritmo con un un grupo de indices
		int indice = 0;
		for (int i = 0; i < slices - 1; i++)
		{
			for (int j = 0; j < stacks - 1; j++)
			{
				indices[indice++] = i * stacks + j;
				indices[indice++] = (i + 1) * stacks + j + 1;
				indices[indice++] = i * stacks + j + 1;

				indices[indice++] = i * stacks + j;
				indices[indice++] = (i + 1) * stacks + j;
				indices[indice++] = (i + 1) * stacks + j + 1;
			}
		}

		//una vez generados los damos a conocer a traves del objeto "salida"
		salida.maya = vertices;
		salida.indices = indices;

		return salida;
	}

	Maya Plano(int vertx, int vertz, float anchof, float profz)
	{
		//Cargamos la estructura con los espacios de memoria necesarios
		Vertices *vertices = new Vertices[vertx*vertz];
		unsigned int *indices = new unsigned int[(vertx - 1)*(vertz - 1) * 6];
		cantVert = vertx * vertz;
		cantIndices = (vertx - 1)*(vertz - 1) * 6;
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
				vertices[z*vertx + x].Posx = (float)x*deltax;
				vertices[z*vertx + x].Posy = 0.0;
				vertices[z*vertx + x].Posz = (float)z*deltaz;

				//carga las normales con cero
				vertices[z*vertx + x].Normx = 0.0;
				vertices[z*vertx + x].Normy = 1.0;
				vertices[z*vertx + x].Normz = 0.0;
			}
		}

		//calcula los uv's
		for (int z = 0; z < vertz; z++)
		{
			for (int x = 0; x < vertx; x++)
			{
				vertices[z*vertx + x].u = (float)x / (vertx - 1);
				vertices[z*vertx + x].v = (float)z / (vertz - 1);
			}
		}

		VectorRR aux;
		//crea las normales
		for (int z = 0; z < (vertz - 1); z++)
		{
			for (int x = 0; x < (vertx - 1); x++)
			{
				Copia(aux, GenNormal(&vertices[z*vertx + x].Posx, &vertices[(z + 1)*vertx + (x + 1)].Posx,
					&vertices[z*vertx + (x + 1)].Posx));

				SumaNormal(&vertices[z*vertx + x].Normx, &aux.X);
				SumaNormal(&vertices[(z + 1)*vertx + (x + 1)].Normx, &aux.X);
				SumaNormal(&vertices[z*vertx + (x + 1)].Normx, &aux.X);

				Copia(aux, GenNormal(&vertices[z*vertx + x].Posx, &vertices[(z + 1)*vertx + x].Posx,
					&vertices[(z + 1)*vertx + (x + 1)].Posx));

				SumaNormal(&vertices[z*vertx + x].Normx, &aux.X);
				SumaNormal(&vertices[(z + 1)*vertx + x].Normx, &aux.X);
				SumaNormal(&vertices[(z + 1)*vertx + (x + 1)].Normx, &aux.X);
			}
		}

		//Normaliza las normales
		for (int z = 0; z < vertz; z++)
		{
			for (int x = 0; x < vertx; x++)
			{
				Normaliza(&vertices[z*vertx + x].Normx);
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

		//generamos un objeto para poder transportar los punteros

		Maya salida;

		salida.maya = vertices;
		salida.indices = indices;

		return salida;
	}


	Maya Plano(int vertx, int vertz, float anchof, float profz, unsigned char *altura, float tile = 1)
	{
		//Cargamos la estructura con los espacios de memoria necesarios
		Vertices *vertices = new Vertices[vertx*vertz];
		unsigned int *indices = new unsigned int[(vertx-1)*(vertz-1) * 6];
		cantVert = vertx * vertz;
		cantIndices = (vertx - 1)*(vertz - 1) * 6;
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
				vertices[z*vertx + x].Posx = (float)x*deltax - anchof / 2.0;
				vertices[z*vertx + x].Posy = (float)altura[z*vertx * 4 + x * 4] / 10.0;
				vertices[z*vertx + x].Posz = (float)z*deltaz - profz / 2.0;

				//carga las normales con cero
				vertices[z*vertx + x].Normx = 0.0;
				vertices[z*vertx + x].Normy = 0.0;
				vertices[z*vertx + x].Normz = 0.0;

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
				vertices[z*vertx + x].u =  (float)(x*tile) / (vertx - 1);
				vertices[z*vertx + x].v =  (float)(z*tile) / (vertz - 1);
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
				Copia(aux, GenNormal(&vertices[z*vertx + x].Posx, &vertices[z*vertx + (x + 1)].Posx,
					&vertices[(z + 1)*vertx + (x + 1)].Posx));

				SumaNormal(&vertices[z*vertx + x].Normx, &aux.X);
				SumaNormal(&vertices[(z + 1)*vertx + (x + 1)].Normx, &aux.X);
				SumaNormal(&vertices[z*vertx + (x + 1)].Normx, &aux.X);

				Copia(aux, GenNormal(&vertices[z*vertx + x].Posx, &vertices[(z + 1)*vertx + x + 1].Posx,
					&vertices[(z + 1)*vertx + x].Posx));

				SumaNormal(&vertices[z*vertx + x].Normx, &aux.X);
				SumaNormal(&vertices[(z + 1)*vertx + x].Normx, &aux.X);
				SumaNormal(&vertices[(z + 1)*vertx + (x + 1)].Normx, &aux.X);
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

		VectorRR bitangente, tan , norm;
		//crea las  tangentes
		for (int z = 0; z < (vertz - 1); z++)
		{
			for (int x = 0; x < (vertx - 1); x++)
			{				
				bitangente.X = -(vertices[z * vertx + x].Ty * vertices[z * vertx + x].Normz - vertices[z * vertx + x].Tz * 
					vertices[z * vertx + x].Normy);
				bitangente.Y = -(vertices[z * vertx + x].Tz * vertices[z * vertx + x].Normx- vertices[z * vertx + x].Tx * 
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

		//generamos un objeto salida para poder transportar los punteros

		Maya salida;

		salida.maya = vertices;
		salida.indices = indices;

		return salida;
	}
};
#endif