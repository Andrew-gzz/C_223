#ifndef _LIGHTSHADERCLASS_H_
#define _LIGHTSHADERCLASS_H_

#include "openglclass.h"
#include "Shader.h"

class LightShaderClass : public Shader
{
private:
	OpenGLClass* OpenGL;
	HWND hwnd;
protected:
	void BindAttributeLocation() {
		// Bind the shader input variables.
		OpenGL->glBindAttribLocation(m_shaderProgram, 0, "inputPosition");
		OpenGL->glBindAttribLocation(m_shaderProgram, 1, "inputNormal");
		OpenGL->glBindAttribLocation(m_shaderProgram, 2, "inputTexCoord");
		OpenGL->glBindAttribLocation(m_shaderProgram, 3, "inputTangent");
		OpenGL->glBindAttribLocation(m_shaderProgram, 4, "inputBinormal");
	}
public:
	// Al mismo tiempo que obtenemos los parametros, ejecutamos el constructor de la clase padre
	LightShaderClass(OpenGLClass* OpenGL, HWND hwnd) : Shader(OpenGL, hwnd) {
		this->OpenGL = OpenGL;
		this->hwnd = hwnd;
	}
	// Al mismo tiempo que obtenemos los parametros, ejecutamos el constructor de la clase padre
	// y luego brindamos los archivos para inicializar el shader
	LightShaderClass(OpenGLClass* OpenGL, HWND hwnd, const char* vsFilename, const char* fsFilename) : Shader(OpenGL, hwnd) {
		this->OpenGL = OpenGL;
		this->hwnd = hwnd;
		Initialize(vsFilename, fsFilename);
	}
	~LightShaderClass() {
	}
};

#endif