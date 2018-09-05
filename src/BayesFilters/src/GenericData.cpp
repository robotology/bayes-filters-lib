#include <BayesFilters/GenericData.h>

#include <iostream>

using namespace bfl;


bool GenericData::setProperty(const std::string& property)
{
    static_cast<void>(property);
    return false;
}
