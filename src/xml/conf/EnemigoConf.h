/*
 * EnemigoConf.h
 *
 *  Created on: 14/06/2016
 *      Author: gvallerino
 */

#ifndef ENEMIGOCONF_H_
#define ENEMIGOCONF_H_

using namespace std;

typedef struct EnemigoConf {
    char tipo[1];
    char disparosSpriteID[30];
    int apareceEn;
    int velocidadDisparos;
	int x;
	int y;
} EnemigoConf;

#endif /* ENEMIGOCONF_H_ */
