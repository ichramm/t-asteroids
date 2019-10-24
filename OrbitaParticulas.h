#ifndef ORBITAPARTICULAS_H
#define ORBITAPARTICULAS_H


#include "Animable.h"
class SistemaParticulas;
class Vector;

class OrbitaParticulas : public Animable
{
    private:
        SistemaParticulas *sp;
        Vector *velocidad;
        int cantidad;
        float masa;

    public:
        OrbitaParticulas(Vector pos,float duracion,Vector color,int cantidad,float dist,float dispersion=0,float masa=1);
        virtual ~OrbitaParticulas();

        void animar(float dt);

        void setMasa(float masa);
        float getMasa();
};

#endif // ORBITAPARTICULAS_H
