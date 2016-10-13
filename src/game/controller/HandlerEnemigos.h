/*
 * HandlerEnemigos.h
 *
 *  Created on: 17 de jun. de 2016
 *      Author: keynaka
 */

#ifndef SRC_GAME_CONTROLLER_HANDLERENEMIGOS_H_
#define SRC_GAME_CONTROLLER_HANDLERENEMIGOS_H_

#include "SDL2/SDL.h"
#include <map>
#include "IController.h"
#include "PlayersController.h"
#include "../model/Avion.h"
#include "../model/Enemy.h"
#include "../model/EnemyMediano.h"
#include "../model/EnemyGrande.h"
#include "../model/Vivible.h"
#include "../../libs/data/game_data.h"
#include "../view/Screen.h"
#include "../../xml/conf/GameConf.h"

class HandlerEnemigos {

private:
	SDL_Renderer* renderer;
	Resolucion* resolucion;
	Screen* screen;
	GameConf* gc;
	int velocidadStandard;
	int velX,velY;
	map<string, Avion*> mapaAvionesApuntables;

public:
	map<int, Enemy*> mapaEnemigos;
	Enemy* getEnemigo(int id);
	HandlerEnemigos(SDL_Renderer* renderer, Resolucion* resolucion, Screen* screen, GameConf* gc);
	virtual ~HandlerEnemigos();
	Posicion* mover(int id, char event);
	void createEnemigo(int id, char type, int posX, int posY);
	void hacerVivir();
	void matar(int id);
	void bajarHP( int id );
	void deleteEnemys();
	void clearEnemies();
	void addAvionesApuntables(Avion* avion);
	void setAvionApuntar(int idEnemigo, string idAvion);
};
#endif /* SRC_GAME_CONTROLLER_HANDLERENEMIGOS_H_ */
