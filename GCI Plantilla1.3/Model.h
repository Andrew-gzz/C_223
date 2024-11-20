#ifndef _MODEL_
#define _MODEL_
#include "openglclass.h"
#include "TextureLoaderForOpenGL.h"
#include <math.h>
#include <fstream>
#include <vector>
#include <string>
#include "Shader.h"
#include "BoundingBox.h"

using std::vector;
using namespace std;

class Model
	//class Model: public TexLoader
{
private:
	struct TempVertexType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

	struct VectorType
	{
		float x, y, z;
	};
	struct VertexType {
		float Posx, Posy, Posz;
		float Normx, Normy, Normz;
		float u, v;
		float Tx, Ty, Tz;
		float Bx, By, Bz;
	};
	class GLFLOAT3
	{
	public:
		float x;
		float y;
		float z;
		GLFLOAT3(float x, float y, float z) {
			this->x = x;
			this->y = y;
			this->z = z;
		};
	};
	//Buffer Model
	struct ModelType {

		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
		float tx, ty, tz;
		float bx, by, bz;
	};
	typedef struct
	{
		//v
		float x, y, z;
		//vt
		float tu, tv;
		//vn
		float nx, ny, nz;
		//float ft4, fv4, fn4;

		//f       v      /      vt           /     vn
		int ft3, fv3, fn3; int ft2, fv2, fn2; int ft1, fv1, fn1;
	}VertexLoad;
	OpenGLClass* OpenGL;
	HWND HandlerWindow;
	float* world;
	GLFLOAT3* min, * max;
	float r;
	GLFLOAT3* center;
	GLuint Texturas;
	int textureNormalID;
	unsigned int m_vertexArrayId, m_vertexBufferId, m_indexBufferId;
	unsigned int TextureID[5];
	int MaxTexturesPerModel;
	TextureClass* LoaderTextureReference;
	BoundingBox* box;
	BoundingBox* box2;
protected:

	//Important
	int m_indexCount, m_vertexCount;
	ModelType* m_model;
public:
	Model();
	Model(OpenGLClass* OpenGL, HWND hwnd, TextureClass* LoaderTexRef);
	Model(const Model&);
	~Model();
	//LoadModel obj or txt
	bool LoadModel(const char*);
	int GetIndexCount();
	/*Read Model*/
	bool ReadFileObj(const char*);
	bool ReadFileTxt(const char*);
	void ReleaseModel();
	//Toma un .obj para luego cargarlo en Ram
	bool First_LoadConvertObjModelsToBuffers(const char*);
	//Abre y rectifica que el modelo sea correcto y no este dañado
	bool Second_RectifyModelFile(const char*);
	//Carga en ram todo el archivo .obj
	bool Third_LoadInRamModelFile(const char*, int&, int&, int&, int&);
	//Beta de carga de obj en automatico carga los materiales
	bool LoadModelObj(const char*);
	//Sistema de carga por .obj
	bool Fourth_LoadModelByObj(vector<VertexLoad>, int);
	bool CargarBuffersOpenGL(std::vector<VertexLoad>);
	void FindBoundingBox(ModelType*);
	void CalculateModelVectors();
	void CalculateTangentBinormal(TempVertexType, TempVertexType, TempVertexType, VectorType&, VectorType&);
	void CalculateNormal(VectorType, VectorType, VectorType&);
	virtual bool Initialize(const char* objPath, string texturePath, string normalMappingPath);
	virtual bool Initialize(const char* objPath, string texturePath);
	//virtual bool LoadTexture(const wchar_t*, int);
	virtual bool AddTexture(string, int);
	virtual void Shutdown(OpenGLClass*);
	virtual bool Frame();
	virtual bool Render(Shader* shader);
	bool InitializeBuffers(OpenGLClass*);
	bool CreateBoundingBox(OpenGLClass*);
	void ChangeTexture(int From, int To);
	BoundingBox* GetBox();
};

#endif // !_MODEL_
