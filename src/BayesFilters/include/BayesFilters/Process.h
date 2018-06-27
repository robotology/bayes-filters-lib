#ifndef PROCESS_H
#define PROCESS_H

#include<string>

#include <Eigen/Dense>

namespace bfl {
    class Process;
}


class bfl::Process
{
public:
    virtual ~Process() noexcept { };

    virtual bool bufferProcessState() = 0;

    virtual std::pair<bool, Eigen::MatrixXf> getProcessState() const = 0;

    virtual bool setProperty(const std::string property) = 0;

    virtual void enableLog(const std::string& prefix_name);

    virtual void disableLog();
};

#endif /* PROCESS_H */
