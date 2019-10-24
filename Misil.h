#ifndef MISIL_H_
#define MISIL_H_

#include "Colisionable.h"
#include "ServidorRed.h"
#include "Municion.h"

class PaqueteRed;
class Luz;
class Targeteable;
class Objeto3d;

class Misil : public Colisionable, public ServidorRed, public Municion {
private:

	static const float mult_aceleracion;
	Vector velocidad;
	Vector aceleracion;
	float danio;
	float ttl;
	float inicio;
	Luz*luz;
	bool arranco;
	Targeteable*target;
	Objeto3d*fuego;

	float retryHumo;


public:
	Misil(Objeto3d* modelo,Vector posicion,Vector direccion);
	virtual ~Misil();

	float getDanio();
	void setDanio(float danio);
	void setTarget(Targeteable*);
	Targeteable*getTarget();

	void tic(float dt);
	void bang(Colisionable*);

	void recibirPaqueteServidor(PaqueteRed *paquete);
	string getNombreServidor();

	void setTtl(float t);

};

#endif /*MISIL_H_*/
