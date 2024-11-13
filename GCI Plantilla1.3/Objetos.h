
#pragma once
#include<iostream>

using namespace std;

class Objetos {
public:
	Objetos() { Nom = "BACIO"; }
	Objetos(std::string Name) { Nom = Name; }
	~Objetos() {}

	std::string getNombreObj() { return Nom; }

	void setNomreObj(std::string Name) { Nom = Name; }

private:

	std::string Nom;

};