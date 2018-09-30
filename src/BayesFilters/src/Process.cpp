#include <BayesFilters/Process.h>

#include <iostream>

using namespace bfl;


bool Process::setProperty(const std::string& property)
{
    static_cast<void>(property);
    return false;
}
