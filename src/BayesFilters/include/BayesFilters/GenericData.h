#ifndef GENERICDATA_H
#define GENERICDATA_H

#include <string>

namespace bfl {
    class GenericData;
}


class bfl::GenericData
{
public:
    virtual ~GenericData() noexcept { };

    virtual bool setProperty(const std::string property) = 0;

    virtual void enableLog(const std::string& prefix_name);

    virtual void disableLog();
};

#endif /* PROCESS_H */
