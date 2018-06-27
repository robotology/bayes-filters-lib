#include <BayesFilters/MeasurementModel.h>

#include <iostream>

using namespace bfl;
using namespace Eigen;


std::pair<bool, MatrixXf> MeasurementModel::getNoiseSample(const int num) const
{
    return std::make_pair(false, MatrixXf::Zero(1, 1));
}


std::pair<bool, MatrixXf> MeasurementModel::getNoiseCovarianceMatrix() const
{
    return std::make_pair(false, MatrixXf::Zero(1, 1));
}


bool MeasurementModel::setProperty(const std::string property)
{
    return false;
}


void MeasurementModel::setProcess(std::unique_ptr<Process> process)
{
    process_ = std::move(process);
}

bool MeasurementModel::bufferProcessState()
{
    if (process_)
        return process_->bufferProcessState();

    std::cerr << "ERROR::MEASUREMENTMODEL::BUFFERPROCESSMEASUREMENTS\n";
    std::cerr << "\tERROR: There is no process assigend to the MeasurementModel. Cannot buffer process state." << std::endl;

    return false;
}


std::pair<bool, MatrixXf> MeasurementModel::getProcessMeasurements() const
{
    /* TODO
       Multi-sensor data are WIP.
       Need to properly think how to model them. */

    if (process_)
    {
        bool valid_process_state = false;
        MatrixXf process_state;
        std::tie(valid_process_state, process_state) = process_->getProcessState();

        if (valid_process_state)
            return measure(process_state);
        else
        {
            std::cerr << "ERROR::MEASUREMENTMODEL::GETPROCESSMEASUREMENTS\n";
            std::cerr << "\tERROR: Invalid process state." << std::endl;

            return std::make_pair(false, MatrixXf::Zero(1, 1));
        }
    }

    std::cerr << "ERROR::MEASUREMENTMODEL::GETPROCESSMEASUREMENTS\n";
    std::cerr << "\tERROR: There is no process assigend to the MeasurementModel. Cannot measure process state." << std::endl;

    return std::make_pair(false, MatrixXf::Zero(1, 1));
}


void MeasurementModel::enableLog(const std::string& prefix_name)
{
    static_cast<void>(prefix_name);
    std::cerr << "WARNING::MEASUREMENTMODEL::ENABLELOG\n";
    std::cerr << "\tWARNING: Log facility is not implemented for this MeasurementModel class. Nothing to enable." << std::endl;
}


void MeasurementModel::disableLog()
{
    std::cerr << "WARNING::MEASUREMENTMODEL::DISABLELOG\n";
    std::cerr << "\tWARNING: Log facility is not implemented for this MeasurementModel class. Nothing to disable." << std::endl;
}