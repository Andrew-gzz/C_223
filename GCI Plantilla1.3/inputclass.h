#ifndef _INPUTCLASS_H_
#define _INPUTCLASS_H_

#include <Windows.h>
#include "gamepad.h"
#include "KeyCode.h"

class InputClass
{
private:
	bool ReadKeyboard();
	bool EventScroll(MSG);
	void ReadControllerXBOX(GAMEPAD_DEVICE);
	float Left_axis_x;
	float Left_axis_y;
	float Right_axis_x;
	float Right_axis_y;
	float Left_Z_tigger;
	float Right_Z_tigger;
	BOOL Btn_XBOX[13];
	BOOL BtnDOWN_XBOX[13];
	BOOL KEYSoldDOWN[256];
	BOOL Btn_XBOX_old[10];
	BOOL KEYSDOWN[256];
	BOOL KEYS[256];
	int m_screenWidth, m_screenHeight;
	GAMEPAD_DEVICE gamepad;
	POINT m_lastMousePosition;  // Para almacenar la última posición del mouse
	POINT m_currentMousePosition; // Posición actual del mouse

	void ReadMouseMovement(); // Nuevo método para leer el movimiento del mouse
public:
	InputClass();
	InputClass(int screenWidth, int screenHeight);
	InputClass(const InputClass&);
	~InputClass();

	bool InitializeWithMouse(int screenWidth, int screenHeight);
	bool InitializeWithoutMouse();
	void Shutdown();
	bool UpdateByFrame(MSG);


	unsigned char m_keyboardState[256];
	void MostrarCursor();
	void OcultarCursor();
	// resultX da 1 (el mouse se mueve hacia la derecha), si da -1 el mouse se mueve hacia la izquerda, cero no hace nada
	int resultX;
	//resultY da 1 se mueve hacia abajo, si da -1 se mueve hacia arriba, cero nada
	int resultY;
	//Scroll da > 1 lo mueves hacia adelante, si da < 1 lo mueves hacia atras
	int scroll;
	int GetMouseDirectionX();  // Devuelve la dirección X del mouse
	int GetMouseDirectionY();  // Devuelve la dirección Y del mouse 

	bool GetKeyXbox(KeyBoard::XBOX_Button);
	bool GetKeyDownXbox(KeyBoard::XBOX_Button);
	bool GetKey(KeyBoard::Keys);
	bool GetKeyDown(KeyBoard::Keys);
	float GetRightAxisY();
	float GetRightAxisX();
	float GetLefttAxisY();
	float GetLeftAxisX();
};

#endif