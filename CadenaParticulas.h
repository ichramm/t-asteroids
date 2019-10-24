#ifndef CADENAPARTICULAS_H
#define CADENAPARTICULAS_H

#include "Animable.h"
#include "vector.h"
class SistemaParticulas;

class CadenaParticulas : public Animable {
    public:
        CadenaParticulas(int cantParticulas);
        virtual ~CadenaParticulas();

        void setCantParticulas(int cant);
        int getCantParticulas();

        void setPosParticula(int i, const Vector &v);
        Vector getPosParticula(int i);

        void setElasticidad(float e);
        float getElasticidad();

        void setDistanciaEntreParticulas(float dist);
        float getDistanciaEntreParticulas();

        virtual void animar(float);

    private:
        SistemaParticulas *sp;
        float elasticidad;
        float distanciaEntreParticulas;
        Vector *velocidades;
        Vector *aceleraciones;
};

#endif // CADENAPARTICULAS_H
