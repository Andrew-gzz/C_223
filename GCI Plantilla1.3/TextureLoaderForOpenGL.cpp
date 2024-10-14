////////////////////////////////////////////////////////////////////////////////
// Filename: textureclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "TextureLoaderForOpenGL.h"


TextureClass::TextureClass(OpenGLClass* OpenGL)
{
	this->OpenGL = OpenGL;
	loaded = false;
}


TextureClass::TextureClass(const TextureClass& other)
{
}


TextureClass::~TextureClass()
{
}


unsigned int TextureClass::Initialize(string filename, bool wrap)
{
	bool result;

	// Load the targa file.
	// Carga la textura del modelo, tiene 2 modos, tga y otros.
	if (filename != "")
	{
		if (filename.substr(filename.find_last_of(".") + 1) == "tga")
			result = LoadTextureTarga(filename);
		else
			result = LoadTextureOtherFormats(filename);
		if (!result)
			return false;//string("No se pudo cargar la textura del modelo correctamente: \n Texture ") + string(" \nTEXTURA ") + string(fn) + string(" ") + string(result);
	}
	loaded = true;
	return TexturesIndexLoaded-1;
}


void TextureClass::Shutdown()
{
	// If the texture was loaded then make sure to release it on shutdown.
	if (loaded)
	{
		glDeleteTextures(1, &m_textureID);
		loaded = false;
	}

	return;
}


bool TextureClass::LoadTextureTarga(string filename)
{
	bool result;
	// Load the targa image data into memory.
	result = LoadTarga(filename, TexturesIndexLoaded, true);
	if (!result)
	{
		return false;
	}
	TexturesIndexLoaded++;

	return true;
}

bool TextureClass::LoadTextureOtherFormats(string filename)
{
	bool result;
	// Load the targa image data into memory.
	result = LoadOtherFormats(filename, TexturesIndexLoaded, true);

	if (!result)
	{
		return false;
	}
	TexturesIndexLoaded++;

	return true;
}

bool TextureClass::LoadTarga(string filename, unsigned int textureUnit, bool wrap)
{
	string ErrorString = "";
	try {
		ifstream in(filename);

		if (in.fail()) {
			ErrorString = "Archivo Textura no encontrado o extensiones erroneas, favor de revisar - ";
			ErrorString += filename;
			throw std::invalid_argument("Archivo no encontrado");
		}

		int error, width, height, bpp, imageSize;
		FILE* filePtr;
		unsigned int count;
		TargaHeader targaFileHeader;

		unsigned char* targaImage;
		std::wstring widestr = std::wstring(filename.begin(), filename.end());
		const wchar_t* szName = widestr.c_str();
		_bstr_t b(szName);
		const char* c = b;

		// Open the targa file for reading in binary.
		error = fopen_s(&filePtr, c, "rb");
		if (error != 0)
		{
			return false;
		}

		// Read in the file header.
		count = fread(&targaFileHeader, sizeof(TargaHeader), 1, filePtr);
		if (count != 1)
		{
			return false;
		}

		// Get the important information from the header.
		width = (int)targaFileHeader.width;
		height = (int)targaFileHeader.height;
		bpp = (int)targaFileHeader.bpp;

		// Check that it is 32 bit and not 24 bit.
		if (bpp != 32)
		{
			return false;
		}

		// Calculate the size of the 32 bit image data.
		imageSize = width * height * 4;

		// Allocate memory for the targa image data.
		targaImage = new unsigned char[imageSize];
		if (!targaImage)
		{
			return false;
		}

		// Read in the targa image data.
		count = fread(targaImage, 1, imageSize, filePtr);
		if (count != imageSize)
		{
			return false;
		}

		// Close the file.
		error = fclose(filePtr);
		if (error != 0)
		{
			return false;
		}

		// Set the unique texture unit in which to store the data.
		OpenGL->glActiveTexture(GL_TEXTURE0 + textureUnit);

		// Generate an ID for the texture.
		glGenTextures(1, &m_textureID);

		// Bind the texture as a 2D texture.
		glBindTexture(GL_TEXTURE_2D, m_textureID);

		// Load the image data into the texture unit.
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, targaImage);

		// Set the texture color to either wrap around or clamp to the edge.
		if (wrap)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		}

		// Set the texture filtering.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		// Generate mipmaps for the texture.
		OpenGL->glGenerateMipmap(GL_TEXTURE_2D);

		// Release the targa image data.
		delete[] targaImage;
		targaImage = 0;

		return true;
	}
	catch (exception ex) {
		printf("Error,&s", ErrorString.c_str());
		MessageBoxA(NULL, ErrorString.c_str(), "Error", MB_OK);
		return false;
	}
}

bool TextureClass::LoadOtherFormats(string filename, unsigned int textureUnit, bool wrap) {
	string ErrorString = "";
	try {
		ifstream in(filename);

		if (in.fail()) {
			ErrorString = "Archivo Textura no encontrado o extensiones erroneas, favor de revisar - ";
			ErrorString += filename;
			throw std::invalid_argument("Archivo no encontrado");
		}

		std::wstring widestr = std::wstring(filename.begin(), filename.end());
		const wchar_t* szName = widestr.c_str();
		_bstr_t b(szName);
		const char* nombre = b;

		textureBitmap = gdipLoad(b);

		// Set the unique texture unit in which to store the data.
		OpenGL->glActiveTexture(GL_TEXTURE0 + textureUnit);

		// allocate a texture name
		glGenTextures(1, &m_textureID);

		// select our current texture
		glBindTexture(GL_TEXTURE_2D, m_textureID);

		//// select modulate to mix texture with color for shading
		//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		//// when texture area is small, bilinear filter the closest MIP map
		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		//	GL_LINEAR_MIPMAP_NEAREST);
		//// when texture area is large, bilinear filter the first MIP map
		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Load the image data into the texture unit.

		unsigned char* primero;
		int tamaño;
		tamaño = textureBitmap.ancho * textureBitmap.alto * 4;
		primero = new unsigned char[tamaño];

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureBitmap.ancho, textureBitmap.alto, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, textureBitmap.pixeles);// cambio de RGBA a BGRA

		// if wrap is true, the texture wraps over at the edges (repeat)
		//       ... false, the texture ends at the edges (clamp)
		// Set the texture color to either wrap around or clamp to the edge.
		if (wrap)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		}
		// Generate mipmaps for the texture.
		OpenGL->glGenerateMipmap(GL_TEXTURE_2D);

		//// build our texture MIP maps
		//gluBuild2DMipmaps(GL_TEXTURE_2D, 4, textureBitmap.ancho,
		//	textureBitmap.alto, GL_BGRA_EXT, GL_UNSIGNED_BYTE, textureBitmap.pixeles);

		return true;
	}
	catch (exception ex) {
		printf("Error,&s", ErrorString.c_str());
		MessageBoxA(NULL, ErrorString.c_str(), "Error", MB_OK);
		return false;
	}
}

unsigned int TextureClass::GetTextureID() {
	return m_textureID;
}

void TextureClass::CargaBitmapYRegresaDatos(string filename, int &ancho, int& alto, int& pixeles, unsigned int Direction) {
	string ErrorString = "";
	try {
		ifstream in(filename);

		if (in.fail()) {
			ErrorString = "Archivo Textura no encontrado o extensiones erroneas, favor de revisar - ";
			ErrorString += filename;
			throw std::invalid_argument("Archivo no encontrado");
		}

		std::wstring widestr = std::wstring(filename.begin(), filename.end());
		const wchar_t* szName = widestr.c_str();
		_bstr_t b(szName);
		const char* nombre = b;

		miBitmap NuevoBitmap = gdipLoad(b);
		alto = NuevoBitmap.alto;
		ancho = NuevoBitmap.ancho;
		pixeles = *NuevoBitmap.pixeles;
		Direction = *NuevoBitmap.Direction;
	}
	catch (exception ex) {
		printf("Error,&s", ErrorString.c_str());
		MessageBoxA(NULL, ErrorString.c_str(), "Error", MB_OK);
	}
}