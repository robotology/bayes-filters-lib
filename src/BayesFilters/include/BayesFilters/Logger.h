#ifndef LOGGER_H
#define LOGGER_H

#include <string>

namespace bfl {
    class Logger;
}


class bfl::Logger
{
public:
    virtual ~Logger() noexcept;

    virtual void enable_log(const std::string& prefix_name);

    virtual void disable_log();

    virtual void log();
};

#endif /* LOGGER_H */
