/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#include <BayesFilters/AdditiveStateModel.h>

using namespace bfl;
using namespace Eigen;


void AdditiveStateModel::motion(const Ref<const MatrixXd>& cur_states, Ref<MatrixXd> mot_states)
{
    propagate(cur_states, mot_states);

    mot_states += getNoiseSample(mot_states.cols());
}
