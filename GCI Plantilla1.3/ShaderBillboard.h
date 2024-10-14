#ifndef _BillboardShaderClass_H_
#define _BillboardShaderClass_H_

#include "openglclass.h"
#include "Shader.h"

class BillboardShaderClass : public Shader
{
private:
	OpenGLClass* OpenGL;
	HWND hwnd;
protected:
	void BindAttributeLocation() {
		// Bind the shader input variables.
		OpenGL->glBindAttribLocation(m_shaderProgram, 0, "inputPosition");
		OpenGL->glBindAttribLocation(m_shaderProgram, 1, "inputTexCoord");
	}
public:
	// Al mismo tiempo que obtenemos los parametros, ejecutamos el constructor de la clase padre
	BillboardShaderClass(OpenGLClass* OpenGL, HWND hwnd) : Shader(OpenGL, hwnd) {
		this->OpenGL = OpenGL;
		this->hwnd = hwnd;
	}
	// Al mismo tiempo que obtenemos los parametros, ejecutamos el constructor de la clase padre
	// y luego brindamos los archivos para inicializar el shader
	BillboardShaderClass(OpenGLClass* OpenGL, HWND hwnd, const char* vsFilename, const char* fsFilename) : Shader(OpenGL, hwnd) {
		this->OpenGL = OpenGL;
		this->hwnd = hwnd;
		Initialize(vsFilename, fsFilename);
	}
	~BillboardShaderClass() {
	}
};

#endif
