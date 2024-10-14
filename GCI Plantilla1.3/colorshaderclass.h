#ifndef _COLORSHADERCLASS_H_
#define _COLORSHADERCLASS_H_

#include "openglclass.h"
#include "Shader.h"

class ColorShaderClass: public Shader
{
private:
	OpenGLClass* OpenGL;
	HWND hwnd;
public:
	ColorShaderClass(OpenGLClass* OpenGL, HWND hwnd) : Shader(OpenGL, hwnd) {
		this->OpenGL = OpenGL;
		this->hwnd = hwnd;
	}
	ColorShaderClass(OpenGLClass* OpenGL, HWND hwnd, const char* vsFilename, const char* fsFilename) : Shader(OpenGL, hwnd) {
		this->OpenGL = OpenGL;
		this->hwnd = hwnd;
		Initialize(vsFilename, fsFilename);
	}
	~ColorShaderClass() {

	}
protected:
	void BindAttributeLocation() {
		// Bind the shader input variables.
		OpenGL->glBindAttribLocation(m_shaderProgram, 0, "inputPosition");
		OpenGL->glBindAttribLocation(m_shaderProgram, 1, "inputColor");
		return;
	}

};

#endif