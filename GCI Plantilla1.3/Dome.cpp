#include "Dome.h"

Dome::Dome()
{
	this->myMatrix = 0;
	Dome::radius = 10.0f;
	Dome::slices = 32;
	Dome::slides = 32;
	DeltaColor = 0;
	inicio = 0.5;
	final = 1;
	pathTexture = "";
}

Dome::Dome(string pathTexture, OpenGLClass* OpenGL, TextureClass* LoaderTextureRef, float radius)
{
	this->myMatrix = 0;
	Dome::radius = 10.0f;
	Dome::slices = 36;
	Dome::slides = 36;
	inicio = 0.4;
	final = 1;
	this->pathTexture = pathTexture;
	this->OpenGL = OpenGL;
	this->LoaderTexRef = LoaderTextureRef;
	this->radius = radius;
}

Dome::~Dome()
{
	//Stuff
}

float* Dome::GetWorldMatrix() {
	return this->myMatrix;
}

void Dome::RedefineTransformsTSR(float toXTranslate, float toYTranslate, float toZTranslate,
	float toXScale, float toYScale, float toZScale,
	float toXFactorRotation, float toYFactorRotation, float toZFactorRotation, float Angle)
{
	XTranslate = toXTranslate;
	YTranslate = toYTranslate;
	ZTranslate = toZTranslate;
	XScale = toXScale;
	YScale = toYScale;
	ZScale = toZScale;
	XFactorRotation = toXFactorRotation;
	YFactorRotation = toYFactorRotation;
	ZFactorRotation = toZFactorRotation;
	AngleRotation = Angle;
}

void Dome::RedefineTransformsTS(float toXTranslate, float toYTranslate, float toZTranslate,
	float toXScale, float toYScale, float toZScale)
{
	XTranslate = toXTranslate;
	YTranslate = toYTranslate;
	ZTranslate = toZTranslate;
	XScale = toXScale;
	YScale = toYScale;
	ZScale = toZScale;
}

void Dome::RedefineTransformsT(float toXTranslate, float toYTranslate, float toZTranslate)
{
	XTranslate = toXTranslate;
	YTranslate = toYTranslate;
	ZTranslate = toZTranslate;
}

void Dome::RedefineColor(float changeRColor, float changeGColor, float changeBColor,float changeAColor)
{
	DeltaColor = new float[4] { changeRColor, changeGColor,
		changeBColor, changeAColor };
}

float* Dome::GetDeltaColor() {
	return this->DeltaColor;
}

void Dome::SetIterpolationDay(bool Auto, bool TreeSteps) {
	if (Auto)
		interpolacionAutomatica = Auto;
	else
		interpolacionAmanecerTarde = TreeSteps;
	return;
}

void Dome::ConstructDome() {
	float dx = slices - 1.0f;
	float dy = slides - 1.0f;

	// Vertices
	for (int i = 0; i < slices; i++)
	{
		for (int j = 0; j < slides; j++)
		{
			//Pos
			int indice = (i * slides + j);
			vertices[indice].Posx = radius * cos(((double)j / (slides - 1)) * (PI * (final - inicio)) + PI * inicio - PI / 2.0) * cos(2.0 * PI * (double)i / (slices - 1));
			vertices[indice].Posy = radius * sin(((double)j / (slides - 1)) * (PI * (final - inicio)) + PI * inicio - PI / 2.0);
			vertices[indice].Posz = radius * cos(((double)j / (slides - 1)) * (PI * (final - inicio)) + PI * inicio - PI / 2.0) * sin(2.0 * PI * (double)i / (slices - 1));

			//Color
			vertices[indice].ColorR = DeltaColor[0];
			vertices[indice].ColorG = DeltaColor[1];
			vertices[indice].ColorB = DeltaColor[2];

			//Normal
			vertices[indice].Normx = cos(((double)j / (slides - 1)) * (PI * (final - inicio)) + PI * inicio - PI / 2.0) * cos(2.0 * PI * (double)i / (slices - 1));
			vertices[indice].Normy = sin(((double)j / (slides - 1)) * (PI * (final - inicio)) + PI * inicio - PI / 2.0);
			vertices[indice].Normz = cos(((double)j / (slides - 1)) * (PI * (final - inicio)) + PI * inicio - PI / 2.0) * sin(2.0 * PI * (double)i / (slices - 1));

			//UV
			vertices[indice].u = (float)1 * (1 - (float)i / (slides - 1));
			vertices[indice].v = (float)1 * (1 - (float)j / (slices - 1));
		}
	}

	// Indices
	int index = 0, index1 = 0, index2 = 0, index3 = 0, index4 = 0;
	for (int i = 0; i < slices - 1; i++)
	{
		for (int j = 0; j < slides - 1; j++)
		{
			index1 = (slides * i) + j;    // Bottom left.
			index2 = (slides * i) + (j + 1);  // Bottom right.
			index3 = (slides * (i + 1)) + j;    // Upper left.
			index4 = (slides * (i + 1)) + (j + 1);  // Upper right.

			indices[index++] = index4;
			indices[index++] = index2;
			indices[index++] = index1;

			indices[index++] = index3;
			indices[index++] = index4;
			indices[index++] = index1;
		}
	}
}

bool Dome::Redraw() {
	m_vertexCount = slices * slides * 3;
	m_indexCount = (slices) * (slides) * 6;

	vertices = new VertexType[m_vertexCount];
	if (!vertices)
		return false;
	indices = new unsigned int[m_indexCount];
	if (!indices)
		return false;
	
	ConstructDome();

	OpenGL->glGenVertexArrays(1, &m_vertexArrayId);
	OpenGL->glBindVertexArray(m_vertexArrayId);

	OpenGL->glGenBuffers(1, &m_vertexBufferId);
	OpenGL->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
	OpenGL->glBufferData(GL_ARRAY_BUFFER, m_vertexCount * sizeof(VertexType), vertices, GL_STATIC_DRAW);

	OpenGL->glEnableVertexAttribArray(0); //Position
	OpenGL->glEnableVertexAttribArray(1); // Color
	OpenGL->glEnableVertexAttribArray(2); // Normal
	OpenGL->glEnableVertexAttribArray(3); // Texcoord

	OpenGL->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
	OpenGL->glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(VertexType), 0);
	OpenGL->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
	OpenGL->glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(VertexType), (unsigned char*)NULL + (3 * sizeof(float)));
	OpenGL->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
	OpenGL->glVertexAttribPointer(2, 3, GL_FLOAT, false, sizeof(VertexType), (unsigned char*)NULL + (6 * sizeof(float)));
	OpenGL->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
	OpenGL->glVertexAttribPointer(3, 2, GL_FLOAT, false, sizeof(VertexType), (unsigned char*)NULL + (9 * sizeof(float)));

	OpenGL->glGenBuffers(1, &m_indexBufferId);
	OpenGL->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId);
	OpenGL->glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	//Release the arrays now that the vertex and index buffers have been created and loaded
	delete[]vertices;
	vertices = 0;

	delete[]indices;
	indices = 0;

	return true;
}

bool Dome::Initialize()
{
	myMatrix = new float[16] { 0.0f };
	if (!OpenGL) {
		return false;
	}
	OpenGL->BuildIdentityMatrix(myMatrix);

	m_vertexCount = slices * slides;
	m_indexCount = (slices - 1) * (slides - 1) * 6;

	vertices = new VertexType[m_vertexCount];
	if (!vertices)
		return false;
	indices = new unsigned int[m_indexCount];
	if (!indices)
		return false;

	DeltaColor = new float[4] { ColorInicial[0], ColorInicial[1], ColorInicial[2], ColorInicial[3] };

	SetIterpolationDay(true, false);

	if (pathTexture != "") {
		TextureID1 = LoaderTexRef->Initialize(pathTexture, true);
	}

	ConstructDome();

	OpenGL->glGenVertexArrays(1, &m_vertexArrayId);
	OpenGL->glBindVertexArray(m_vertexArrayId);

	OpenGL->glGenBuffers(1, &m_vertexBufferId);
	OpenGL->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
	OpenGL->glBufferData(GL_ARRAY_BUFFER, m_vertexCount * sizeof(VertexType), vertices, GL_STATIC_DRAW);

	OpenGL->glEnableVertexAttribArray(0); //Position
	OpenGL->glEnableVertexAttribArray(1); // Color
	OpenGL->glEnableVertexAttribArray(2); // Normal
	OpenGL->glEnableVertexAttribArray(3); // Texcoord

	OpenGL->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
	OpenGL->glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(VertexType), 0);
	OpenGL->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
	OpenGL->glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(VertexType), (unsigned char*)NULL + (3 * sizeof(float)));
	OpenGL->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
	OpenGL->glVertexAttribPointer(2, 3, GL_FLOAT, false, sizeof(VertexType), (unsigned char*)NULL + (6 * sizeof(float)));
	OpenGL->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
	OpenGL->glVertexAttribPointer(3, 2, GL_FLOAT, false, sizeof(VertexType), (unsigned char*)NULL + (9 * sizeof(float)));

	OpenGL->glGenBuffers(1, &m_indexBufferId);
	OpenGL->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId);
	OpenGL->glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	//Release the arrays now that the vertex and index buffers have been created and loaded
	delete[]vertices;
	vertices = 0;

	delete[]indices;
	indices = 0;

	return true;
}

bool Dome::Render(float* viewMatrix, float* projectionMatrix)
{
	AngleRotation < 360 ? AngleRotation += 0.001f : AngleRotation = 0;

	// ** Definimos la escala **
	MatrixScale();

	// ** Definimos la rotacion **
	MatrixRotationY();

	// ** Definimos la traslacion **
	MatrixTranslation();

	SkyShader->SetShader();
	SkyShader->PonMatriz4x4((char*)"worldMatrix", myMatrix);
	SkyShader->PonMatriz4x4((char*)"viewMatrix", viewMatrix);
	SkyShader->PonMatriz4x4((char*)"projectionMatrix", projectionMatrix);
	SkyShader->Pon1Entero((char*)"shaderTexture", TextureID1);
	SkyShader->PonVec3((char*)"colorShader", DeltaColor);

	//Render shader

	// Bind the vertex array object that stored all the information about the vertex and index buffers.
	OpenGL->glBindVertexArray(m_vertexArrayId);

	// Render the vertex buffer using the index buffer.
	glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);

	return true;
}

void Dome::Shutdown()
{
	OpenGL->glDisableVertexAttribArray(0);
	OpenGL->glDisableVertexAttribArray(1);

	OpenGL->glBindBuffer(GL_ARRAY_BUFFER, 0);
	OpenGL->glDeleteBuffers(1, &m_vertexBufferId);

	OpenGL->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	OpenGL->glDeleteBuffers(1, &m_indexBufferId);

	OpenGL->glBindVertexArray(0);
	OpenGL->glDeleteVertexArrays(1, &m_vertexArrayId);

	if (myMatrix) {
		delete myMatrix;
		myMatrix = 0;
	}
	return;
}

bool Dome::Interpolar1() {
	ColorAnterior[0] = 255.00f;
	ColorAnterior[1] = 255.00f;
	ColorAnterior[2] = 255.00f;
	ColorTarget[0] = 255.00f;
	ColorTarget[1] = 165.00f;
	ColorTarget[2] = 1.00f;
	// Validamos cuanto tiempo ha transcurrido
	if (elapsedTime <= cicloLuz) {
		float* nuevoColor = new float[4] {
			// DE MOMENTO este coeficiente no necesita realizar cambios
			colorDomo[0],
				// El siguiente si lo necesita pero hay que desarrollar la logica siguiente
				// 1. Extraemos el coeficiente que existe entre el color actual y el color deseado (haciendo una resta entre ambos)
				// 2. Multiplicamos por el coeficiente en que se mide RGB, es decir, en 255, de esta manera sacamos cuantas unidades necesitamos antes de llegar al nuevo color en RGB
				// 3. Dividimos entre el lapso de segundos que debemos esperar en que llegue al color deseado (expresado en segundos)
				// 4. Devolvemos el valor de RGB a escala de entre 0.00f y 1.00f dividiendo entre 255
				colorDomo[1] - ((((ColorAnterior[1] / 255.00f) - (ColorTarget[1] / 255.00f)) * 255) / cicloLuz) / 255,
				// Mismo caso que arriba
				colorDomo[2] - ((((ColorAnterior[2] / 255.00f) - (ColorTarget[2] / 255.00f)) * 255) / cicloLuz) / 255,
				// Este no necesita realizar cambios
				colorDomo[3]};
		// Traspasamos el color antes trabajado
		RedefineColor(nuevoColor[0],
			nuevoColor[1],
			nuevoColor[2],
			nuevoColor[3]);
		// Codigo de color naranja en RG (255,165,0,1)
		this->RedefineColor(this->coeficienteDeCambio * (10.00f / 255.00f),
			this->coeficienteDeCambio * (10.00f / 165.00f),
			this->coeficienteDeCambio * (10.00f / 1.00f),
			this->coeficienteDeCambio * 10.00f);
		Redraw();
		return false;
	}
	else if (cicloInterpolacionSeg >= 0 && cicloLuz <= 0) {
		cicloInterpolacionSeg--;
		return false;
	}
	else {
		interpolacionAmanecerTarde = false; //Bandera
		interpolacionTardeNoche = true; //Bandera
		cicloLuz = 360; //Segundos en que permanece la textura actual
		cicloInterpolacionSeg = 200; // Lo que me demoro en cambiar a la siguiente textura
		// definimos el color que se supone debe inicializar para la siguiente fase
		RedefineColor((255.00f / 255.00f),
			(170.00f / 255.00f),
			(1.00f / 255.00f),
			(255.00f / 255.00f));
		// Equivalencia del color entre 255 para sacarlo = 1.0
		// EJ: 165 / 255 = .064705fM
		//this->RedefineColor(1.00f, 0.64705f, 0.00f, 1.00f);
		Redraw();
		elapsedTime = 0;
		return true;
	}
}

bool Dome::Interpolar2() {
	ColorAnterior[0] = 255.00f;
	ColorAnterior[1] = 165.00f;
	ColorAnterior[2] = 1.00f;
	ColorTarget[0] = 60.00f;
	ColorTarget[1] = 60.00f;
	ColorTarget[2] = 60.00f;
	if (elapsedTime <= cicloLuz) {
		float* nuevoColor = new float[4] {
			colorDomo[0] - ((((ColorAnterior[0] / 255.00f) - (ColorTarget[0] / 255.00f)) * 255) / cicloLuz) / 255,
				colorDomo[1] - ((((ColorAnterior[1] / 255.00f) - (ColorTarget[1] / 255.00f)) * 255) / cicloLuz) / 255,
				colorDomo[2] - ((((ColorAnterior[2] / 255.00f) - (ColorTarget[2] / 255.00f)) * 255) / cicloLuz) / 255,
				colorDomo[3]};
		// Traspasamos el color antes trabajado
		RedefineColor(nuevoColor[0],
			nuevoColor[1],
			nuevoColor[2],
			nuevoColor[3]);
		//// Codigo de color negro deseado en RG (60,60,60,1)
		this->RedefineColor(this->coeficienteDeCambio * (10.00f /60.00f),
			this->coeficienteDeCambio * (10.00f / 60.00f),
			this->coeficienteDeCambio * (10.00f / 60.00f),
			this->coeficienteDeCambio * 10.00f);
		Redraw();
		return false;
	}
	else if (cicloInterpolacionSeg >= 0 && cicloLuz <= 0) {
		cicloInterpolacionSeg--;
		return false;
	}
	else {
		interpolacionTardeNoche = false; //Bandera
		interpolacionNocheAmanecer = true; //Bandera
		cicloLuz = 360; //Segundos en que permanece la textura actual
		cicloInterpolacionSeg = 200; // Lo que me demoro en cambiar a la siguiente textura
		// definimos el color que se supone debe inicializar para la siguiente fase
		//RedefineColor((65.00f / 255.00f),
		//	(65.00f / 255.00f),
		//	(65.00f / 255.00f),
		//	(255.00f / 255.00f));
		Redraw();
		elapsedTime = 0;
		return true;
	}
}

bool Dome::Interpolar3() {
	ColorAnterior[0] = 60.00f;
	ColorAnterior[1] = 60.00f;
	ColorAnterior[2] = 60.00f;
	ColorTarget[0] = 250.00f;
	ColorTarget[1] = 250.00f;
	ColorTarget[2] = 250.00f;
	if (elapsedTime <= cicloLuz) {
		float* nuevoColor = new float[4] {
			colorDomo[0] - ((((ColorAnterior[0] / 255.00f) - (ColorTarget[0] / 255.00f)) * 255) / cicloLuz) / 255,
				colorDomo[1] - ((((ColorAnterior[1] / 255.00f) - (ColorTarget[1] / 255.00f)) * 255) / cicloLuz) / 255,
				colorDomo[2] - ((((ColorAnterior[2] / 255.00f) - (ColorTarget[2] / 255.00f)) * 255) / cicloLuz) / 255,
				colorDomo[3]};
		// Traspasamos el color antes trabajado
		RedefineColor(nuevoColor[0],
			nuevoColor[1],
			nuevoColor[2],
			nuevoColor[3]);
		//// Codigo de color blanco deseado en RG (230,230,230,1)
		this->RedefineColor(this->coeficienteDeCambio * (1.00f / 230.00f),
			this->coeficienteDeCambio * (10.00f / 230.00f),
			this->coeficienteDeCambio * (10.00f / 230.00f),
			this->coeficienteDeCambio * 10.00f);
		Redraw();
		return false;
	}
	else if (cicloInterpolacionSeg >= 0 && cicloLuz <= 0) {
		cicloInterpolacionSeg--;
		return false;
	}
	else {
		interpolacionNocheAmanecer = false; //Bandera
		interpolacionAmanecerTarde = true; //Bandera
		cicloLuz = 360; //Segundos en que permanece la textura actual
		cicloInterpolacionSeg = 200; // Lo que me demoro en cambiar a la siguiente textura
		// definimos el color que se supone debe inicializar para la siguiente fase
		//RedefineColor((255.00f / 255.00f),
		//	(255.00f / 255.00f),
		//	(255.00f / 255.00f),
		//	(255.00f / 255.00f));
		Redraw();
		elapsedTime = 0;
		return true;
	}
}

void Dome::CicloInterpolaciones() {
	colorDomo = this->GetDeltaColor();
	if (interpolacionAmanecerTarde == true) {
		Interpolar1();
	}
	else if (interpolacionTardeNoche == true) {
		Interpolar2();
	}
	else if (interpolacionNocheAmanecer == true) {
		Interpolar3();
	}
	elapsedTime++;
}

// Transforms
void Dome::MatrixTranslation()
{
	//myMatrix[0]  = 1.0f;
	//myMatrix[1]  = 0.0f;
	//myMatrix[2]  = 0.0f;
	//myMatrix[3]  = 0.0f;

	//myMatrix[4]  = 0.0f;
	//myMatrix[5]  = 1.0f;
	//myMatrix[6]  = 0.0f;
	//myMatrix[7]  = 0.0f;

	//myMatrix[8]  = 0.0f;
	//myMatrix[9]  = 0.0f;
	//myMatrix[10] = 1.0f;
	//myMatrix[11] = 0.0f;

	myMatrix[12] = XTranslate;
	myMatrix[13] = YTranslate;
	myMatrix[14] = ZTranslate;
	myMatrix[15] = 1.0f;

	return;
}

void Dome::MatrixRotationY()
{
	myMatrix[0] = cosf(AngleRotation);
	myMatrix[1] = 0.0f;
	myMatrix[2] = -sinf(AngleRotation);
	myMatrix[3] = 0.0f;

	myMatrix[4] = 0.0f;
	myMatrix[5] = 1.0f;
	myMatrix[6] = 0.0f;
	myMatrix[7] = 0.0f;

	myMatrix[8] = sinf(AngleRotation);
	myMatrix[9] = 0.0f;
	myMatrix[10] = cosf(AngleRotation);
	myMatrix[11] = 0.0f;

	myMatrix[12] = 0.0f;
	myMatrix[13] = 0.0f;
	myMatrix[14] = 0.0f;
	myMatrix[15] = 1.0f;

	return;
}

void Dome::MatrixScale() {
	myMatrix[0] = XScale;
	//myMatrix[1] = 0.0f;
	//myMatrix[2] = 0.0f;
	//myMatrix[3] = 0.0f;

	//myMatrix[4] = 0.0f;
	myMatrix[5] = YScale;
	//myMatrix[6] = 0.0f;
	//myMatrix[7] = 0.0f;

	//myMatrix[8] = 0.0f;
	//myMatrix[9] = 0.0f;
	myMatrix[10] = ZScale;
	//myMatrix[11] = 0.0f;

	//myMatrix[12] = 0.0f;
	//myMatrix[13] = 0.0f;
	//myMatrix[14] = 0.0f;
	//myMatrix[15] = 1.0f;
}

void Dome::SetShader(Shader* SkyShader) {
	this->SkyShader = SkyShader;
}