#ifndef SRC_VIEW_AVIONVIEW_H_
#define SRC_VIEW_AVIONVIEW_H_

#include "SDL2/SDL.h"

#include "Texture.h"
#include <string>
#include <iostream>

using namespace std;

class AvionView {

private:
	Texture *vistaAvionTexture = NULL;
	SDL_Renderer *rendererAvion;
	SDL_Rect clipsDerecha[ 2 ];
	SDL_Rect clipsIzquierda[ 2 ];
	SDL_Rect clipsVueltereta[ 6 ];
	SDL_Rect clipsEstacionado[ 1 ];
	string dir = "src/game/images/";
	string color;

	int anchoVista;
	int largoVista;

public:
	bool cargarImagenDelAvion( string filename );
	AvionView(SDL_Renderer* renderer, string color, string imageId);
	~AvionView();
	int getAnchoVista();
	int getLargoVista();
	void mostrar(int x, int y, int velX);
	void mostrarRotado(int x, int y, int velX, int velY);
	void mostrarVueltereta(int x, int y, int frame);
	void mostrarEstacionar(int x, int y, int frame);
	void cargarClips();
	void desconectar();
	void conectar();
	void ametralladora();
};

#endif /* SRC_VIEW_AVIONVIEW_H_ */
