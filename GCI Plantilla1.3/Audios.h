#ifndef _AUDIOS_
#define _AUDIOS_

#include <iostream>
#include <windows.h>
#include <windowsx.h>
#include <fstream>
#include <random>
#include "musica/libzplay.h"
#pragma comment (lib,"musica/libzplay.lib")

using namespace libZPlay;
using namespace std;

ZPlay* player = CreateZPlay();
bool pause = false;

TStreamStatus status;

class Audios {
private:

	string Direccion;

public:

	Audios() { Direccion = ""; }
	Audios(string dir) { Direccion = dir; }
	~Audios(){}

	string GetDire() { return Direccion; }

	void setDir(string dir) { Direccion = dir; }

	void reproducirInicio(string Dir) {

		ifstream audio(Dir);

		if (!audio.good()) {
			MessageBoxA(NULL, "No file found", "No file found", MB_ICONERROR);
			return;
		}
		else{ player->OpenFile(Dir.c_str(), sfAutodetect); }

		player->SetMasterVolume(25, 25);// Sonido tipo estereo Left and Right - Volumen de 0 - 100
		
	}
	void ReproductorCambiarCancionYReproduce(string dir) {
		player->Stop();
		ifstream inputFile(dir);

		if (!inputFile.good())
			printf("No file found");
		else
			player->OpenFile(dir.c_str(), sfAutodetect);
		player->Play();
	}
	void AudioNPC(string Dir) {
		player->GetStatus(&status);
		static bool T = false;

		if (status.fPlay == 0 && T == false) { // inicia
			//MessageBoxA(NULL, "Inicia Reproduccion.", "musica", MB_ICONINFORMATION);
			reproducirInicio(Dir);
			T = true;
		}
		else if (status.fPlay == 0 && T == true) { // finaliza audio
			
			ReproductorPausa();

			T = false;
		}
	}

	void ReproductorPausa() {
		player->Pause();
		pause = true;
	}
	void ReproductorReproduce() {
		if (pause)
			player->Resume();
		else
			player->Play();
		pause = false;
	}


};

#endif