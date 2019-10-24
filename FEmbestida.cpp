#include "FEmbestida.h"
#include "Vector.h"
#include "Nave.h"

FEmbestida::FEmbestida(Nave* origen):FMunicion(origen)
{
    this->cantMunicion = 10;
}

FEmbestida::~FEmbestida()
{
    //dtor
}

void FEmbestida::disparar(Vector posicion,Vector direccion){
        this->origen->setEmbistiendo();
}
