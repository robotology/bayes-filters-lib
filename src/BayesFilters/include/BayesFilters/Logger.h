/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace bfl {
    class Logger;
}


class bfl::Logger
{
public:
    virtual ~Logger() noexcept;

    bool enable_log(const std::string& folder_path, const std::string& file_name_prefix);

    bool disable_log();

    std::string get_folder_path() const;

    std::string get_file_name_prefix() const;

    template<typename DatumType>
    void logger(DatumType datum)
    {
        if (log_enabled_)
            log_files_[0] << datum << std::endl;
    }

    template<typename... DataType>
    void logger(DataType... data)
    {
        if (log_enabled_)
            logger_helper(0, data...);
    }

    template<typename DatumType>
    void logger(DatumType datum) const
    {
        if (log_enabled_)
            log_files_[0] << datum << std::endl;
    }

    template<typename... DataType>
    void logger(DataType... data) const
    {
        if (log_enabled_)
            logger_helper(0, data...);
    }


protected:
    virtual std::vector<std::string> log_file_names(const std::string& folder_path, const std::string& file_name_prefix);

    virtual void log();


private:
    std::string folder_path_;

    std::string file_name_prefix_;

    std::vector<std::string> file_names_;

    bool log_enabled_ = false;

    mutable std::vector<std::ofstream> log_files_;

    template<typename DatumType>
    void logger_helper(const size_t pos, DatumType datum)
    {
        log_files_[pos] << datum << std::endl;
    }

    template<typename DatumType, typename... DataType>
    void logger_helper(const size_t pos, DatumType datum, DataType... data)
    {
        log_files_[pos] << datum << std::endl;

        logger_helper(pos + 1, data...);
    }

    template<typename DatumType>
    void logger_helper(const size_t pos, DatumType datum) const
    {
        log_files_[pos] << datum << std::endl;
    }

    template<typename DatumType, typename... DataType>
    void logger_helper(const size_t pos, DatumType datum, DataType... data) const
    {
        log_files_[pos] << datum << std::endl;

        logger_helper(pos + 1, data...);
    }
};

#endif /* LOGGER_H */
