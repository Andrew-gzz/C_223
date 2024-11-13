#include <windows.h>
#include <windowsx.h>
#include <random>
#include "openglclass.h"
#include "GameLogic.h"
#include "inputclass.h"
#include "Scene.h"
#include "KeyCode.h"
#include "timerclass.h"
#include "musica/libzplay.h"
#pragma comment (lib,"musica/libzplay.lib")

#pragma comment (lib,"Gdiplus.lib") 

///////////////////////////////
// PRE-PROCESSING DIRECTIVES //
///////////////////////////////
#define WIN32_LEAN_AND_MEAN

using namespace libZPlay;// Se declara que se usara la libreria de libZPlay para reproductor de musica
using namespace std; // Utiliza el espacio de nombres de la libreria STD de c++

#define Tick 100
#define Timer 100

struct Musica
{
	string Dir;
	string Nombre;
};
Musica Cancion[6];
ZPlay* player = CreateZPlay();//Generamos un objeto puntero para nuestro reproductor
ZPlay* player2 = CreateZPlay();
int LVolume = 25, RVolume = 25;
TStreamStatus status;
bool pausa = false;
void ReproductorPausa();
void ReproductorReproduce();
void ReproductorInicializaYReproduce();
void ReproductorCambiarCancionYReproduce(int);
void AudioNPC(int);

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<> distrib(0, 3);
// aca genera un numero random del 0 al 3
static int indexCancion = distrib(gen);

void ReproductorInicializa(ZPlay* players) {
	Cancion[0].Nombre = "Inicio 1";
	Cancion[0].Dir = "recursos/SoundTrack/Moog_City.mp3";
	Cancion[1].Nombre = "Inicio 2";
	Cancion[1].Dir = "recursos/SoundTrack/Aria_Math.mp3";
	Cancion[2].Nombre = "Inicio 3";
	Cancion[2].Dir = "recursos/SoundTrack/DKC__Aquatic_Ambience.mp3";
	Cancion[3].Nombre = "Inicio 4";
	Cancion[3].Dir = "recursos/SoundTrack/Wait.mp3";
	Cancion[4].Nombre = "Inicio 5";
	Cancion[4].Dir = "recursos/Audios/blablabla.mp3";
	Cancion[5].Nombre = "Inicio 6";
	Cancion[5].Dir = "recursos/Audios/blablabla.mp3";
}

void ReproductorInicializaYReproduce(ZPlay* Players) {
	Cancion[0].Nombre = "Inicio 1";
	Cancion[0].Dir = "recursos/SoundTrack/Moog_City.mp3";
	Cancion[1].Nombre = "Inicio 2";
	Cancion[1].Dir = "recursos/SoundTrack/Aria_Math.mp3";
	Cancion[2].Nombre = "Inicio 3";
	Cancion[2].Dir = "recursos/SoundTrack/DKC__Aquatic_Ambience.mp3";
	Cancion[3].Nombre = "Inicio 4";
	Cancion[3].Dir = "recursos/SoundTrack/Wait.mp3";
	Cancion[4].Nombre = "Inicio 5";
	Cancion[4].Dir = "recursos/Audios/blablabla.mp3";
	Cancion[5].Nombre = "Inicio 6";
	Cancion[5].Dir = "recursos/Audios/blablabla.mp3";

	//random_device rd;
	//mt19937 gen(rd());
	//uniform_int_distribution<> distrib(0, 3);
	//// aca genera un numero random del 0 al 3
	//int indexCancion = distrib(gen);

	ifstream inputFile(Cancion[indexCancion].Dir.c_str());

	if (!inputFile.good()) {
		MessageBoxA(NULL, "No file found", "No file found", MB_ICONERROR);
		return;
	}
	else {
		Players->OpenFile(Cancion[indexCancion].Dir.c_str(), sfAutodetect);
	}

	Players->SetMasterVolume(LVolume, RVolume);// Sonido tipo estereo Left and Right - Volumen de 0 - 100	
	//player->Play();
}

void ReproductorCambiarCancionYReproduce(int NumeroCancionAeproducir, ZPlay* Players) {
	Players->Stop();
	ifstream inputFile(Cancion[NumeroCancionAeproducir].Dir.c_str());

	if (!inputFile.good())
		printf("No file found");
	else
		Players->OpenFile(Cancion[NumeroCancionAeproducir].Dir.c_str(), sfAutodetect);
	Players->Play();
}

void ReproductorPausa(ZPlay* Players) {
	Players->Pause();
	pausa = true;
}

void ReproductorReproduce(ZPlay* Players) {
	if (pausa)
		Players->Resume();
	else
		Players->Play();
	pausa = false;
}

void AudioNPC(int pista, ZPlay* Players) {
	ReproductorInicializa(Players);
	Players->GetStatus(&status);
	static bool T = false;

	if (status.fPlay == 0 && T == false) { // inicia
		//MessageBoxA(NULL, "Inicia Reproduccion.", "musica", MB_ICONINFORMATION);
		ifstream inputFile(Cancion[pista].Dir.c_str()); 

		if (!inputFile.good()) { 
			MessageBoxA(NULL, "No file found", "No file found", MB_ICONERROR);
			return;
		}
		else {
			Players->OpenFile(Cancion[pista].Dir.c_str(), sfAutodetect);
		}

		Players->SetMasterVolume(LVolume, RVolume);// Sonido tipo estereo Left and Right - Volumen de 0 - 100	


		T = true;
	}
	else if (status.fPlay == 0 && T == true) { // finaliza audio

		Players->Stop();

		T = false;
	}
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);

LPCWSTR m_applicationName;
HINSTANCE m_hinstance;
HWND m_hwnd;

Scene* Escena;

OpenGLClass* m_OpenGL;
GameLogic* GLogic;
InputClass* m_Input;
TimerClass* m_Timer;
TimerClass* m_TimeDeltaTime;
const float sinlimite = 0;
const float fps1 = 1000 / 1;
const float fps12 = 1000 / 12;
const float fps24 = 1000 / 24;
const float fps30 = 1000 / 30;
const float fps45 = 1000 / 45;
const float fps60 = 1000 / 60;
const float fps120 = 1000 / 120;
float fpsOld = fps30; // aqui definimos a cuantos frames por segundo deseamos que vaya nuestro proyecto
float fps = fps30; // aqui definimos a cuantos frames por segundo deseamos que vaya nuestro proyecto
float timer = 0.0f;

// enable optimus!
extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 1;
	_declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	KeyBoard KeyCode;
	int screenWidth, screenHeight;
	bool result;
	m_OpenGL = 0;
	GLogic = 0;

	screenWidth = 0;
	screenHeight = 0;

	// Create the OpenGL object.
	m_OpenGL = new OpenGLClass;
	if (!m_OpenGL)
	{
		return false;
	}


	// Create the window the application will be using and also initialize OpenGL.
	WNDCLASS wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	// Get the instance of this application.
	m_hinstance = GetModuleHandle(NULL);

	// Give the application a name.
	m_applicationName = L"Nombre del proyecto";

	// Setup the windows class with default settings.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	//wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = (LPCWSTR)m_applicationName;
	//wc.cbSize = sizeof(WNDCLASSEX);

	// Register the window class.
	RegisterClass(&wc);

	// Create a temporary window for the OpenGL extension setup.
	m_hwnd = CreateWindow((LPCWSTR)m_applicationName, (LPCWSTR)m_applicationName, WS_POPUP,
		0, 0, 640, 480, NULL, NULL, m_hinstance, NULL);
	if (m_hwnd == NULL)
	{
		return false;
	}

	// Don't show the window.
	ShowWindow(m_hwnd, SW_HIDE);

	// Initialize a temporary OpenGL window and load the OpenGL extensions.
	result = m_OpenGL->InitializeExtensions(m_hwnd);
	if (!result)
	{
		MessageBoxA(m_hwnd, "Could not initialize the OpenGL extensions.", "Error", MB_OK);
		return false;
	}

	// Release the temporary window now that the extensions have been initialized.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// Determine the resolution of the clients desktop screen.
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if (FULL_SCREEN)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;
	}
	else
	{
		// If windowed then set it to 800x600 resolution.
		screenWidth = 800;
		screenHeight = 600;

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// Crea la pantalla de WinAPI y todas las configuraciones para manejarla.
	// hwnd = Handler window = Manejador de ventana
	// este objeto lo iremos delegando al resto de objetos que necesiten una ventana para funcionar adecuadamente.
	m_hwnd = CreateWindow((LPCWSTR)m_applicationName, (LPCWSTR)m_applicationName, WS_OVERLAPPEDWINDOW | WS_CAPTION | WS_POPUPWINDOW | WS_VISIBLE,
		posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);
	if (m_hwnd == NULL)
	{
		return false;
	}

	// Inicializa el entorno OpenGL abarcando toda la pantalla especificada.
	result = m_OpenGL->InitializeOpenGL(m_hwnd, screenWidth, screenHeight, SCREEN_DEPTH, SCREEN_NEAR, VSYNC_ENABLED);
	if (!result)
	{
		MessageBoxA(m_hwnd, "Could not initialize OpenGL, check if video card supports OpenGL 4.0. or 3.3",
			"Error", MB_OK);
		return false;
	}

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// Hide the mouse cursor.
	ShowCursor(true);

	// Crea el objeto que dictara la forma en que se comportara todo nuestro entorno grafico.
	GLogic = new GameLogic(m_OpenGL, m_hwnd);
	if (!GLogic)
	{
		return false;
	}

	m_Input = new InputClass();
	if (!m_Input) {
		MessageBoxA(m_hwnd, "Error inputclass", "Error al inicializar inputclass", MB_ICONERROR);
		return false;
	}
	m_Timer = new TimerClass;
	if (!m_Timer)
	{
		MessageBoxA(m_hwnd, "Error TimerClass", "Error al inicializar TimerClass", MB_ICONERROR);
		return false;
	}
	m_TimeDeltaTime = new TimerClass;
	if (!m_TimeDeltaTime)
	{
		MessageBoxA(m_hwnd, "Error TimerClass", "Error al inicializar TimerClass", MB_ICONERROR);
		return false;
	}

	MSG msg;
	bool done;


	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));

	// Loop until there is a quit message from the window or the user.
	done = false;

	TStreamStatus status;

	while (!done)
	{
		if (m_Input->GetKey(KeyCode.O)) {
			AudioNPC(4, player2);
		}

		player->GetStatus(&status);
		static bool T = false;

		if (status.fPlay == 0 && T == false) {
			//MessageBoxA(NULL, "Inicia Reproduccion.", "musica", MB_ICONINFORMATION);
			T = true;
		}
		else if (status.fPlay == 0 && T == true) {
			//MessageBoxA(NULL, "Termino la Reproduccion.", "musica", MB_ICONINFORMATION);
			random_device rd;
			mt19937 gen(rd());
			uniform_int_distribution<> distrib(0, 3);

			indexCancion = distrib(gen);

			ReproductorCambiarCancionYReproduce(indexCancion,player);

			T = false;
		}
		// Handle the windows messages.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		// If windows signals to end the application then exit out.
		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{	
			bool result;
			m_Timer->UpdateByFrame();
			timer += m_Timer->GetTime();
			m_Input->UpdateByFrame(msg);
			if (m_Input->GetKeyDown(KeyCode.F1)) {
				if (!pausa)
					ReproductorPausa(player);
				else
					ReproductorReproduce(player);
			}
			if (m_Input->GetKeyDown(KeyCode.F2)) {
				LVolume -= 5;
				RVolume -= 5;
				player->SetMasterVolume(LVolume, RVolume);// Sonido tipo estereo Left and Right - Volumen de 0 - 100
			}
			if (m_Input->GetKeyDown(KeyCode.F3)) {
				LVolume += 5;
				RVolume += 5;
				player->SetMasterVolume(LVolume, RVolume);// Sonido tipo estereo Left and Right - Volumen de 0 - 100
			}
			if (m_Input->GetKeyDown(KeyCode.F4)) {
				if (fpsOld == fps12)
					fpsOld = fps = fps24;
				else if (fpsOld == fps24)
					fpsOld = fps = fps30;
				else if (fpsOld == fps30)
					fpsOld = fps = fps45;
				else if (fpsOld == fps45)
					fpsOld = fps = fps60;
				else if (fpsOld == fps60)
					fpsOld = fps = fps120;
				else if (fpsOld == fps120)
					fpsOld = fps = fps12;
			}
			if (m_Input->GetKeyDown(KeyCode.F5)) {
				if (fpsOld != fps1)
					fpsOld = fps = fps1;
				else
					fpsOld = fps = fps24;
			}
			//if (m_Input->GetKeyDown(KeyCode.F6)) {
			//	GLogic->FormatRender();
			//}
			if (m_Input->GetKey(KeyCode.Escape) || m_Input->GetKeyXbox(KeyCode.BACK)) {
				if (MessageBoxA(m_hwnd, "Salir", "¿Esta seguro de salir?", MB_YESNO) == IDYES) {
					PostQuitMessage(0);
				}
			}
			else {
				if (timer > fps) {
					timer = 0.0f;
					m_TimeDeltaTime->UpdateByFrame();
					// Do the frame processing for the graphics object.
					result = GLogic->UpdateByFrame(m_Input, m_TimeDeltaTime->GetTime());
					if (!result)
					{
						return false;
					}
					else {
						player->GetStatus(&status);
						if (!pausa && status.fPlay == 0) {
							ReproductorReproduce(player);
						}
					}
				}
			}
		}

	}

	// Release the graphics object.
	if (GLogic)
	{
		GLogic->Shutdown();
		delete GLogic;
		GLogic = 0;
	}

	// Release the OpenGL object.
	if (m_OpenGL)
	{
		m_OpenGL->Shutdown(m_hwnd);
		delete m_OpenGL;
		m_OpenGL = 0;
	}
	if (m_Input)
	{
		m_Input->Shutdown();
		delete m_Input;
		m_Input = 0;
	}
	if (m_Timer)
	{
		delete m_Timer;
		m_Timer = 0;
	}
	if (m_TimeDeltaTime)
	{
		delete m_TimeDeltaTime;
		m_TimeDeltaTime = 0;
	}

	// Show the mouse cursor.
	// ShowCursor(true);

	// Fix the display settings if leaving full screen mode.
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// Remove the application instance.
	UnregisterClass((LPCWSTR)m_applicationName, m_hinstance);
	m_hinstance = NULL;

	return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
	case WM_CREATE:
	{
		SetTimer(hwnd, Timer, Tick, NULL);
		ReproductorInicializaYReproduce(player);
		return 0;
	}
	case WM_TIMER:
	{
		return 0;
	}
	// Check if the window is being closed.
	case WM_CLOSE:
	{
		//PostQuitMessage(0);
		return 0;
	}
	// Any other messages send to the default message handler as our application won't make use of them.
	default:
	{
		return DefWindowProc(hwnd, umessage, wparam, lparam);
	}

	}
}