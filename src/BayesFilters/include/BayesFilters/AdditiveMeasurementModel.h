/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#ifndef ADDITIVEMEASUREMENTMODEL_H
#define ADDITIVEMEASUREMENTMODEL_H

#include <BayesFilters/MeasurementModel.h>

namespace bfl {
    class AdditiveMeasurementModel;
}


class bfl::AdditiveMeasurementModel : public bfl::MeasurementModel
{
public:
    virtual ~AdditiveMeasurementModel() noexcept = default;
};

#endif /* ADDITIVEMEASUREMENTMODEL_H */
