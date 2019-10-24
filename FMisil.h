#ifndef FMISIL_H_
#define FMISIL_H_
#include <string>
#include <iostream>
using namespace std;
#include "FMunicion.h"
class Nave;

class FMisil: public FMunicion{
private:
	string nameModelo;
	string textura;
	float retry[2];
	float retry_global;
	int lado;
	bool con_target;

public:
	FMisil(Nave* origen);
	virtual ~FMisil();

	int getLado();
	void setLado(int);
	bool isConTarget();
	void setConTarget(bool in);


	void disparar(Vector posicion,Vector direccion);
	void tic(float deltate);
};

#endif /*FMISIL_H_*/

