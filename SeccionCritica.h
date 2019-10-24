#ifndef SECCIONCRITICA_H
#define SECCIONCRITICA_H
#include <windows.h>

// Clase para el manejo del problema de la seccion critica

class SeccionCritica{
    private:
        CRITICAL_SECTION seccionCritica;

    public:
        SeccionCritica();
        virtual ~SeccionCritica();

        void entrar();
        bool intentarEntrar();

        void salir();

};


class ScopedLock
{
private:
    SeccionCritica* m_lock;
public:
    ScopedLock(SeccionCritica * lock) {
        m_lock = lock;
        m_lock->entrar();
    }
    ~ScopedLock() {
        m_lock->salir();
    }
};

#endif // SECCIONCRITICA_H
