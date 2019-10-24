#ifndef ELECTRICIDAD_H
#define ELECTRICIDAD_H

#include "Vector.h"
#include "Animable.h"
class Multiobjeto3d;

class Electricidad : public Animable
{

    public:
        //Constructor
        Electricidad();
        Electricidad(float timeout);
        virtual ~Electricidad();
        //Funcion
        void animar(float);
        //Propiedades
        void setPosIni(Vector pos);
        void setPosFin(Vector pos);
        Vector getPosIni();
        Vector getPosFin();

        void setCantPartes(int cant);
        int getCantPartes();

        void setCurvatura(float c);
        float getCurvatura();

        void setFrecuencia(float frec);
        float getFrecuencia();

        Multiobjeto3d *getMultiobjeto();


    private:
        //Variables
        Vector pos_inicial;
        Vector pos_final;
        int cantPartes;
        float curvatura;
        float periodo;
        float tiempo;
        Multiobjeto3d* rayo;

};

#endif // ELECTRICIDAD_H