////////////////////////////////////////////////////////////////////////////////
// Filename: inputclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "inputclass.h"
#include <Windows.h>

InputClass::InputClass()
{
	scroll = 0;
	resultX = 0; // Inicializar las direcciones del mouse
	resultY = 0;
	m_lastMousePosition.x = 0; // Inicializar la posición del mouse
	m_lastMousePosition.y = 0;
	m_currentMousePosition.x = 0; 
	m_currentMousePosition.y = 0; 
	for (int i = 0; i < 256; i++) {
		KEYS[i] = 0;
		KEYSoldDOWN[i] = false;
		KEYSDOWN[i] = false;
	}
	Left_axis_x = 0.0f;
	Left_axis_y = 0.0f;
	Right_axis_x = 0.0f;
	Right_axis_y = 0.0f;
	Left_Z_tigger = 0.0f;
	Right_Z_tigger = 0.0f;
	gamepad = GAMEPAD_0;
	InitializeWithoutMouse();
}

InputClass::InputClass(int screenWidth, int screenHeight)
{
	scroll = 0;
	for (int i = 0; i < 256; i++) {
		KEYS[i] = 0;
		KEYSoldDOWN[i] = false;
		KEYSDOWN[i] = false;
	}
	Left_axis_x = 0.0f;
	Left_axis_y = 0.0f;
	Right_axis_x = 0.0f;
	Right_axis_y = 0.0f;
	Left_Z_tigger = 0.0f;
	Right_Z_tigger = 0.0f;
	gamepad = GAMEPAD_0;
	InitializeWithMouse(screenWidth, screenHeight);
}


InputClass::InputClass(const InputClass& other)
{
}


InputClass::~InputClass(){
	Shutdown();
}


bool InputClass::InitializeWithMouse(int screenWidth, int screenHeight)
{
	HRESULT result;

	// Guarda la informacion del tamaño de la ventana solo para cuando queremos utilizar el cursor.
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;
	// Permite inicializar el Gamepad
	GamepadInit();

	return true;
}

bool InputClass::InitializeWithoutMouse()
{
	HRESULT result;

	// Permite inicializar el Gamepad
	GamepadInit();

	return true;
}


bool InputClass::UpdateByFrame(MSG msg)
{
	bool result;

	// Read the current state of the keyboard.
	result = ReadKeyboard();
	if (!result)
	{
		return false;
	}

	//Lee el evento del scroll
	EventScroll(msg);
	ReadMouseMovement();  
	ReadControllerXBOX(gamepad);
	//ReadControllerXBOX(GAMEPAD_1);
	//ReadControllerXBOX(GAMEPAD_2);
	//ReadControllerXBOX(GAMEPAD_3);

	return true;
}

int InputClass::GetMouseDirectionX()
{
	return (resultX > 0) ? 1 : (resultX < 0) ? -1 : 0;
}

int InputClass::GetMouseDirectionY()
{
	return (resultY > 0) ? 1 : (resultY < 0) ? -1 : 0;
}
bool InputClass::ReadKeyboard()
{
	BOOL result;
	//Este arreglo tiene que tecla se presiono
	char keystate[256];
	//Instanciar el mouse con esta funcion
	result = GetKeyboardState((PBYTE)keystate);

	//Si manda 0 hay un error en el teclado
	if (result == 0)
		return false;

	//Pasar el buffer del teclado a un arreglo boleano
	for (int i = 0; i < 256; i++) {
		KEYS[i] = keystate[i] & 0x8000;

		if (KEYS[i] && KEYSDOWN[i] == 0 && KEYSoldDOWN[i] == 0)
		{
			KEYSDOWN[i] = KEYS[i];
			KEYSoldDOWN[i] = KEYS[i];
		}
		else if (KEYS[i] && KEYSDOWN[i] && KEYSoldDOWN[i])
		{
			KEYSDOWN[i] = 0;
		}
		else if (KEYS[i] == 0 && KEYSDOWN[i] == 0 && KEYSoldDOWN[i])
		{
			KEYSoldDOWN[i] = 0;
		}
	}



	return true;
}


void InputClass::ReadControllerXBOX(GAMEPAD_DEVICE dev)
{
	GamepadUpdate();
	if (GamepadIsConnected(dev)) {
		//Read controller
		GamepadStickNormXY(dev, STICK_LEFT, &Left_axis_x, &Left_axis_y);
		GamepadStickNormXY(dev, STICK_RIGHT, &Right_axis_x, &Right_axis_y);
		Left_Z_tigger = GamepadTriggerLength(dev, TRIGGER_LEFT);
		Right_Z_tigger = GamepadTriggerLength(dev, TRIGGER_RIGHT);
		Btn_XBOX[0] = GamepadButtonDown(dev, BUTTON_DPAD_UP);
		Btn_XBOX[1] = GamepadButtonDown(dev, BUTTON_DPAD_DOWN);
		Btn_XBOX[2] = GamepadButtonDown(dev, BUTTON_DPAD_LEFT);
		Btn_XBOX[3] = GamepadButtonDown(dev, BUTTON_DPAD_RIGHT);
		Btn_XBOX[4] = GamepadButtonDown(dev, BUTTON_BACK);
		Btn_XBOX[5] = GamepadButtonDown(dev, BUTTON_LEFT_THUMB);
		Btn_XBOX[6] = GamepadButtonDown(dev, BUTTON_RIGHT_THUMB);
		Btn_XBOX[7] = GamepadButtonDown(dev, BUTTON_LEFT_SHOULDER);
		Btn_XBOX[8] = GamepadButtonDown(dev, BUTTON_RIGHT_SHOULDER);
		Btn_XBOX[9] = GamepadButtonDown(dev, BUTTON_A);
		Btn_XBOX[10] = GamepadButtonDown(dev, BUTTON_B);
		Btn_XBOX[11] = GamepadButtonDown(dev, BUTTON_X);
		Btn_XBOX[12] = GamepadButtonDown(dev, BUTTON_Y);


		for (int i = 0; i < 13; i++) {
			if (Btn_XBOX[i] && BtnDOWN_XBOX[i] == 0 && Btn_XBOX_old[i] == 0)
			{
				BtnDOWN_XBOX[i] = Btn_XBOX[i];
				Btn_XBOX_old[i] = Btn_XBOX[i];
			}
			else if (Btn_XBOX[i] && BtnDOWN_XBOX[i] && Btn_XBOX_old[i])
			{
				BtnDOWN_XBOX[i] = 0;
			}
			else if (Btn_XBOX[i] == 0 && BtnDOWN_XBOX[i] == 0 && Btn_XBOX_old[i])
			{
				Btn_XBOX_old[i] = 0;
			}
		}
		return;
	}
	return;
}
bool InputClass::GetKeyXbox(KeyBoard::XBOX_Button key ) {
	return GamepadIsConnected(gamepad) ? Btn_XBOX[key] : false;
}
bool InputClass::GetKeyDownXbox(KeyBoard::XBOX_Button key) {
	return GamepadIsConnected(gamepad) ? BtnDOWN_XBOX[key] : false;
}
bool InputClass::GetKey(KeyBoard::Keys key) {
	return KEYS[key];
}
bool InputClass::GetKeyDown(KeyBoard::Keys key) {
	return KEYSDOWN[key];
}
float InputClass::GetRightAxisY() {
	return GamepadIsConnected(gamepad) ? Right_axis_y : 0.0f;
}
float InputClass::GetRightAxisX() {
	return GamepadIsConnected(gamepad) ? Right_axis_x : 0.0f;
}
float InputClass::GetLefttAxisY() {
	return GamepadIsConnected(gamepad) ? Left_axis_y : 0.0f;
}
float InputClass::GetLeftAxisX() {
	return GamepadIsConnected(gamepad) ? Left_axis_x : 0.0f;
}

//Muestra el cursor
void InputClass::MostrarCursor()
{
	ShowCursor(TRUE);
	return;
}
//Oculta el cursor
void InputClass::OcultarCursor()
{
	ShowCursor(FALSE);
	return;
}
//Evento para dectectar el scorll
bool InputClass::EventScroll(MSG msg)
{
	//la variable MSG tiene todo lo que hace la ventana
	if (msg.message == WM_MOUSEWHEEL)
	{
		scroll = (short)HIWORD(msg.wParam);
		scroll /= WHEEL_DELTA;
		return true;
	}
	scroll = 0;
	
	return true;
}
void InputClass::ReadMouseMovement()
{
	// Obtiene la posición actual del mouse
	POINT mousePos; 
	GetCursorPos(&mousePos); 

	// Convertir la posición del mouse a coordenadas relativas a la ventana
	ScreenToClient(GetForegroundWindow(), &mousePos); 

	// Actualizar la posición actual del mouse
	m_currentMousePosition = mousePos;

	// Calcular las diferencias entre la posición actual y la última
	resultX = m_currentMousePosition.x - m_lastMousePosition.x;
	resultY = m_currentMousePosition.y - m_lastMousePosition.y;

	// Actualizar la última posición del mouse para el próximo frame
	m_lastMousePosition = m_currentMousePosition;
}
void InputClass::Shutdown()
{
	GamepadShutdown();
	return;
}