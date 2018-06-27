#include <BayesFilters/Process.h>

#include <iostream>

using namespace bfl;


void Process::enableLog(const std::string& prefix_name)
{
    static_cast<void>(prefix_name);
    std::cerr << "WARNING::PROCESS::ENABLELOG\n";
    std::cerr << "\tWARNING: Log facility is not implemented for this Process class. Nothing to enable." << std::endl;
}


void Process::disableLog()
{
    std::cerr << "WARNING::PROCESS::DISABLELOG\n";
    std::cerr << "\tWARNING: Log facility is not implemented for this Process class. Nothing to disable." << std::endl;
}