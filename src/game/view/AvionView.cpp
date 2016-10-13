#include "AvionView.h"

#include <math.h>

using namespace std;

void AvionView::ametralladora(){
	string filename = "ametralladora.bmp";
}

void AvionView::desconectar(){
	string filename = "AvionSprites_gris.bmp";

	if (!cargarImagenDelAvion(filename)) {
		cout << "Vista del avion no ha sido cargada correctamente.." << endl;
	}
}

void AvionView::conectar(){
	string filename = "AvionSprites_" + color + ".bmp";

	if (!cargarImagenDelAvion(filename)) {
		cout << "Vista del avion no ha sido cargada correctamente.." << endl;
	}
}

void AvionView::cargarClips(){
	int posYInicial = 50;

		clipsDerecha[ 0 ].x = 40;
		clipsDerecha[ 0 ].y = posYInicial+5;
		clipsDerecha[ 0 ].w = anchoVista-10;
		clipsDerecha[ 0 ].h = largoVista;

		clipsDerecha[ 1 ].x = 1010;
		clipsDerecha[ 1 ].y = posYInicial+5;
		clipsDerecha[ 1 ].w = anchoVista-10;
		clipsDerecha[ 1 ].h = largoVista;

		clipsIzquierda[ 0 ].x = 40;
		clipsIzquierda[ 0 ].y = posYInicial+5;
		clipsIzquierda[ 0 ].w = anchoVista-10;
		clipsIzquierda[ 0 ].h = largoVista;

		clipsIzquierda[ 1 ].x = 865;
		clipsIzquierda[ 1 ].y = posYInicial+5;
		clipsIzquierda[ 1 ].w = anchoVista-10;
		clipsIzquierda[ 1 ].h = largoVista;

		clipsVueltereta[ 0 ].x = 40;
		clipsVueltereta[ 0 ].y = posYInicial+5;
		clipsVueltereta[ 0 ].w = anchoVista-10;
		clipsVueltereta[ 0 ].h = largoVista;

		clipsVueltereta[ 1 ].x = 175;
		clipsVueltereta[ 1 ].y = posYInicial-20;
		clipsVueltereta[ 1 ].w = anchoVista-10;
		clipsVueltereta[ 1 ].h = largoVista+25;

		clipsVueltereta[ 2 ].x = 310;
		clipsVueltereta[ 2 ].y = posYInicial-20;
		clipsVueltereta[ 2 ].w = anchoVista;
		clipsVueltereta[ 2 ].h = largoVista+50;

		clipsVueltereta[ 3 ].x = 410;
		clipsVueltereta[ 3 ].y = posYInicial-25;
		clipsVueltereta[ 3 ].w = anchoVista+75;
		clipsVueltereta[ 3 ].h = largoVista+75;

		clipsVueltereta[ 4 ].x = 560;
		clipsVueltereta[ 4 ].y = posYInicial;
		clipsVueltereta[ 4 ].w = anchoVista+50;
		clipsVueltereta[ 4 ].h = largoVista+50;

		clipsVueltereta[ 5 ].x = 720;
		clipsVueltereta[ 5 ].y = posYInicial+15;
		clipsVueltereta[ 5 ].w = anchoVista+10;
		clipsVueltereta[ 5 ].h = largoVista+45;

		clipsEstacionado[ 0 ].x = 30;
		clipsEstacionado[ 0 ].y = 0;
		clipsEstacionado[ 0 ].w = anchoVista+20;
		clipsEstacionado[ 0 ].h = largoVista-5;

}

bool AvionView::cargarImagenDelAvion( string filename ){
	vistaAvionTexture = new Texture( rendererAvion );

	if (rendererAvion == NULL){
		printf("nulo");
	}

	if (!vistaAvionTexture->loadFromFile(filename)) {
		printf("Failed to load avion texture image!\n");
		return false;
	}

    return true;
}

AvionView::AvionView(SDL_Renderer * unRenderer, string color, string filename){
	anchoVista = 70;
	largoVista = 60;
	this->color = color;

	cargarClips();
	rendererAvion = unRenderer;

	if (!cargarImagenDelAvion(filename+"_"+color+".bmp")) {
		cout << "Vista del avion no ha sido cargada correctamente.." << endl;
	}

}

int AvionView::getAnchoVista(){
		return anchoVista;
}

int AvionView::getLargoVista(){
		return largoVista;
}

AvionView::~AvionView(){
	vistaAvionTexture->free();
}

void AvionView::mostrar( int x, int y, int velX){
	bool izquierda = true;
	int frame = 0;

  if (velX > 0){
  	izquierda = false;
  }
  if (velX != 0){
    frame ++;
  }

  SDL_Rect* currentClip;
  if(izquierda){
  	currentClip = &clipsIzquierda[ frame ];
  }else{
  	currentClip = &clipsDerecha[ frame ];
  }
  vistaAvionTexture->render( x, y, currentClip );
}

void AvionView::mostrarRotado( int x, int y, int velX, int velY){
  bool izquierda = true;
  int frame = 0;
  SDL_Rect* currentClip = &clipsIzquierda[0];
  if(velX == 0 && velY == 0) {
  	velY = -1;
  	velX = 0;
  }
  double mod = pow(velX * velX + velY * velY, 0.5);
  double angle = asin(velY / mod);
  angle = angle * 180 / M_PI;
  if (velX < 0) angle  = 180 - angle;
  vistaAvionTexture->renderWithAngle( x, y, currentClip, 90 - angle);
}

void AvionView::mostrarVueltereta(int x, int y, int frame){
  SDL_Rect* currentClip;
  currentClip = &clipsVueltereta[ frame ];

  vistaAvionTexture->render( x, y, currentClip );
}

void AvionView::mostrarEstacionar(int x, int y, int frame){
  SDL_Rect* currentClip;
  currentClip = &clipsEstacionado[ 0 ];

  vistaAvionTexture->render( x, y, currentClip );
}
