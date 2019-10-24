#ifndef MENURED_H
#define MENURED_H

#include "Menu.h"
#include "ObserversConexion.h"
class Evento;
class TextBox;
class Texto2D;
class Luz;

enum ModoMenuRed
{
    ModoServidor,
    ModoCliente
};

class MenuRed
    : public Menu, ObserverConexion
{
    public:
        static MenuRed * getInstance();
        virtual ~MenuRed();
        void setModo(ModoMenuRed);

        virtual void onError(std::string message);
        virtual void onOK(std::string message);
    protected:
        void iniciarItems();
        Menu* onTicCheckKey();
        Menu* onEnterPressed();
        void handleIndexChanged(int last);
        void OnEnter();
    private:
        MenuRed();
        static MenuRed * s_instance;

        ModoMenuRed m_modoRed;

        TextBox
            * m_txtServer,
            * m_txtPort;

        Luz * m_otraLuz;

        Evento * m_event;
        std::string m_eventData;
        Texto2D * m_eventText;
        bool m_onError;

        bool waitForConnection();
};

#endif // MENURED_H
