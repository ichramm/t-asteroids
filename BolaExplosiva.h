#ifndef BOLAEXPLOSIVA_H_
#define BOLAEXPLOSIVA_H_

#include "Animable.h"
#include "Vector.h"

class Engine;
class Objeto3d;
class Luz;
class ModoJuego;
class Sprite;

#define CANT_DIRS 10

class BolaExplosiva : public Animable {
private:
	Objeto3d*modelo;
	Sprite *explosion;
	Luz*luz;
	Vector dirs[CANT_DIRS];
	Vector pos;
	float retry;

public:
	BolaExplosiva(Vector posicion);
	virtual ~BolaExplosiva();
	void animar(float);
};

#endif /*BOLAEXPLOSIVA_H_*/
