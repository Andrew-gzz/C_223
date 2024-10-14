#include "cameraclass.h"

CameraClass::CameraClass()
{
	m_positionX = 0.0f;
	m_positionY = 0.0f;
	m_positionZ = 0.0f;

	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;
}

CameraClass::CameraClass(float PosX, float PosY, float PosZ, 
					float RotX, float RotY, float RotZ)
{
	m_positionX = PosX;
	m_positionY = PosY;
	m_positionZ = PosZ;

	m_rotationX = RotX;
	m_rotationY = RotY;
	m_rotationZ = RotZ;
}

CameraClass::CameraClass(const CameraClass& other)
{
}

CameraClass::~CameraClass()
{
}

void CameraClass::SetPosition(float x, float y, float z)
{
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;
	return;
}

void CameraClass::SetRotation(float x, float y, float z)
{
	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;
	return;
}

void CameraClass::Render()
{
	VectorType up, position, lookAt;
	float yaw, pitch, roll;
	float rotationMatrix[9];


	// Definimos el vector que apunta hacia arriba.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// Definimos la posicion de la camara en el mundo.
	position.x = m_positionX;
	position.y = m_positionY;
	position.z = m_positionZ;

	// Definimos la direccion de la vista de la camara.
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	// Definimos las rotaciones yaw (Y axis), pitch (X axis), y roll (Z axis) en radianes.
	pitch = m_rotationX * 0.0174532925f;
	yaw   = m_rotationY * 0.0174532925f;
	roll  = m_rotationZ * 0.0174532925f;

	// creamos la matriz de rotacion a partir de yaw, pitch, y roll.
	MatrixRotationYawPitchRoll(rotationMatrix, yaw, pitch, roll);

	// Interpretamos la Vista y el vector que apunto arriba por la rotacion de la matriz
	// dando asi una vista correctamente rotada al origen.
	TransformCoord(lookAt, rotationMatrix);
	TransformCoord(up, rotationMatrix);
	
	// Traslada la posicion de la camara acorde a la ubicacion del personaje.
	lookAt.x = position.x + lookAt.x;
	lookAt.y = position.y + lookAt.y;
	lookAt.z = position.z + lookAt.z;

	// Finalmente creamos la matriz de vista a partir de los 3 vectores.
	BuildViewMatrix(position, lookAt, up);

	return;
}

void CameraClass::MatrixRotationYawPitchRoll(float* matrix, float yaw, float pitch, float roll)
{
	float cYaw, cPitch, cRoll, sYaw, sPitch, sRoll;


	// Get the cosine and sin of the yaw, pitch, and roll.
	cYaw = cosf(yaw);
	cPitch = cosf(pitch);
	cRoll = cosf(roll);

	sYaw = sinf(yaw);
	sPitch = sinf(pitch);
	sRoll = sinf(roll);

	// Calculate the yaw, pitch, roll rotation matrix.
	matrix[0] = (cRoll * cYaw) + (sRoll * sPitch * sYaw);
	matrix[1] = (sRoll * cPitch);
	matrix[2] = (cRoll * -sYaw) + (sRoll * sPitch * cYaw);
	
	matrix[3] = (-sRoll * cYaw) + (cRoll * sPitch * sYaw);
	matrix[4] = (cRoll * cPitch);
	matrix[5] = (sRoll * sYaw) + (cRoll * sPitch * cYaw);
	
	matrix[6] = (cPitch * sYaw);
	matrix[7] = -sPitch;
	matrix[8] = (cPitch * cYaw);

	return;
}

void CameraClass::TransformCoord(VectorType& vector, float* matrix)
{
	float x, y, z;


	// Transform the vector by the 3x3 matrix.
	x = (vector.x * matrix[0]) + (vector.y * matrix[3]) + (vector.z * matrix[6]);
	y = (vector.x * matrix[1]) + (vector.y * matrix[4]) + (vector.z * matrix[7]);
	z = (vector.x * matrix[2]) + (vector.y * matrix[5]) + (vector.z * matrix[8]);

	// Store the result in the reference.
	vector.x = x;
	vector.y = y;
	vector.z = z;

	return;
}

void CameraClass::BuildViewMatrix(VectorType position, VectorType lookAt, VectorType up)
{
	VectorType zAxis, xAxis, yAxis;
	float length, result1, result2, result3;


	// zAxis = normal(lookAt - position)
	zAxis.x = lookAt.x - position.x;
	zAxis.y = lookAt.y - position.y;
	zAxis.z = lookAt.z - position.z;
	length = sqrt((zAxis.x * zAxis.x) + (zAxis.y * zAxis.y) + (zAxis.z * zAxis.z));
	zAxis.x = zAxis.x / length;
	zAxis.y = zAxis.y / length;
	zAxis.z = zAxis.z / length;

	// xAxis = normal(cross(up, zAxis))
	xAxis.x = (up.y * zAxis.z) - (up.z * zAxis.y);
	xAxis.y = (up.z * zAxis.x) - (up.x * zAxis.z);
	xAxis.z = (up.x * zAxis.y) - (up.y * zAxis.x);
	length = sqrt((xAxis.x * xAxis.x) + (xAxis.y * xAxis.y) + (xAxis.z * xAxis.z));
	xAxis.x = xAxis.x / length;
	xAxis.y = xAxis.y / length;
	xAxis.z = xAxis.z / length;

	// yAxis = cross(zAxis, xAxis)
	yAxis.x = (zAxis.y * xAxis.z) - (zAxis.z * xAxis.y);
	yAxis.y = (zAxis.z * xAxis.x) - (zAxis.x * xAxis.z);
	yAxis.z = (zAxis.x * xAxis.y) - (zAxis.y * xAxis.x);

	// -dot(xAxis, position)
	result1 = ((xAxis.x * position.x) + (xAxis.y * position.y) + (xAxis.z * position.z)) * -1.0f;

	// -dot(yaxis, eye)
	result2 = ((yAxis.x * position.x) + (yAxis.y * position.y) + (yAxis.z * position.z)) * -1.0f;

	// -dot(zaxis, eye)
	result3 = ((zAxis.x * position.x) + (zAxis.y * position.y) + (zAxis.z * position.z)) * -1.0f;

	// Set the computed values in the view matrix.
	m_viewMatrix[0]  = xAxis.x;
	m_viewMatrix[1]  = yAxis.x;
	m_viewMatrix[2]  = zAxis.x;
	m_viewMatrix[3]  = 0.0f;

	m_viewMatrix[4]  = xAxis.y;
	m_viewMatrix[5]  = yAxis.y;
	m_viewMatrix[6]  = zAxis.y;
	m_viewMatrix[7]  = 0.0f;

	m_viewMatrix[8]  = xAxis.z;
	m_viewMatrix[9]  = yAxis.z;
	m_viewMatrix[10] = zAxis.z;
	m_viewMatrix[11] = 0.0f;

	m_viewMatrix[12] = result1;
	m_viewMatrix[13] = result2;
	m_viewMatrix[14] = result3;
	m_viewMatrix[15] = 1.0f;

	return;
}

void CameraClass::GetViewMatrix(float* matrix)
{
	matrix[0]  = m_viewMatrix[0];
	matrix[1]  = m_viewMatrix[1];
	matrix[2]  = m_viewMatrix[2];
	matrix[3]  = m_viewMatrix[3];

	matrix[4]  = m_viewMatrix[4];
	matrix[5]  = m_viewMatrix[5];
	matrix[6]  = m_viewMatrix[6];
	matrix[7]  = m_viewMatrix[7];

	matrix[8]  = m_viewMatrix[8];
	matrix[9]  = m_viewMatrix[9];
	matrix[10] = m_viewMatrix[10];
	matrix[11] = m_viewMatrix[11];

	matrix[12] = m_viewMatrix[12];
	matrix[13] = m_viewMatrix[13];
	matrix[14] = m_viewMatrix[14];
	matrix[15] = m_viewMatrix[15];

	return;
}