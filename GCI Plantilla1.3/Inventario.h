#pragma once
#include "Objetos.h"

class Inventario {
public:

	Objetos getObjeto(int esp) { return Objeto[esp].getNombreObj(); } // obtienes el objeto en si

	void setObjeto(Objetos Obj, int esp) { Objeto[esp] = Obj; } // aqui solamente se guardara un objeto ya creado en el main
	void setObjt(string nom, int esp) { Objeto[esp].setNomreObj(nom); } // aqui lo agregas directamente

private:

	Objetos Objeto[6];

};