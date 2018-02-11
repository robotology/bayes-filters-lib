#ifndef DIRECTIONALSTATISTICSUTILS_H
#define DIRECTIONALSTATISTICSUTILS_H

#include <Eigen/Dense>


namespace bfl
{
    Eigen::MatrixXd directional_add(const Eigen::Ref<const Eigen::MatrixXd>& a, const Eigen::Ref<const Eigen::VectorXd>& b);
    
    Eigen::MatrixXd directional_sub(const Eigen::Ref<const Eigen::MatrixXd>& a, const Eigen::Ref<const Eigen::VectorXd>& b);

    double directional_mean(const Eigen::Ref<const Eigen::VectorXd>& a, const Eigen::Ref<const Eigen::VectorXd>& w);
}

#endif /* DIRECTIONALSTATISTICSUTILS_H */
