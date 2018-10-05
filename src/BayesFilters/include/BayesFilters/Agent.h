#ifndef AGENT_H
#define AGENT_H

#include <BayesFilters/Data.h>

#include <memory>
#include <string>

#include <Eigen/Dense>

namespace bfl {
    class Agent;
}


class bfl::Agent
{
public:
    virtual ~Agent() noexcept;

    virtual bool bufferData() = 0;

    virtual Data getData() const = 0;

    virtual bool setProperty(const std::string& property);
};

#endif /* AGENT_H */
