#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <ctime>
class Appender {
   public:
    virtual Appender& operator<<(const std::string& msg) = 0;
};
std::string now( const char* format = "%c" )
{
    std::time_t t = std::time(0) ;
    char cstr[128] ;
    std::strftime( cstr, sizeof(cstr), format, std::localtime(&t) ) ;
    return cstr ;
}
class FileAppender : public Appender {
   public:
    FileAppender(const std::string& filename) {
        out.open(filename, std::ios::app);
    }

    virtual FileAppender& operator<<(const std::string& msg) {
        out << msg;
        out.flush();
        return *this;
    }

    ~FileAppender() {
        out.flush();
        out.close();
    }

   private:
    std::ofstream out;
};

class StderrAppender : public Appender {
   public:
    virtual StderrAppender& operator<<(const std::string& msg) {
        std::cerr  << msg;
        return *this;
    }
};

class Logger {
   public:
    enum LEVEL { DEBUG, INFO, WARNING, ERROR };
    Logger(const std::string& funcname = "", const std::list<std::shared_ptr<Appender>> appenders = {})
        : funcname(funcname), appenders(appenders) {}
    Logger(const Logger& logger, const std::string& funcname = "")
        : funcname(logger.funcname + funcname), appenders(logger.appenders) {}
    void info(const std::string& msg) {
        for (auto appender : appenders) {
            (*appender) << now("[%Y-%m-%d %T] ") << "[INFO] " << funcname << ": " << msg << "\n";
        }
    }

    void debug(const std::string& msg) {
        for (auto appender : appenders) {
            (*appender) << now("[%Y-%m-%d %T] ") << "[DEBUG] " << funcname << ": " << msg << "\n";
        }
    }

    void warning(const std::string& msg) {
        for (auto appender : appenders) {
            (*appender) << now("[%Y-%m-%d %T] ") << "[WARNING] " << funcname << ": " << msg << "\n";
        }
    }

    void error(const std::string& msg) {
        for (auto appender : appenders) {
            (*appender) << now("[%Y-%m-%d %T] ") << "[ERROR] " << funcname << ": " << msg << "\n";
        }
    }

   private:
    std::string funcname;
    std::list<std::shared_ptr<Appender>> appenders;
};

#endif