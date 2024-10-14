#ifndef _SHADER_BOUNDING_BOX_H_
#define _SHADER_BOUNDING_BOX_H_

#include "openglclass.h"
#include <Windows.h>
#include "Shader.h"

class ShaderBoundingBox : public Shader {
private:
	OpenGLClass* OpenGL;
	HWND hwnd;

protected:
	void BindAttributeLocation() {
		// Bind the shader input variables.
		OpenGL->glBindAttribLocation(m_shaderProgram, 0, "inputPosition");
	}
public:
	ShaderBoundingBox(OpenGLClass* OpenGL, HWND hwnd) : Shader(OpenGL, hwnd) {
		this->OpenGL = OpenGL;
		this->hwnd = hwnd;
	}
	ShaderBoundingBox(OpenGLClass* OpenGL, HWND hwnd, const char* vsFilename, const char* fsFilename) : Shader(OpenGL, hwnd) {
		this->OpenGL = OpenGL;
		this->hwnd = hwnd;
		Initialize(vsFilename, fsFilename);
	}
	~ShaderBoundingBox() {

	}
};

#endif // !_SHADER_BOUNDING_BOX_H_
