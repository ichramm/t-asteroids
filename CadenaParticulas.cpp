#include "CadenaParticulas.h"
#include "ModoJuego.h"
#include "SistemaParticulas.h"
#include "Escena.h"

CadenaParticulas::CadenaParticulas(int cantParticulas): Animable() {
    sp=new SistemaParticulas(cantParticulas,"flare5.tga");

    elasticidad=1;
    ModoJuego::getInstance()->getEscena()->addDibujable(sp);
    velocidades=new Vector[cantParticulas];
    aceleraciones = new Vector[cantParticulas];
    distanciaEntreParticulas=0;
}

CadenaParticulas::~CadenaParticulas(){
    delete sp;
}

void CadenaParticulas::setCantParticulas(int cant){
    delete sp;
    delete velocidades;
    sp=new SistemaParticulas(cant);
    ModoJuego::getInstance()->getEscena()->addDibujable(sp);
    velocidades=new Vector[cant];
}

int CadenaParticulas::getCantParticulas(){
    return sp->getCantParticulas();
}

void CadenaParticulas::setPosParticula(int i,const Vector &pos) {
    sp->setParticula(i,pos);
}

Vector CadenaParticulas::getPosParticula(int i){
    return sp->getParticula(i);
}

void CadenaParticulas::setElasticidad(float k) {
    elasticidad=k;
}

float CadenaParticulas::getElasticidad(){
    return elasticidad;
}

void CadenaParticulas::setDistanciaEntreParticulas(float dist) {
    distanciaEntreParticulas=dist;
}

float CadenaParticulas::getDistanciaEntreParticulas(){
    return distanciaEntreParticulas;
}

void CadenaParticulas::animar(float dt){
    int cant=sp->getCantParticulas();
    float k=dt*elasticidad;
    float dist2=distanciaEntreParticulas*distanciaEntreParticulas;

    for(int i=1; i<cant-1; i++){ //salteo la primera particula y la ultima
        Vector dif=sp->getParticula(i-1) + sp->getParticula(i+1) - 2*sp->getParticula(i);
        aceleraciones[i] = dif;
        if(dif.norma2()>dist2){
            velocidades[i]+=k*dif;
        }else{
            velocidades[i]*=0;
        }
        velocidades[i]*=0.999; //friccion
        velocidades[i]+=Vector(0,-1,0)*dt*3; //gravedad
    }
    //ahora que tengo la velocidad de todas las particulas actualizo su posicion
    for(int i=1; i<cant; i++){ //salteo la primera particula
        Vector pos=sp->getParticula(i) + velocidades[i]*dt + aceleraciones[i]*dt*dt*0.5; //2 terminos del desarrollo de tailor
        sp->setParticula(i, pos);
    }
}
