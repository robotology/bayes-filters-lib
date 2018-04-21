#include <BayesFilters/InitSurveillanceAreaGrid.h>

using namespace bfl;
using namespace Eigen;


InitSurveillanceAreaGrid::InitSurveillanceAreaGrid(const double surv_x_inf, const double surv_x_sup, const double surv_y_inf, const double surv_y_sup,
                                                   const unsigned int num_particle_x, const unsigned int num_particle_y) noexcept :
    surv_x_inf_(surv_x_inf),
    surv_x_sup_(surv_x_sup),
    surv_y_inf_(surv_y_inf),
    surv_y_sup_(surv_y_sup),
    num_particle_x_(num_particle_x),
    num_particle_y_(num_particle_y)
{ }


InitSurveillanceAreaGrid::InitSurveillanceAreaGrid(const double surv_x, const double surv_y,
                                                   const unsigned int num_particle_x, const unsigned int num_particle_y) noexcept :
    InitSurveillanceAreaGrid(0, surv_x, 0, surv_y,
                             num_particle_x, num_particle_y)
{ }


bool InitSurveillanceAreaGrid::initialize(Ref<MatrixXf> states, Ref<VectorXf> weights)
{
    int num_particle = states.cols();
    if (num_particle != num_particle_x_ * num_particle_y_)
        return false;

    double delta_surv_x = surv_x_sup_ - surv_x_inf_;
    double delta_surv_y = surv_y_sup_ - surv_y_inf_;

    for (int i = 0; i < num_particle_x_; ++i)
        for (int j = 0; j < num_particle_y_; ++j)
            states.col(i*num_particle_y_ + j) << static_cast<float>((delta_surv_x / (num_particle_x_ - 1)) * i + surv_x_inf_),
                                                                                                                            0,
                                                 static_cast<float>((delta_surv_y / (num_particle_y_ - 1)) * j + surv_y_inf_),
                                                                                                                            0;

    weights.setConstant(1.0f/num_particle);

    return true;
}
