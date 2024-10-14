////////////////////////////////////////////////////////////////////////////////
// Filename: textureclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _TEXTURECLASS_H_
#define _TEXTURECLASS_H_


//////////////
// INCLUDES //
//////////////
#include <stdio.h>


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include <windows.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <cmath>
#include "gdipload.h"
#include "openglclass.h"
#include <comdef.h>  // you will need this
#include <string>
#include <vector>
#include <fstream>

using std::vector;
using namespace std;

////////////////////////////////////////////////////////////////////////////////
// Class name: TextureClass
////////////////////////////////////////////////////////////////////////////////
class TextureClass
{
private:
	struct TargaHeader
	{
		unsigned char data1[12];
		unsigned short width;
		unsigned short height;
		unsigned char bpp;
		unsigned char data2;
	};
	unsigned int TexturesIndexLoaded = 0;
	bool LoadTextureTarga(string filename);
	bool LoadTextureOtherFormats(string filename);
	bool LoadTarga(string filename, unsigned int textureUnit, bool wrap);
	bool LoadOtherFormats(string filename, unsigned int textureUnit, bool wrap);
	bool loaded;
	unsigned int m_textureID;
	OpenGLClass* OpenGL;
	miBitmap textureBitmap;
public:
	void CargaBitmapYRegresaDatos(string filename, int &Ancho, int &alto, int &pixeles, unsigned int Direction);
	TextureClass(OpenGLClass* OpenGL);
	TextureClass(const TextureClass&);
	~TextureClass();

	unsigned int Initialize(string filename, bool wrap);
	unsigned int GetTextureID();
	void Shutdown();
	bool LiberarTextura();
};

#endif