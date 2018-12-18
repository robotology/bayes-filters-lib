#include <BayesFilters/Agent.h>

#include <iostream>

using namespace bfl;


Agent::~Agent() noexcept
{ }


bool Agent::setProperty(const std::string& property)
{
    static_cast<void>(property);
    return false;
}
