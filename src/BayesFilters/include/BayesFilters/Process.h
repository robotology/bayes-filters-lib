#ifndef PROCESS_H
#define PROCESS_H

#include <BayesFilters/Data.h>

#include <memory>
#include <string>

#include <Eigen/Dense>

namespace bfl {
    class Process;
}


class bfl::Process
{
public:
    virtual ~Process() noexcept { };

    virtual bool bufferProcessData() = 0;

    virtual Data getProcessData() const = 0;

    virtual bool setProperty(const std::string& property);
};

#endif /* PROCESS_H */
