#pragma once
#include <chrono> 
#include <thread> 

using namespace std;

class ChancheColor {
public:
	void InterColor(float* color, const float* inicio, const float* fin, float t) {
		for (int i = 0; i < 4; i++) {
			color[i] = inicio[i] + (fin[i] - inicio[i]) * t;
		}
	}

	//void transicion(float* colorActual, const float* colorInicio, const float* colorFin, int duracionMs) {
	//	auto inicio = chrono::high_resolution_clock::now();
	//	auto fin = inicio + chrono::milliseconds(duracionMs);

	//	while (chrono::high_resolution_clock::now() < fin) {
	//		auto ahora = chrono::high_resolution_clock::now();
	//		chrono::duration<float> tiempoTranscurrido = ahora - inicio;
	//		float t = tiempoTranscurrido.count() / (duracionMs / 1000.0f);

	//		InterColor(colorActual, colorInicio, colorFin, t);

	//		// Espera un momento para no saturar la consola
	//		this_thread::sleep_for(chrono::milliseconds(50));
	//	}

	//	return;
	//}
private:

	float Color[4] = {0,0,0,0};

};