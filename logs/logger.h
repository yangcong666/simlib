#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <sstream>
#include <mutex>

struct tm;

namespace logger
{
    enum struct level{Debug,Info,Warning,Error,Fatal};
    class FileLogger;
    class ConsoleLogger;
    class BaseLogger;

    class BaseLogger
    {
        class LogStream;

    public:

        BaseLogger() = default;
        virtual ~BaseLogger() = default;

        virtual LogStream operator()(level nLevel = level::Debug);

    private:
        const tm* getLocalTime();
        void endline(level nLevel, std::string&& oMessage);
        virtual void output(const tm *p_tm,
                            const char *str_level,
                            const char *str_message) = 0;
    private:

        std::mutex _lock;
        tm _localTime;
    };

    class BaseLogger::LogStream:public std::ostringstream
    {
        BaseLogger & m_oLogger;
        level        m_nLevel;
    public:

        LogStream(BaseLogger &oLogger,level nLevel)
            :m_oLogger(oLogger),m_nLevel(nLevel){};
        LogStream(const LogStream&ls)
            :m_oLogger(ls.m_oLogger),m_nLevel(ls.m_nLevel){};
        ~LogStream()
        {
            m_oLogger.endline(m_nLevel, std::move(str()));
        }
    };

    class ConsoleLogger : public BaseLogger
{
    using BaseLogger::BaseLogger;
    virtual void output(const tm *p_tm,
                        const char *str_level,
                        const char *str_message);
};

class FileLogger : public BaseLogger
{
public:
    FileLogger(std::string filename) noexcept;
    FileLogger(const FileLogger&) = delete;
    FileLogger(FileLogger&&) = delete;
    virtual ~FileLogger();
private:
    virtual void output(const tm *p_tm,
                        const char *str_level,
                        const char *str_message);
private:
    std::ofstream _file;
};

extern ConsoleLogger debug;
extern FileLogger record;




}







#endif