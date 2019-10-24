#ifndef BALACOMUN_H_
#define BALACOMUN_H_

#include "Municion.h"
#include "ServidorRed.h"
#include "Colisionable.h"

class Luz;
class Nave;
class Asteroide;
class Objeto3d;
class PaqueteRed;


class BalaComun : public Colisionable, public ServidorRed, public Municion {
private:
	Luz* luz;
	Vector velocidad;
	float ttl;
	bool porMorir;
	float danio;

public:

	BalaComun(Objeto3d* modelo,Vector posicion,Vector direccion);
	virtual ~BalaComun();
	void setRoja();
	Vector getVelocidad();
	void setVelocidad(Vector velocidad);
	float getDanio();
	void setDanio(float);
	//colisionable
	void tic(float dt);
	void bang(Colisionable*);

	void recibirPaqueteServidor(PaqueteRed* paquete);

	string getNombreServidor();

	void setTtl(float t);
	//string getNombreServidor();
};

#endif /*BALACOMUN_H_*/
