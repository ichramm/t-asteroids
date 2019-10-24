#ifndef ASTEROIDE_H_
#define ASTEROIDE_H_

#include "ServidorRed.h"
#include "Targeteable.h"
#include "Colisionable.h"
class Objeto3d;
class PaqueteRed;

class Asteroide:public Colisionable, public ServidorRed, public Targeteable {
private:
	int id;
	float nivel;
	float vida;
	Vector velocidad;
	Vector velocidadAngular;
	float masa;
    bool ya_choco;
	float dt;
	float maxVel; // lo seteo en el trasladar

public:
	Asteroide(int id, float nivel,Objeto3d*modelo,Vector posicion);
	virtual ~Asteroide();
	void setPosicionTarget(Vector v){Colisionable::posicionar(v);}
	Vector getPosicionTarget(){return Colisionable::getPosicion();}


	float getNivel(){return nivel;}
	void setModeloAsteroide(Objeto3d*);
	void setVelocidad(Vector vel);
	void setVelocidadAngular(Vector vel);
	Vector getVelocidad();
	Vector getVelocidadAngular();
	float getMasa();

	//colisionable
	void tic(float dt);
	void bang(Colisionable*);
	float getDanio();

	void setVida(float v);
	float getVida();

    void setVidaTarget(float v){setVida(v);}
	float getVidaTarget(){return getVida();}


	void recibirPaqueteServidor(PaqueteRed *paquete);
	string getNombreServidor();

};

#endif /*ASTEROIDE_H_*/
