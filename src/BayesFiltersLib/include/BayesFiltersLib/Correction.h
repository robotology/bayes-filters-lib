#ifndef CORRECTION_H
#define CORRECTION_H

#include <Eigen/Dense>


namespace bfl
{

class Correction
{
public:
    virtual ~Correction() noexcept { };
    
    virtual void correct(const Eigen::Ref<const Eigen::VectorXf>& pred_state, const Eigen::Ref<const Eigen::MatrixXf>& measurements, Eigen::Ref<Eigen::VectorXf> cor_state) = 0;
};

} // namespace bfl

#endif /* CORRECTION_H */
