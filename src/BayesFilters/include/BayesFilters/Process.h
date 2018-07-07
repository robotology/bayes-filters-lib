#ifndef PROCESS_H
#define PROCESS_H

#include <BayesFilters/GenericData.h>

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

    virtual std::shared_ptr<GenericData> getProcessData() = 0;

    virtual bool setProperty(const std::string property) = 0;

    virtual void enableLog(const std::string& prefix_name);

    virtual void disableLog();
};

#endif /* PROCESS_H */
