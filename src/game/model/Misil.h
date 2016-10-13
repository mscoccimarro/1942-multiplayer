/*
 * Misil.h
 *
 *  Created on: 30 de abr. de 2016
 *      Author: ramon
 */

#ifndef SRC_GAME_MODEL_MISIL_H_
#define SRC_GAME_MODEL_MISIL_H_

#include "Vivible.h"
#include "../controller/Timer.h"
#include "../view/MisilView.h"
#include "../model/Resolucion.h"
#include "../model/Posicion.h"
#include "../model/Sound.h"
#include "../../xml/conf/MisilConf.h"

using namespace std;

class Misil: public Vivible {
private:
	int anchoFondo, largoFondo;
	int dano;
	int velocidadDisparo;
	MisilView* vistaMisil;
	Posicion* posicion;
	Sound* sonido;

	Timer* t;

	void mostrar();
	void mover(int velX, int velY);
	void moverEjeX(int velX);
	void moverEjeY(int velY);

public:
	Misil(SDL_Renderer * renderer, Posicion* posicion, Resolucion* resolucion, MisilConf* config);
	virtual ~Misil();

	void vivir(int x,int y);
	int getAnchoFondo();
	int getLargoFondo();
	int getAncho();
	int getLargo();
	int getVelocidadDisparo();
	bool aunVive();
	int getDano();
	int getX();
	int getY();
	Posicion* getPosicion();

	void setAmetralladora();
	void morir();
};

#endif /* SRC_GAME_MODEL_MISIL_H_ */
