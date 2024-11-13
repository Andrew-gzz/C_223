#pragma once
#include "Objetos.h"

class Armas {
private:

	Objetos Arma;
	std::string Tipo;
	int Municion;
	int cargas;

public:

	Armas() {
		Arma.setNomreObj("Manos");
		Tipo = "Meele";
		Municion = 0;
		cargas = 0;
	}

	Armas(Objetos arm, string Tipe, int mun, int carg) {
		Arma = arm;
		Tipo = Tipe;
		Municion = mun;
		cargas = carg;
	}

	Armas(string Tipe, int mun, int carg) {
		Arma.setNomreObj("Manos");
		Tipo = Tipe;
		Municion = mun;
		cargas = carg;
	}

	Armas(string Tipe) {
		Tipo = Tipe;
		if (Tipe == "Melee") {
			Municion = 0;
			cargas = 0;
		}
	}

	Armas(Objetos arm, string Tipe) {
		Arma = arm;
		Tipo = Tipe;
		if (Tipe == "Melee") {
			Municion = 0;
			cargas = 0;
		}
	}

	~Armas() {}

	Objetos getArma() { return Arma.getNombreObj(); }
	string getTipoDeArma() { return Tipo; }
	int getMunicion() { return Municion; }
	int getCargas() { return cargas; }

	void setArma(Objetos obj) { Arma = obj; }
	void setArm(std::string Nom) { Arma.setNomreObj(Nom); }
	void setTipoDeArma(string Tipe) { Tipo = Tipe; }
	void setMunicion(int Balas) { Municion = Balas; }
	void setCargas(int Cartuchos) { cargas = Cartuchos; }

};