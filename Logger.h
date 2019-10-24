#ifndef LOGGER_H
#define LOGGER_H

#define DO_DEBUG

#ifndef DO_DEBUG

# define LOG_INIT (minLevel, logName, overwrite)
# define LOG(level,str)
# define LOG_MORE(level, format, ...)

#else

#ifdef _MSC_VER
# define FUNCTION_NAME __FUNCDNAME__
#else
# define FUNCTION_NAME __PRETTY_FUNCTION__
#endif

// Conveniente para iniciar el Log
# define LOG_INIT(minLevel, logName, overwrite) Logger::getInstance()->Init(minLevel, logName, overwrite, FUNCTION_NAME , __LINE__);

// Usar estas dos
// An no se que quieras meter el nombre de la funcion a mano :)
// Loguea el string pasado como parametro
// Ej: "LOG(LogLevel::Info, "Llamando al tic de pepe")
# define LOG(level,str) Logger::getInstance()->Log(level, FUNCTION_NAME, __LINE__, str);

// Comportamiento parecido al prinf
// Ej: LOG_MORE(LogLevel::Debug, "A pepe le gusta %s %d veces por dia", "orinar", 3)
# define LOG_MORE(level, format, ...) Logger::getInstance()->Log(level, FUNCTION_NAME, __LINE__, format, __VA_ARGS__);

// Largo maximo en cantidad de caracteres para loguear (con Variadic Function)
// No se si deba cambiarlo
# define LOG_MAX_BUFFER_SIZE 1024

#endif



#include "SeccionCritica.h"

#include <iostream>
#include <fstream>
using namespace std;

// No tenia ganas de meterlo como enumerado
// Ademas los enumerados me cagan con el cout (me meten un int creo)
class LogLevel
{
private:
    LogLevel(string level, short prio) : m_value(level), m_priority(prio){}
    string m_value;
    short  m_priority;
public:
	friend ostream& operator<<(ostream &, const LogLevel &);
	friend bool operator>=(const LogLevel & me, const LogLevel & other);
	friend class Logger;

    static LogLevel Debug;
    static LogLevel Info;
    static LogLevel Warning;
    static LogLevel Error;
    static LogLevel Fatal;
};
bool operator>=(const LogLevel & me, const LogLevel & other);


class Logger // Singleton
{
public:
    static Logger* getInstance();

    void Init(const LogLevel & minLevel, const string & logName, bool overWrite, const string & funcName = " ", int line = 0 );
    void Log(const LogLevel & level, const string & functionName, int line, const string & logInfo);
    void Log(const LogLevel & level, const string & functionName, int line, char* format, ...);

private:

    static Logger* s_instance;
    Logger();
    ~Logger();

    // El archivo de log
    ofstream        m_logStream;

	SeccionCritica  m_lock;

    LogLevel        m_minLevel;

    void logDateLevelFunction(const LogLevel & level, const string & functionName, int line);
};

#endif // LOGGER_H
