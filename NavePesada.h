#ifndef NAVEPESADA_H
#define NAVEPESADA_H

#include "Nave.h"
#include "ServidorRed.h"


class NavePesada : public Nave, public ServidorRed
{
    public:
        NavePesada(Objeto3d*obj,Vector posicion);
        virtual ~NavePesada();
        void explotar();

        void recibirPaqueteServidor(PaqueteRed* paquete);
        string getNombreServidor();

        void disparar();

    private:
        Objeto3d*fuegoP;
        Sprite *brilloFuegoP1;
        Sprite *brilloFuegoP2;
        Sprite *brilloFuegoN1;
        Sprite *brilloFuegoN2;
        void ticVivo(float dt);
};

#endif // NAVEPESADA_H
