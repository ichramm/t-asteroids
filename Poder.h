#ifndef PODERVIDA_H_
#define PODERVIDA_H_

#include "ServidorRed.h"
#include "Colisionable.h"

class Poder;
class Nave;
class colisionable;
class OrbitaParticulas;
class Luz;
class Objeto3d;

enum pow_type{POW_VIDA,POW_MISIL,POW_BALAS,POW_ENV};

class Poder: public Colisionable, public ServidorRed {
private:
	float ttl;
	static const int MAX_PODER=4;
	int pow;
	Luz*luz;
	Objeto3d*secundario;
	OrbitaParticulas *op;

public:
	Poder(Objeto3d*modelo,Vector posicion);
	virtual ~Poder();
	void tic(float);
	void bang(Colisionable*who);
	void recibirPaqueteServidor(PaqueteRed* paquete);

    string getNombreServidor();
    int GetPoder(){return pow;}
    void SetPoder(int p){pow = p;}
};

#endif /*PODERVIDA_H_*/
