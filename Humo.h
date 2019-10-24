#ifndef HUMO_H
#define HUMO_H

#include "Animable.h"
class Sprite;
class Vector;

class Humo : public Animable
{
    private:
        Sprite* humo;
        float random;
        float factorOpacidad;

    public:
        Humo(Vector pos,float tiempo,float factorOpacidad=1);
        virtual ~Humo();

        void animar(float dt);

};

#endif // HUMO_H
