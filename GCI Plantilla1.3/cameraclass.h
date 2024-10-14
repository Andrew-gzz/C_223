#ifndef _CAMERACLASS_H_
#define _CAMERACLASS_H_

#include <math.h>

class CameraClass
{
private:
	struct VectorType 
	{ 
		float x, y, z;
	};
	void MatrixRotationYawPitchRoll(float*, float, float, float);
	void TransformCoord(VectorType&, float*);
	void BuildViewMatrix(VectorType, VectorType, VectorType);
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	float m_viewMatrix[16];
public:
	CameraClass();
	CameraClass(float PosX, float PosY, float PosZ,
		float RotX, float RotY, float RotZ);
	CameraClass(const CameraClass&);
	~CameraClass();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	void Render();
	void GetViewMatrix(float*);
};

#endif