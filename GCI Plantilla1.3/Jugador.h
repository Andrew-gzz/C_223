#pragma once
#include"Inventario.h"

class Jugador {

private:

	Inventario Inv;
	float px, py, pz;
	int vida;
	int da�o;

public:
	Jugador(){
		px = 0;
		py = 0;
		pz = 0;
		vida = 50;
		da�o = 50;
	}
	Jugador(float pX,float pY, float pZ,int live,int damage){
		px = pX;
		py = pY;
		pz = pZ;
		vida = live;
		da�o = damage;
	}
	Jugador(float pX, float pY, float pZ, int live) {
		px = pX;
		py = pY;
		pz = pZ;
		vida = live;
	}
	Jugador(float pX, float pY, float pZ) {
		px = pX;
		py = pY;
		pz = pZ;
	}
	~Jugador(){}

	Inventario getInventario() { return Inv; }
	float getPosicionX() { return px; }
	float getPosicionY() { return py; }
	float getPosicionZ() { return pz; }
	int getVida() { return vida; }
	int getDa�o() { return da�o; }

	void setObjInventario(string NombreObjeto, int Espacio) { Inv.setObjt(NombreObjeto, Espacio); }
	void setObjInvt(Objetos Objeto, int Espacio) { Inv.setObjeto(Objeto, Espacio); }

	void setPosX(float pX) { px = pX; }
	void setPosY(float pY) { py = pY; }
	void setPosZ(float pZ) { pz = pZ; }

	void setVida(int HP) { vida = HP; }
	void setDamage(int ATK) { da�o = ATK; }


};