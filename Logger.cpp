#include "Logger.h"

#include <sstream>
#include <string>

LogLevel LogLevel::Debug   ("[DEBUG]", 0);
LogLevel LogLevel::Info    ("[INFO]", 1);
LogLevel LogLevel::Warning ("[WARNING]", 2);
LogLevel LogLevel::Error   ("[ERROR]", 3);
LogLevel LogLevel::Fatal   ("[FATAL]", 4);

bool operator>=(const LogLevel & me, const LogLevel & log2) {
	return me.m_priority >= log2.m_priority;
}

ostream& operator << (ostream & co, const LogLevel & logLevel) {
    co << logLevel.m_value;
    return co;
}


Logger* Logger::s_instance = NULL;

Logger* Logger::getInstance() {
    if (!s_instance)
        s_instance = new Logger();
    return s_instance;
}

Logger::Logger() : m_minLevel(LogLevel::Debug) { }

Logger::~Logger() { }


void Logger::Init(const LogLevel & minLevel, const string & logName, bool overWrite, const string & funcName, int line) {
#ifdef DO_DEBUG
    ScopedLock lock(&m_lock);

    m_minLevel = minLevel;
    string fileName(logName);
    if (fileName.find_last_of(".") == string::npos){
        fileName += ".txt";
    }
	ios_base::openmode openMode = (overWrite)? ios_base::out : ios_base::app;

    m_logStream.open(fileName.data(), openMode );
    Log(LogLevel::Info, funcName, line, "Log started, min log level is %s.", minLevel.m_value.c_str());
#endif
}

void Logger::Log(const LogLevel & level, const string & functionName, int line, const string & logInfo) {
#ifdef DO_DEBUG
	Log(level, functionName, line, logInfo.c_str());
#endif
}


void Logger::Log(const LogLevel & level, const string & functionName, int line, char* format, ...) {
#ifdef DO_DEBUG
	ScopedLock lock(&m_lock);
	if (level >= m_minLevel) {
		SYSTEMTIME st;
		GetLocalTime(&st);

		char dateBuffer[24];  // 1 para el NULL
		sprintf(dateBuffer, "%d-%02d-%02d %02d:%02d:%02d.%03d",
			st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

		char buffer[LOG_MAX_BUFFER_SIZE];
		va_list args;
		va_start(args, format);
		int len;
		if (format){
			len = _vsnprintf(buffer, LOG_MAX_BUFFER_SIZE - 1, format, args);
			if(len == -1) {
				len = LOG_MAX_BUFFER_SIZE - 1;
				buffer[LOG_MAX_BUFFER_SIZE - 1] = 0;
			}
		}
		va_end(args);

		m_logStream<<dateBuffer<<";"<<level<<";"<<functionName<<";"<<"Line."<<line<<";"<<buffer<<endl;
		m_logStream.flush();
	}
#endif
}

