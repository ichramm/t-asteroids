#ifndef CTRLJUEGO_H_
#define CTRLJUEGO_H_

class Modo;

//namespace
using namespace std;
//class definition
class CtrlJuego{
private:
	static CtrlJuego*instance;
	CtrlJuego();
	Modo*modo;
public:
	static CtrlJuego*getInstance();
	virtual ~CtrlJuego();
	//operaciones
	void inicializar(int argc, char *argv[]);
	static void tic(void);
	//getters & setters
	Modo*getModo();
	void setModo(Modo*);

};



#endif /*CTRLJUEGO_H_*/
