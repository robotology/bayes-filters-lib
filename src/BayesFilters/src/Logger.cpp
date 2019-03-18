#include <BayesFilters/Logger.h>

#include <iostream>

using namespace bfl;


Logger::~Logger() noexcept
{
    if (log_enabled_)
        disable_log();
}


bool Logger::enable_log(const std::string& prefix_path, const std::string& prefix_name)
{
    if (!log_enabled_)
    {
        const std::vector<std::string>& file_names = log_file_names(prefix_path, prefix_name);

        if (file_names.size() == 0)
        {
            std::cerr << "WARNING::LOGGER::ENABLELOG\n";
            std::cerr << "\tWARNING: Log facility could not be started due to missing file names." << std::endl;

            return false;
        }

        auto file_name = file_names.begin();
        for (size_t i = 0; i < file_names.size(); ++i, ++file_name)
        {
            log_files_.emplace_back(std::ofstream("./" + *file_name + ".txt", std::ofstream::out | std::ofstream::app));

            if (!log_files_[i].is_open())
            {
                std::cerr << "WARNING::LOGGER::ENABLELOG\n";
                std::cerr << "\tWARNING: Log facility could not be started for file " + *file_name + ".txt." << std::endl;

                log_files_.clear();

                file_names_.clear();

                return false;
            }

            file_names_.emplace_back(*file_name);
        }

        log_enabled_ = true;

        return true;
    }
    else
        return false;
}


bool Logger::disable_log()
{
    if (log_enabled_)
    {
        log_files_.clear();

        file_names_.clear();

        log_enabled_ = false;

        return true;
    }
    else
        return false;
}


std::string Logger::get_prefix_path() const
{
    return prefix_path_;
}


std::string Logger::get_prefix_name() const
{
    return prefix_name_;
}


std::vector<std::string> Logger::log_file_names(const std::string& prefix_path, const std::string& prefix_name)
{
    static_cast<void>(prefix_path);
    static_cast<void>(prefix_name);

    std::cerr << "WARNING::LOGGER::LOG_FILENAMES\n";
    std::cerr << "\tWARNING: Log file names where not provided. Did you override `log_file_names()`?" << std::endl;

    return {};
}


void Logger::log()
{ }
