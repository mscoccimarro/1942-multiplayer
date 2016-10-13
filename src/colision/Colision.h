#ifndef COLISION_H
#define COLISION_H

#include "../game/model/Vivible.h"
#include "../game/model/Enemy.h"
#include "../game/view/ChispazoView.h"
#include "../game/view/Screen.h"

#include <iostream>
using namespace std;

class Colision {

private:
	static bool inside(int, int, int, int, int, int);
	static bool norte(Vivible* unVivible, Vivible* otroVivible);
	static bool sur(Vivible* unVivible, Vivible* otroVivible);
	static bool este(Vivible* unVivible, Vivible* otroVivible);
	static bool oeste(Vivible* unVivible, Vivible* otroVivible);

	static bool norte(Vivible* unVivible, Enemy* otroVivible);
	static bool sur(Vivible* unVivible, Enemy* otroVivible);
	static bool este(Vivible* unVivible, Enemy* otroVivible);
	static bool oeste(Vivible* unVivible, Enemy* otroVivible);

public:
	static bool is(Vivible* unVivible, Vivible* otroVivible);
	static bool is(Vivible* unVivible, Enemy* otroVivible);
	static bool is(Misil* unVivible, Enemy* otroVivible);

	static void view(Vivible* unVivible, Vivible* otroVivible, Screen* screen);
	static void view(Misil* unVivible, Enemy* otroVivible, Screen* screen);
};

#endif
