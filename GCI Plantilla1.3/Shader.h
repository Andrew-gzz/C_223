#ifndef _SHADER_MODEL_H_
#define _SHADER_MODEL_H_
#include "openglclass.h"
#include <fstream>
#include <string>
using namespace std;

class Shader {
private:
	OpenGLClass* OpenGL;
	HWND hwnd;
	bool InitializeShader(const char*, const char*);
	char* LoadShaderSourceFile(const char*);
	void OutputShaderErrorMessage(unsigned int, const char*);
	void OutputLinkerErrorMessage(unsigned int);
	void ShutdownShader();
protected:
	virtual void BindAttributeLocation();
	virtual bool SetShaderParameters(float*, float*, float*);
	unsigned int m_vertexShader;
	unsigned int m_fragmentShader;
	unsigned int m_shaderProgram;
public:
	Shader(OpenGLClass* OpenGL, HWND hwnd);
	Shader(const Shader&);
	~Shader();
	bool Initialize( const char*, const char*);
	void Shutdown();
	void SetShader();
	bool PonMatriz4x4(char* nombre, float* matriz);

	bool Pon1Entero(char* nombre, unsigned int valor);

	bool PonVec3(char* nombre, float* vector);

	bool PonVec4(char* nombre, float* vector);
};
#endif // !_SHADER_MODEL_H_

