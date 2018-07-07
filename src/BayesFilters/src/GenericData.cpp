#include <BayesFilters/GenericData.h>

#include <iostream>

using namespace bfl;


void GenericData::enableLog(const std::string& prefix_name)
{
    static_cast<void>(prefix_name);
    std::cerr << "WARNING::GENERICDATA::ENABLELOG\n";
    std::cerr << "\tWARNING: Log facility is not implemented for this GenericData class. Nothing to enable." << std::endl;
}


void GenericData::disableLog()
{
    std::cerr << "WARNING::GENERICDATA::DISABLELOG\n";
    std::cerr << "\tWARNING: Log facility is not implemented for this GenericData class. Nothing to disable." << std::endl;
}
