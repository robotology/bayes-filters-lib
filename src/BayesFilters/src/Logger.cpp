#include <BayesFilters/Logger.h>

#include <iostream>

using namespace bfl;


Logger::~Logger() noexcept
{ }


void Logger::enable_log(const std::string& prefix_name)
{
    static_cast<void>(prefix_name);
    std::cerr << "WARNING::LOGGER::ENABLELOG\n";
    std::cerr << "\tWARNING: Log facility is not implemented for this class. Nothing to enable." << std::endl;
}


void Logger::disable_log()
{
    std::cerr << "WARNING::GENERICDATA::DISABLELOG\n";
    std::cerr << "\tWARNING: Log facility is not implemented for this class. Nothing to disable." << std::endl;
}


void Logger::log()
{ }
