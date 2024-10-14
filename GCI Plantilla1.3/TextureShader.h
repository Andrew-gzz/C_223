#ifndef _SHADER_TEXTURE_H_
#define _SHADER_TEXTURE_H_

#include "Shader.h"

class ShaderTexture: public Shader {
private:
	OpenGLClass* OpenGL;
	HWND hwnd;

protected:
	void BindAttributeLocation() {
		// Bind the shader input variables.
		OpenGL->glBindAttribLocation(m_shaderProgram, 0, "inputPosition");
		OpenGL->glBindAttribLocation(m_shaderProgram, 1, "inputNormal");
		OpenGL->glBindAttribLocation(m_shaderProgram, 2, "inputTexCoord");
	}
public:
	ShaderTexture(OpenGLClass* OpenGL, HWND hwnd) : Shader(OpenGL, hwnd) {
		this->OpenGL = OpenGL;
		this->hwnd = hwnd;
	}
	ShaderTexture(OpenGLClass* OpenGL, HWND hwnd, const char* vsFilename, const char* fsFilename) : Shader(OpenGL, hwnd) {
		this->OpenGL = OpenGL;
		this->hwnd = hwnd;
		Initialize(vsFilename, fsFilename);
	}
	~ShaderTexture() {
	}
};

#endif // !_SHADER_TEXTURE_H_

