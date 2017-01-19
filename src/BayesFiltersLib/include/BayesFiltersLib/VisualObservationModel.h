#ifndef VISUALOBSERVATIONMODEL_H
#define VISUALOBSERVATIONMODEL_H

#include <Eigen/Dense>
#include <opencv2/core/core.hpp>


namespace bfl
{

    class VisualObservationModel
    {
    public:

        VisualObservationModel() = default;

        virtual ~VisualObservationModel() noexcept { };

        virtual void observe(const Eigen::Ref<const Eigen::VectorXf>& cur_state, cv::OutputArray observation) = 0;
    };
    
} // namespace bfl

#endif /* VISUALOBSERVATIONMODEL_H */
