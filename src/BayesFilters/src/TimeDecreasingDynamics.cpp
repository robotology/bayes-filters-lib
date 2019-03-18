#include <BayesFilters/TimeDecreasingDynamics.h>
#include <BayesFilters/utils.h>

#include <iostream>
#include <limits>

using namespace bfl;
using namespace Eigen;


struct TimeDecreasingDynamics::ImplData
{
    enum class Modality
    {
        Iteration,
        Time
    };

    Modality modality_;

    unsigned int iterations_;

    unsigned int current_iterations_ = 0;

    double seconds_;

    double current_seconds_ = 0.0;

    utils::CpuTimer<> timer_;
};


TimeDecreasingDynamics::TimeDecreasingDynamics(std::unique_ptr<StateModel> state_model, const unsigned int iterations) noexcept :
    StateModelDecorator(std::move(state_model)),
    pImpl_(std::unique_ptr<ImplData>(new ImplData))
{
    ImplData& rImpl = *pImpl_;


    rImpl.modality_ = ImplData::Modality::Iteration;

    rImpl.iterations_ = iterations;

    rImpl.seconds_ = std::numeric_limits<double>::infinity();
}


TimeDecreasingDynamics::TimeDecreasingDynamics(std::unique_ptr<StateModel> state_model, const double seconds) noexcept :
    StateModelDecorator(std::move(state_model)),
    pImpl_(std::unique_ptr<ImplData>(new ImplData))
{
    ImplData& rImpl = *pImpl_;


    rImpl.modality_ = ImplData::Modality::Time;

    rImpl.iterations_ = std::numeric_limits<double>::infinity();

    rImpl.seconds_ = std::abs(seconds);

    if (seconds < 0)
    {
        std::cerr << "WARNING::TIMEDECREASINGDYNAMICS::CTOR\n";
        std::cerr << "WARNING::LOG:\n\tInput parameter `seconds` is negative. Used as positive.\n";
        std::cerr << "WARNING::LOG:\n\tProvided: " << seconds << ". Used " << rImpl.seconds_ << "." << std::endl;
    }
}


TimeDecreasingDynamics::TimeDecreasingDynamics(TimeDecreasingDynamics&& state_model) noexcept :
    StateModelDecorator(std::move(state_model)),
    pImpl_(std::move(state_model.pImpl_))
{ }


TimeDecreasingDynamics::~TimeDecreasingDynamics() noexcept
{ }


TimeDecreasingDynamics& TimeDecreasingDynamics::operator=(TimeDecreasingDynamics&& state_model) noexcept
{
    StateModelDecorator::operator=(std::move(state_model));

    pImpl_ = std::move(state_model.pImpl_);

    return *this;
}


MatrixXd TimeDecreasingDynamics::getNoiseSample(const std::size_t num)
{
    ImplData& rImpl = *pImpl_;


    double damper = 1.0;

    switch (rImpl.modality_)
    {
        case ImplData::Modality::Iteration:
        {
            damper = (rImpl.current_iterations_ <= rImpl.iterations_) ? std::exp(-rImpl.current_iterations_) : 0.0;

            break;
        }

        case ImplData::Modality::Time:
        {
            damper = (rImpl.current_seconds_ <= rImpl.seconds_) ? std::exp(-rImpl.current_seconds_) : 0.0;

            break;
        }

        default:
            return StateModelDecorator::getNoiseSample(num);
    }

    return StateModelDecorator::getNoiseSample(num) * damper;
}


bool TimeDecreasingDynamics::setProperty(const std::string& property)
{
    ImplData& rImpl = *pImpl_;


    if (property == "tdd_reset")
    {
        switch (rImpl.modality_)
        {
            case ImplData::Modality::Iteration:
            {
                rImpl.current_iterations_ = 0;

                break;
            }

            case ImplData::Modality::Time:
            {
                rImpl.timer_.stop();

                break;
            }

            default:
                return false;
        }

        return true;
    }


    if (property == "tdd_advance")
    {
        switch (rImpl.modality_)
        {
            case ImplData::Modality::Iteration:
            {
                ++rImpl.current_iterations_;

                break;
            }

            case ImplData::Modality::Time:
            {
                rImpl.current_seconds_ = rImpl.timer_.elapsed() * 1000.0;

                if (!rImpl.timer_.is_running())
                    rImpl.timer_.start();

                break;
            }

            default:
                return false;
        }

        return true;
    }


    return StateModelDecorator::setProperty(property);
}
