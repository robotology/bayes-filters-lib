#ifndef EXOGENOUSMODEL_H
#define EXOGENOUSMODEL_H

#include <Eigen/Dense>

namespace bfl {
    class ExogenousModel;
}


class bfl::ExogenousModel
{
public:
    virtual ~ExogenousModel() noexcept { };

    virtual void propagate(const Eigen::Ref<const Eigen::MatrixXd>& cur_states, Eigen::Ref<Eigen::MatrixXd> prop_states) = 0;

    virtual Eigen::MatrixXd getExogenousMatrix() = 0;

    virtual bool setProperty(const std::string& property) = 0;
};

#endif /* EXOGENOUSMODEL_H */
