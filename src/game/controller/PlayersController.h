/*
 * Controller.h
 *
 *  Created on: 30 de abr. de 2016
 *      Author: keynaka
 */

#ifndef SRC_CONTROL_PLAYERSCONTROLLER_H_
#define SRC_CONTROL_PLAYERSCONTROLLER_H_

#include "SDL2/SDL.h"
#include "IController.h"
#include "ControllerMissiles.h"
#include "../model/Avion.h"
#include "../../xml/conf/MisilConf.h"
#include "../../xml/conf/AvionConf.h"
#include "../../libs/data/game_data.h"

#include <iostream>
#include <string>

class PlayersController : public IController {

private:
	Vivible* obj;
	int velX,velY;
	const int distanciaDeDesplazamiento = 4;
	int velocidadStandard;
	Resolucion* resolucionPantalla;
	ControllerMissiles* controlDeMisiles;
	MisilConf* misilConf;
	bool crearMisil = false;
	bool desconectarAvion = false;
	GameData* gameData;

public:
	PlayersController(Vivible* unObj, SDL_Renderer* renderer, Resolucion* resolucion, GameData* gameData);
	~PlayersController();
	void press(SDL_Event *event);
	void hacerVivir();
	Vivible* getVivible();
	ControllerMissiles* getMissiles();
};

#endif /* SRC_CONTROL_PLAYERS_CONTROLLER_H_ */
