#ifndef FMUNICION_H_
#define FMUNICION_H_

class Nave;
class Vector;

class FMunicion {
protected:
    Nave* origen;
    float cantMunicion;

public:
	FMunicion(Nave* origen);
	virtual ~FMunicion();

    virtual float getcantMunicion();
    virtual void setcantMunicion(float);
	virtual void disparar(Vector posicion,Vector direccion)=0;
	virtual void tic(float deltate)=0;
	virtual int getLado()=0;
	virtual void setLado(int)=0;
	virtual void setConTarget(bool)=0;
	virtual bool isConTarget()=0;
	void setOrigen(Nave* val){this->origen = val;}
	Nave* getOrigen(){return this->origen;}
};
#endif /*FMUNICION_H_*/
