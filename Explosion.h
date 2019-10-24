#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "Animable.h"
class Sprite;
class Vector;
class Explosion : public Animable
{
    private:
        Sprite* explosion;

    public:
        Explosion(Vector pos,float tiempo);
        virtual ~Explosion();

        void animar(float dt);

};
#endif // EXPLOSION_H