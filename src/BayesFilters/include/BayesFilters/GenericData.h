#ifndef GENERICDATA_H
#define GENERICDATA_H

#include <BayesFilters/Logger.h>

#include <string>

namespace bfl {
    class GenericData;
}


class bfl::GenericData : public bfl::Logger
{
public:
    virtual ~GenericData() noexcept { };

    virtual bool setProperty(const std::string& property);
};

#endif /* PROCESS_H */
