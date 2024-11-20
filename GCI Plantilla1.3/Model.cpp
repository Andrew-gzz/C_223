/*
File name Model.cpp
*/
#include "Model.h"

Model::Model(){
	this->OpenGL = NULL;
	HandlerWindow = NULL;
	MaxTexturesPerModel = 0;
	TextureID[0] = 0;
	TextureID[1] = 0;
	TextureID[2] = 0;
	TextureID[3] = 0;
	TextureID[4] = 0;

	box = NULL;
	box2 = NULL;
}

Model::Model(OpenGLClass* OpenGL, HWND hwnd, TextureClass* LoaderTexRef){
	this->OpenGL = OpenGL;
	HandlerWindow = hwnd;
	LoaderTextureReference = LoaderTexRef;
	MaxTexturesPerModel = 0;
	TextureID[0] = 0;
	TextureID[1] = 0;
	TextureID[2] = 0;
	TextureID[3] = 0;
	TextureID[4] = 0;
}

Model::Model(const Model&)
{
	this->OpenGL = NULL;
	HandlerWindow = NULL;
	MaxTexturesPerModel = 0;
	TextureID[0] = 0;
	TextureID[1] = 0;
	TextureID[2] = 0;
	TextureID[3] = 0;
	TextureID[4] = 0;
}

Model::~Model()
{

}

bool Model::Initialize(const char* objPath, string texturePath, 
	string normalMappingPath) {
	bool result;
	min = new GLFLOAT3(0.0f, 0.0f, 0.0f);
	max = new GLFLOAT3(0.0f, 0.0f, 0.0f);
	//load buffer Model;
	result = LoadModel(objPath);
	if (!result) {
		return false;
	}
	//Get Tangent, Binormal
	CalculateModelVectors();
	
	//Load Vertex buffer and index buffer
	result = InitializeBuffers(OpenGL);
	if (!result)
	{
		return false;
	}
	if (texturePath != "") {
		TextureID[MaxTexturesPerModel] = LoaderTextureReference->Initialize(texturePath, true);
		MaxTexturesPerModel++;
	}
	if (normalMappingPath != "") {
		TextureID[MaxTexturesPerModel] = LoaderTextureReference->Initialize(normalMappingPath, true);
		MaxTexturesPerModel++;
	}
}

bool Model::Initialize(const char* objPath, string texturePath) {
	bool result;
	min = new GLFLOAT3(0.0f, 0.0f, 0.0f);
	max = new GLFLOAT3(0.0f, 0.0f, 0.0f);

	//load buffer Model;
	result = LoadModel(objPath);
	if (!result) {
		return false;
	}
	//Get Tangent, Binormal
	CalculateModelVectors();

	//Load Vertex buffer and index buffer
	result = InitializeBuffers(OpenGL);
	if (!result)
	{
		return false;
	}

	if (texturePath != "") {
		TextureID[MaxTexturesPerModel] = LoaderTextureReference->Initialize(texturePath,true);
		MaxTexturesPerModel++;
	}

	// Inicialización de nuestra Bounding Box
	box = new BoundingBox();
	if (!box) {
		return false;
	}
	box2 = new BoundingBox();
	if (!box2) {
		return false;
	}

	BoundingBox::GLFLOAT3 maxBox;
	BoundingBox::GLFLOAT3 minBox;

	maxBox.x = max->x;
	maxBox.y = max->y;
	maxBox.z = max->z;

	minBox.x = min->x;
	minBox.y = min->y;
	minBox.z = min->z;

	BoundingBox::GLCOLOR color;
	color.r = 1.0;
	color.g = 1.0;
	color.b = 1.0;
	color.a = 1.0;

	result = box->Initialize(OpenGL, maxBox, minBox, color);
	if (!result) {
		return false;
	}
	result = box2->Initialize(OpenGL, maxBox, minBox, color);
	if (!result) {
		return false;
	}

	return true;
}

void Model::ChangeTexture(int From, int To) {
	int temp = TextureID[0];
	int temp2 = TextureID[1];
	TextureID[0] = From;
	TextureID[1] = To;
}

bool Model::InitializeBuffers(OpenGLClass* OpenGL) {
	VertexType* vertices;
	unsigned long* indices;
	HRESULT result;
	int i;


	//Create the vertex array
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
		return false;

	//Create the index array
	indices = new unsigned long[m_indexCount];
	if (!indices)
		return false;

	//Load the vertex and index array with data
	for (i = 0; i < m_vertexCount; i++)
	{

		vertices[i].Posx = m_model[i].x;
		vertices[i].Posy = m_model[i].y;
		vertices[i].Posz = m_model[i].z;

		vertices[i].u = m_model[i].tu;
		vertices[i].v = m_model[i].tv;

		vertices[i].Normx = m_model[i].nx;
		vertices[i].Normy = m_model[i].ny;
		vertices[i].Normz = m_model[i].nz;

		vertices[i].Tx = m_model[i].tx;
		vertices[i].Ty = m_model[i].ty;
		vertices[i].Tz= m_model[i].tz;

		vertices[i].Bx = m_model[i].bx;
		vertices[i].By = m_model[i].by;
		vertices[i].Bz= m_model[i].bz;

		indices[i] = i;
	}


	// m_vertexArrayId es el objeto de vertices de arreglo (vertex array object)
	// contiene informacion de nuestro vertex buffer

	//glGenVertexArrays(cuantos objetos vamos a generar,pasamos la direccion de memoria de nuestro objeto)
	OpenGL->glGenVertexArrays(1, &m_vertexArrayId);
	//glBindVertexArray enlaza el objeto vertices
	OpenGL->glBindVertexArray(m_vertexArrayId);

	//m_vertexBufferId es nuestro objeto de buffer de vertices, es el ID 
	// glGenBuffers(cuantos vertex buffer se van a generar,pasamos la direccion de memoria del vertex buffer)
	OpenGL->glGenBuffers(1, &m_vertexBufferId);

	// Enlazar el ID vertex buffer objeto
	OpenGL->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
	//glBufferData especifica que tipo de buffer se va enlazar
	//GL_ARRAY_BUFFER  = vertex buffer
	//m_vertexCount * sizeof(VertexType) tamaño en bytes del buffer
	//vertices nuestro arreglo de vertices (posicion,normal,uv,tangent,binormal)
	/*
	GL_STATIC_DRAW 
	Especifica el patron que se usará el buffer
	en este caso nuestro vertex buffer es una constante que solo se editara una vez para reutilizarlo muchas veces.
	*/
	OpenGL->glBufferData(GL_ARRAY_BUFFER, m_vertexCount * sizeof(VertexType), vertices, GL_STATIC_DRAW);

	//Para especificar los datos del vertex buffer hay que preparar los atributos (Arreglos de atributo)
	//Los AttribArray son atributos que se inicializan para pasar informacion de opengl hacia los shaders
	//Se le dice Array porque puedes declarar un arreglo de flotantes como atributo flotante de 2(vec2), flotante de 3 (vec3) ...
	//En los shaders son las variables declaradas con tipo de dato flotante. (vertices, matrices, etc.)
	// 
	//Aqui lo que queremos es habilitar 5 atributos de arreglo de flotantes
	OpenGL->glEnableVertexAttribArray(0);  // Vertex position. arreglo flotante de 3 (vec3)
	OpenGL->glEnableVertexAttribArray(1);  // Normals. (vec3)
	OpenGL->glEnableVertexAttribArray(2);  // Texture coordinates. (vec2)
	OpenGL->glEnableVertexAttribArray(3);  // Tangent. (vec3)
	OpenGL->glEnableVertexAttribArray(4);  // BiNormals. (vec3)

	OpenGL->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
	//glVertexAttribPointer describe el atributo y posiciona el puntero de la estructura

	//Posicion Posx, Posy, Posz
	//glVertexAttribPointer( 0  indice del atributo,  3 cantidad de componentes (3 float), false no se normaliza, sizeof(VertexType) tamaño del vertice, 0 es nuestro puntero )
	OpenGL->glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(VertexType), 0); // <--- 0 es el puntero de los tres float de posicion

	OpenGL->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
	//Normal indice 1  Normx, Normy, Normz
	OpenGL->glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(VertexType), (unsigned char*)NULL + (3 * sizeof(float))); // <--- 3 es el puntero para llegar a las normales

	OpenGL->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
	//Uv indice 2  u,v
	OpenGL->glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(VertexType), (unsigned char*)NULL + (6 * sizeof(float)));

	OpenGL->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
	//Tangent indice 3  Tx, Ty, Tz
	OpenGL->glVertexAttribPointer(3, 3, GL_FLOAT, false, sizeof(VertexType), (unsigned char*)NULL + (8 * sizeof(float)));

	OpenGL->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
	//Binormal indice 3  Bx, By, Bz
	OpenGL->glVertexAttribPointer(4, 3, GL_FLOAT, false, sizeof(VertexType), (unsigned char*)NULL + (11 * sizeof(float)));

	// Generate an ID for the index buffer.
	OpenGL->glGenBuffers(1, &m_indexBufferId);

	// Bind the index buffer and load the index data into it.
	OpenGL->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId);
	OpenGL->glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexCount * sizeof(unsigned int),
		indices, GL_STATIC_DRAW);
	

	//Release the arrays now that the vertex and index buffers have been created and loaded
	delete[]vertices;
	vertices = 0;

	delete[]indices;
	indices = 0;
	return true;
}

bool Model::AddTexture(string texturePath, int TextureNumber) {
	try{
		TextureID[MaxTexturesPerModel] = LoaderTextureReference->Initialize(texturePath, true);
		MaxTexturesPerModel++;
		return true;
	}
	catch (exception e) {
		printf("Error,&s", e);
		return false;
	}
}

bool Model::Frame()
{
	return true;
}
bool Model::Render(Shader* shader) {
	if (MaxTexturesPerModel == 1) {
		shader->Pon1Entero((char*)"shaderTexture", TextureID[0]);
	}
	else if (MaxTexturesPerModel == 2) {
		shader->Pon1Entero((char*)"shaderTexture", TextureID[0]);
		shader->Pon1Entero((char*)"shaderTexture2", TextureID[1]);
	}
	else if (MaxTexturesPerModel == 3) {
		shader->Pon1Entero((char*)"shaderTexture", TextureID[0]);
		shader->Pon1Entero((char*)"shaderTexture2", TextureID[1]);
		shader->Pon1Entero((char*)"shaderTexture3", TextureID[2]);
	}
	else if (MaxTexturesPerModel == 4) {
		shader->Pon1Entero((char*)"shaderTexture", TextureID[0]);
		shader->Pon1Entero((char*)"shaderTexture2", TextureID[1]);
		shader->Pon1Entero((char*)"shaderTexture3", TextureID[2]);
		shader->Pon1Entero((char*)"shaderTexture4", TextureID[3]);
	}
	// Bind the vertex array object that stored all the information about the vertex and index buffers.
	OpenGL->glBindVertexArray(m_vertexArrayId);

	// Render the vertex buffer using the index buffer.
	glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
	return true;
}
bool Model::LoadModel(const char* fileModel)
{
	bool result;
	string file_local = fileModel;
	//Check type file
	if (file_local.substr(file_local.find_last_of(".") + 1) == "obj")
		result = ReadFileObj(fileModel);
	else
		result = ReadFileTxt(fileModel);
	//Check message Error
	if (!result)
	{
		return false;
	}
	return true;
}
int Model::GetIndexCount()
{
	return m_indexCount;
}
//Read vertex, textCoord and normals of File Obj
bool Model::ReadFileObj(const char* FileObj)
{
	bool result;
	result = First_LoadConvertObjModelsToBuffers(FileObj);
	if (!result)
	{
		false;
	}
	return true;
}

//Read vertex, textCoord and normals of File TXt
bool Model::ReadFileTxt(const char* FileTxt)
{
	ifstream fin;
	char input;
	int i;

	//Open the model file
	fin.open(FileTxt);

	//If it couldn't open then exit
	if (fin.fail())
		return false;

	//Read up to the value of vertex count
	fin.get(input);
	while (input != ':')
		fin.get(input);

	//Read in the vertex count
	fin >> m_vertexCount;

	//Set the number of indices to be the same as the vertex count
	m_indexCount = m_vertexCount;

	//create the model using the vertex count that was read in
	m_model = new ModelType[m_vertexCount];
	if (!m_model)
		return false;

	//Read up the beginning of the data
	fin.get(input);
	while (input != ':')
		fin.get(input);
	fin.get(input);
	fin.get(input);

	//read in the vertex data
	for (i = 0; i < m_vertexCount; i++) {

		fin >> m_model[i].x >> m_model[i].y >> m_model[i].z;
		fin >> m_model[i].tu >> m_model[i].tv;
		fin >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;
	}

	//Close the model file
	fin.close();

	for (int i = 0; i < m_vertexCount; i++)
	{
		//colisiones cuadradas
		if (max->x <= m_model[i].x)
			max->x = m_model[i].x;
		else if (min->x >= m_model[i].x)
			min->x = m_model[i].x;
		if (max->y <= m_model[i].y)
			max->y = m_model[i].y;
		else if (min->y >= m_model[i].y)
			min->y = m_model[i].y;
		if (max->z <= m_model[i].z)
			max->z = m_model[i].z;
		else if (min->z >= m_model[i].z)
			min->z = m_model[i].z;
	}

	return true;
}
void Model::ReleaseModel()
{
	if (m_model)
	{
		delete[] m_model;
		m_model = 0;
	}
	return;
}

#pragma region _LOADOBJ_
//Toma un .obj para luego cargarlo en Ram
bool Model::First_LoadConvertObjModelsToBuffers(const char* filenameModel)
{
	bool result;
	int vertexCount, textureCount, normalCount, faceCount;

	// Lee dentro del archivo y comprueba que existe.
	result = Second_RectifyModelFile(filenameModel);
	if (!result)
		return false;

	// Read in the number of vertices, tex coords, normals, and faces so that the data structures can be initialized with the exact sizes needed.
	result = Third_LoadInRamModelFile(filenameModel, vertexCount, textureCount, normalCount, faceCount);
	if (!result)
		return false;

	// Display the counts to the console for information purposes.
	_RPT1(0, "Vertices: %d\n", vertexCount);
	_RPT1(0, "UVs: %d\n", textureCount);
	_RPT1(0, "Normals: %d\n", normalCount);
	_RPT1(0, "Faces: %d\n", faceCount);

	// Now read the data from the file into the data structures and then output it in our model format.
	if (!result)
		return false;

	// Notify the user the model has been converted.
	_RPT1(0, "File has been converted. \n", 0);

	return true;
}

//Abre y rectifica que el modelo sea correcto y no este dañado
bool Model::Second_RectifyModelFile(const char* filename)
{
	bool done;
	ifstream fin;


	// Loop until we have a file name.
	done = false;
	while (!done)
	{
		// Attempt to open the file.
		fin.open(filename);

		if (fin.good())
		{
			// If the file exists and there are no problems then exit since we have the file name.
			done = true;
		}
		else
		{
			// If the file does not exist or there was an issue opening it then notify the user and repeat the process.
			fin.clear();
			_RPT1(0, "File: %s, couldn't found\n", filename);
			return false;
		}
	}

	return true;
}

//Carga en ram todo el archivo .obj
bool Model::Third_LoadInRamModelFile(const char* filename, int& vertexCount, int& textureCount, int& normalCount, int& faceCount)
{
	bool result;
	ifstream fin;
	char input, in;
	Model Model;
	vector<VertexLoad> array3D;
	vector<VertexLoad> InfoModel;
	int i = 0, j = 0, k = 0, f = 0, vIndex = 0, tIndex = 0, nIndex = 0, IndexAll = 1;

	// Initialize the counts.
	vertexCount = 0;
	textureCount = 0;
	normalCount = 0;
	faceCount = 0;

	// Open the file.
	fin.open(filename);

	// Check if it was successful in opening the file.
	if (fin.fail() == true)
	{
		return false;
	}
	//Start with 1 index
	array3D.resize(IndexAll);

	// Read from the file and continue to read until the end of the file is reached.
	fin.get(input);
	while (!fin.eof())
	{
		if (IndexAll <= f)
		{
			IndexAll++;
			array3D.resize(IndexAll);
		}
		// If the line starts with 'v' then count either the vertex, the texture coordinates, or the normal vector.
		if (input == 'v')
		{
			if (IndexAll <= i)
			{
				IndexAll++;
				array3D.resize(IndexAll);
			}
			else if (IndexAll <= j)
			{
				IndexAll++;
				array3D.resize(IndexAll);
			}
			else if (IndexAll <= k)
			{
				IndexAll++;
				array3D.resize(IndexAll);
			}

			fin.get(input);
			if (input == ' ')
			{
				vertexCount++;
				//if (i >= j && i >= f && i >= k)
				//	array3D.resize(vertexCount);
				fin >> array3D[i].x >> array3D[i].y >> array3D[i].z;
				array3D[i].z = -1.0f * array3D[i].z;
				ModelType* vertice = new ModelType;
				vertice->x = array3D[i].x;
				vertice->y = array3D[i].y;
				vertice->z = array3D[i].z;
				FindBoundingBox(vertice);
				delete vertice;
				vertice = NULL;
				i++;
			}
			if (input == 't')
			{
				textureCount++;
				/*if (j >= i && j >= f&& j >= k)
				array3D.resize(textureCount);*/
				fin >> array3D[j].tu >> array3D[j].tv;
				array3D[j].tv = 1.0f - array3D[j].tv;
				j++;
			}
			if (input == 'n')
			{
				normalCount++;
				/*if (k >= j && k >= f && k>=i)
				array3D.resize(textureCount);*/
				fin >> array3D[k].nx >> array3D[k].ny >> array3D[k].nz;
				array3D[k].nz = -1.0f * array3D[k].nz;
				k++;

			}
		}

		// If the line starts with 'f' then increment the face count.
		if (input == 'f')
		{
			fin.get(input);
			if (input == ' ') {
				faceCount++;
				fin.get(input);
				std::string indiceStr = "";
				int typeIndice = 0; // 0 = vertice, 1 = textura, 2 = normal, default 0
				int vertexInidice = 0; // 0 = fv1, 1 = fv2, 2 = fv3
				while (input != '\n') {
					// Obtener un vertice
					while (input != '/' && input != ' ' && input != '\n') { // Calquiera de estos tres condiciones se tiene que romper el while.
						indiceStr += std::string(1, input);
						fin.get(input);
					}
					if (indiceStr != "") {
						int indiceProp = stoi(indiceStr);
						if (vertexInidice == 0) {

							if (typeIndice == 0) {
								array3D[f].fv3 = indiceProp;
							}
							else if (typeIndice == 1) {
								array3D[f].ft3 = indiceProp;
							}
							else if (typeIndice == 2) {
								array3D[f].fn3 = indiceProp;
							}
						}
						else if (vertexInidice == 1) {
							if (typeIndice == 0) {
								array3D[f].fv2 = indiceProp;
							}
							else if (typeIndice == 1) {
								array3D[f].ft2 = indiceProp;
							}
							else if (typeIndice == 2) {
								array3D[f].fn2 = indiceProp;
							}
						}
						else if (vertexInidice == 2) {
							if (typeIndice == 0) {
								array3D[f].fv1 = indiceProp;
							}
							else if (typeIndice == 1) {
								array3D[f].ft1 = indiceProp;
							}
							else if (typeIndice == 2) {
								array3D[f].fn1 = indiceProp;
							}
						}
					}
					
					if (input == ' ') {
						vertexInidice++;
						typeIndice = 0;
						indiceStr = "";
					}
					else if (input == '/') {
						indiceStr = "";
						typeIndice++;
					}
					if (input != '\n') {
						fin.get(input);
					}
					else {
						indiceStr = "";
						typeIndice = 0;
						vertexInidice = 0;
					}
				}
				f++;
			}
		}

		// De otra manera lee lo que queda de la linea.
		while (input != '\n')
		{
			fin.get(input);
		}
		// Se empieza a leer la siguiente linea.
		fin.get(input);
	}
	if (array3D.size() > 0) {
		//Solo para fines de debugeo en caso de errores... Este codigo puede decir cosas relevantes
		_RPT3(0, "Vertices: %f,%f,%f\n", array3D[0].x, array3D[0].y, array3D[0].z);
		_RPT2(0, "UVs: %f, %f\n", array3D[0].tu, array3D[0].tv);
		_RPT3(0, "Normals: %f,%f,%f\n", array3D[0].nx, array3D[0].ny, array3D[0].nz);
		if (i > 0)
			_RPT3(0, "Vertices: %f,%f,%f\n", array3D[i - 1].x, array3D[i - 1].y, array3D[i - 1].z);
		else
			_RPT3(0,"Vertices: Not found",0);
		if(j > 0)
			_RPT2(0, "UVs: %f, %f\n", array3D[j - 1].tu, array3D[j - 1].tv);
		else 
			_RPT2(0, "UVs: Not found", 0);
		if(k > 0)
			_RPT3(0, "Normals: %f,%f,%f\n", array3D[k - 1].nx, array3D[k - 1].ny, array3D[k - 1].nz);
		else
			_RPT3(0, "Normals: Not found", 0);
		_RPT4(0, "V: %d   T: %d   N: %d   F: %d\n", i, j, k, f);
	}

	//------------------------------------------------------------------------------------------

	// Se cierra el archivo .obj.
	fin.close();

	//Reutilizamos una variable para contar cuantas caras necesitamos para el modelo
	j = 1;
	//A partir de la cuenta de vertices podemos conocer que neceitamos el triple para formar caras con triangulos
	//Aqui si utilizamos 3 vertices podemos usar el codigo siguiente, si se quieren usar quads no esta optimizado.
	
	result  = CargarBuffersOpenGL(array3D);
	if (!result)
		return false;

	//Liberamos la memoria de vectores
	array3D.clear();
	InfoModel.clear();
	return true;
}
bool Model::LoadModelObj(const char* objPath) {
	//Version beta no esta completo
	bool result;
	std::ifstream stream;
	char input, in;
	vector<VertexLoad> vertexArray;
	vector<VertexLoad> InfoModel;
	int i = 0, j = 0, k = 0, f = 0, vIndex = 0, tIndex = 0, nIndex = 0, IndexAll = 1;
	// Initialize the counts.
	int vertexCount = 0;
	int textureCount = 0;
	int normalCount = 0;
	int faceCount = 0;

	stream.open(objPath);
	if (stream.fail() == true) {
		return false;
	}
	//Comenzar en 1
	vertexArray.resize(IndexAll);
	stream.get(input);
	//Si es eof llego al final del archivo
	while (!stream.eof()) {
		// ...

	}
	return true;
}
//Sistema de carga por .obj
bool Model::Fourth_LoadModelByObj(vector<VertexLoad> InfoModelo, int VertexCount) {
	ifstream fin;
	char input;
	int i;

	//Set the number of indices to be the same as the vertex count
	m_indexCount = VertexCount;
	m_vertexCount = VertexCount;

	//create the model using the vertex count that was read in
	m_model = new ModelType[m_vertexCount];
	if (!m_model)
		return false;

	//read in the vertex data
	for (i = 0; i < m_vertexCount; i++) {

		m_model[i].x = InfoModelo[i].x;
		m_model[i].y = InfoModelo[i].y;
		m_model[i].z = InfoModelo[i].z;
		m_model[i].tu = InfoModelo[i].tu;
		m_model[i].tv = InfoModelo[i].tv;
		m_model[i].nx = InfoModelo[i].nx;
		m_model[i].ny = InfoModelo[i].ny;
		m_model[i].nz = InfoModelo[i].nz;

		//colisiones cuadradas
		if (max->x <= m_model[i].x)
			max->x = m_model[i].x;
		else if (min->x >= m_model[i].x)
			min->x = m_model[i].x;
		if (max->y <= m_model[i].y)
			max->y = m_model[i].y;
		else if (min->y >= m_model[i].y)
			min->y = m_model[i].y;
		if (max->z <= m_model[i].z)
			max->z = m_model[i].z;
		else if (min->z >= m_model[i].z)
			min->z = m_model[i].z;

	}

	//Close the model file
	fin.close();

	return true;

}
bool Model::CargarBuffersOpenGL(std::vector<VertexLoad> array3D) {
	int i = 1, j = 1, vIndex = 0, tIndex = 0, nIndex = 0;
	m_indexCount = (array3D.size() - 1) * 3;
	m_vertexCount = (array3D.size() - 1) * 3;

	m_model = new ModelType[m_vertexCount];
	if (!m_model)
		return false;
	
	for (i = 1; i < array3D.size(); i++) {
		//_RPT1(0, "i %d\n", i);
		vIndex = array3D[i].fv1 - 1;
		tIndex = array3D[i].ft1 - 1;
		nIndex = array3D[i].fn1 - 1;
		//InfoModel.resize(j);
		if (vIndex != -1) {
			m_model[j - 1].x = array3D[vIndex].x;
			m_model[j - 1].y = array3D[vIndex].y;
			m_model[j - 1].z = array3D[vIndex].z;
		}
		if (tIndex != -1) {
			m_model[j - 1].tu = array3D[tIndex].tu;
			m_model[j - 1].tv = array3D[tIndex].tv;
		}
		if (nIndex != -1) {
			m_model[j - 1].nx = array3D[nIndex].nx;
			m_model[j - 1].ny = array3D[nIndex].ny;
			m_model[j - 1].nz = array3D[nIndex].nz;
		}
		//FindBoundingBox(&m_model[j-1]);
		j++;
		vIndex = array3D[i].fv2 - 1;
		tIndex = array3D[i].ft2 - 1;
		nIndex = array3D[i].fn2 - 1;
		//InfoModel.resize(j);
		if (vIndex != -1) {
			m_model[j - 1].x = array3D[vIndex].x;
			m_model[j - 1].y = array3D[vIndex].y;
			m_model[j - 1].z = array3D[vIndex].z;
		}
		if (tIndex != -1) {
			m_model[j - 1].tu = array3D[tIndex].tu;
			m_model[j - 1].tv = array3D[tIndex].tv;
		}
		if (nIndex != -1) {
			m_model[j - 1].nx = array3D[nIndex].nx;
			m_model[j - 1].ny = array3D[nIndex].ny;
			m_model[j - 1].nz = array3D[nIndex].nz;
		}
		//FindBoundingBox(&m_model[j - 1]);
		j++;
		vIndex = array3D[i].fv3 - 1;
		tIndex = array3D[i].ft3 - 1;
		nIndex = array3D[i].fn3 - 1;
		//InfoModel.resize(j);
		if (vIndex != -1) {
			m_model[j - 1].x = array3D[vIndex].x;
			m_model[j - 1].y = array3D[vIndex].y;
			m_model[j - 1].z = array3D[vIndex].z;
		}
		if (tIndex != -1) {
			m_model[j - 1].tu = array3D[tIndex].tu;
			m_model[j - 1].tv = array3D[tIndex].tv;
		}
		if (nIndex != -1) {
			m_model[j - 1].nx = array3D[nIndex].nx;
			m_model[j - 1].ny = array3D[nIndex].ny;
			m_model[j - 1].nz = array3D[nIndex].nz;
		}
		//FindBoundingBox(&m_model[j - 1]);
		j++;

	}

	return true;
}
void Model::FindBoundingBox(ModelType* vertice) {
	//colisiones cuadradas
	if (max->x <= vertice->x)
		max->x = vertice->x;
	else if (min->x >= vertice->x)
		min->x = vertice->x;
	if (max->y <= vertice->y)
		max->y = vertice->y;
	else if (min->y >= vertice->y)
		min->y = vertice->y;
	if (max->z <= vertice->z)
		max->z = vertice->z;
	else if (min->z >= vertice->z)
		min->z = vertice->z;
	if (vertice->x == -431602080) {
		min ->z = 0.0f;
	}
}
#pragma endregion

#pragma region _CALCULE_VECTOR_TANGENT_BINORMAL_
void Model::CalculateModelVectors()
{
	int faceCount, i, index;
	TempVertexType vertex1, vertex2, vertex3;
	VectorType tangent, binormal, normal;


	// Calculate the number of faces in the model.
	faceCount = m_vertexCount / 3;

	// Initialize the index to the model data.
	index = 0;

	// Go through all the faces and calculate the the tangent, binormal, and normal vectors.
	for (i = 0; i < faceCount; i++)
	{
		// Get the three vertices for this face from the model.
		vertex1.x = m_model[index].x;
		vertex1.y = m_model[index].y;
		vertex1.z = m_model[index].z;
		vertex1.tu = m_model[index].tu;
		vertex1.tv = m_model[index].tv;
		vertex1.nx = m_model[index].nx;
		vertex1.ny = m_model[index].ny;
		vertex1.nz = m_model[index].nz;
		index++;

		vertex2.x = m_model[index].x;
		vertex2.y = m_model[index].y;
		vertex2.z = m_model[index].z;
		vertex2.tu = m_model[index].tu;
		vertex2.tv = m_model[index].tv;
		vertex2.nx = m_model[index].nx;
		vertex2.ny = m_model[index].ny;
		vertex2.nz = m_model[index].nz;
		index++;

		vertex3.x = m_model[index].x;
		vertex3.y = m_model[index].y;
		vertex3.z = m_model[index].z;
		vertex3.tu = m_model[index].tu;
		vertex3.tv = m_model[index].tv;
		vertex3.nx = m_model[index].nx;
		vertex3.ny = m_model[index].ny;
		vertex3.nz = m_model[index].nz;
		index++;

		// Calculate the tangent and binormal of that face.
		CalculateTangentBinormal(vertex1, vertex2, vertex3, tangent, binormal);

		// Calculate the new normal using the tangent and binormal.
		//CalculateNormal(tangent, binormal, normal);

		// Store the normal, tangent, and binormal for this face back in the model structure.
		//m_model[index - 1].nx = normal.x;
		//m_model[index - 1].ny = normal.y;
		//m_model[index - 1].nz = normal.z;
		m_model[index - 1].tx = tangent.x;
		m_model[index - 1].ty = tangent.y;
		m_model[index - 1].tz = tangent.z;
		m_model[index - 1].bx = binormal.x;
		m_model[index - 1].by = binormal.y;
		m_model[index - 1].bz = binormal.z;

		//m_model[index - 2].nx = normal.x;
		//m_model[index - 2].ny = normal.y;
		//m_model[index - 2].nz = normal.z;
		m_model[index - 2].tx = tangent.x;
		m_model[index - 2].ty = tangent.y;
		m_model[index - 2].tz = tangent.z;
		m_model[index - 2].bx = binormal.x;
		m_model[index - 2].by = binormal.y;
		m_model[index - 2].bz = binormal.z;

		//m_model[index - 3].nx = normal.x;
		//m_model[index - 3].ny = normal.y;
		//m_model[index - 3].nz = normal.z;
		m_model[index - 3].tx = tangent.x;
		m_model[index - 3].ty = tangent.y;
		m_model[index - 3].tz = tangent.z;
		m_model[index - 3].bx = binormal.x;
		m_model[index - 3].by = binormal.y;
		m_model[index - 3].bz = binormal.z;
	}

	return;
}
void Model::CalculateTangentBinormal(TempVertexType vertex1, TempVertexType vertex2, TempVertexType vertex3,
	VectorType& tangent, VectorType& binormal)
{
	float vector1[3], vector2[3];
	float tuVector[2], tvVector[2];
	float den;
	float length;


	// Calculate the two vectors for this face.
	vector1[0] = vertex2.x - vertex1.x;
	vector1[1] = vertex2.y - vertex1.y;
	vector1[2] = vertex2.z - vertex1.z;

	vector2[0] = vertex3.x - vertex1.x;
	vector2[1] = vertex3.y - vertex1.y;
	vector2[2] = vertex3.z - vertex1.z;

	// Calculate the tu and tv texture space vectors.
	tuVector[0] = vertex2.tu - vertex1.tu;
	tvVector[0] = vertex2.tv - vertex1.tv;

	tuVector[1] = vertex3.tu - vertex1.tu;
	tvVector[1] = vertex3.tv - vertex1.tv;

	// Calculate the denominator of the tangent/binormal equation.
	den = 1.0f / (tuVector[0] * tvVector[1] - tuVector[1] * tvVector[0]);

	// Calculate the cross products and multiply by the coefficient to get the tangent and binormal.
	tangent.x = (tvVector[1] * vector1[0] - tvVector[0] * vector2[0]) * den;
	tangent.y = (tvVector[1] * vector1[1] - tvVector[0] * vector2[1]) * den;
	tangent.z = (tvVector[1] * vector1[2] - tvVector[0] * vector2[2]) * den;

	binormal.x = (tuVector[0] * vector2[0] - tuVector[1] * vector1[0]) * den;
	binormal.y = (tuVector[0] * vector2[1] - tuVector[1] * vector1[1]) * den;
	binormal.z = (tuVector[0] * vector2[2] - tuVector[1] * vector1[2]) * den;

	// Calculate the length of this normal.
	length = sqrt((tangent.x * tangent.x) + (tangent.y * tangent.y) + (tangent.z * tangent.z));

	// Normalize the normal and then store it
	tangent.x = tangent.x / length;
	tangent.y = tangent.y / length;
	tangent.z = tangent.z / length;

	// Calculate the length of this normal.
	length = sqrt((binormal.x * binormal.x) + (binormal.y * binormal.y) + (binormal.z * binormal.z));

	// Normalize the normal and then store it
	binormal.x = binormal.x / length;
	binormal.y = binormal.y / length;
	binormal.z = binormal.z / length;

	return;
}
void Model::CalculateNormal(VectorType tangent, VectorType binormal, VectorType& normal)
{
	float length;


	// Calculate the cross product of the tangent and binormal which will give the normal vector.
	normal.x = (tangent.y * binormal.z) - (tangent.z * binormal.y);
	normal.y = (tangent.z * binormal.x) - (tangent.x * binormal.z);
	normal.z = (tangent.x * binormal.y) - (tangent.y * binormal.x);

	// Calculate the length of the normal.
	length = sqrt((normal.x * normal.x) + (normal.y * normal.y) + (normal.z * normal.z));

	// Normalize the normal.
	normal.x = normal.x / length;
	normal.y = normal.y / length;
	normal.z = normal.z / length;

	return;
}
#pragma endregion

void Model::Shutdown(OpenGLClass* OpenGL)
{
	if (world) {
		delete world;
		world = 0;
	}
	if (min) {
		delete min;
		min = 0;
	}
	if (max) {
		delete max;
		max = 0;
	}
	if (center) {
		delete center;
		center = 0;
	}

	OpenGL->glDisableVertexAttribArray(0);
	OpenGL->glDisableVertexAttribArray(1);
	OpenGL->glDisableVertexAttribArray(2);
	OpenGL->glDisableVertexAttribArray(3);
	OpenGL->glDisableVertexAttribArray(4);

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

BoundingBox* Model::GetBox() {
	return this->box;
	return this->box2;
}