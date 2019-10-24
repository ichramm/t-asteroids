#ifndef EXPLOSIONPARTICULAS_H
#define EXPLOSIONPARTICULAS_H

#include "Animable.h"
class SistemaParticulas;
class Vector;


class ExplosionParticulas : public Animable
{
    private:
        SistemaParticulas *sp;
        Vector *direccion;
        int cantidad;

    public:
        ExplosionParticulas(Vector pos,float duracion,Vector color,int cantidad);
        virtual ~ExplosionParticulas();

        void animar(float dt);

};

#endif // EXPLOSIONPARTICULAS_H
