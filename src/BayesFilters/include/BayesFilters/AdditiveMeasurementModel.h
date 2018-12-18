#ifndef ADDITIVEMEASUREMENTMODEL_H
#define ADDITIVEMEASUREMENTMODEL_H

#include <BayesFilters/MeasurementModel.h>

namespace bfl {
    class AdditiveMeasurementModel;
}


class bfl::AdditiveMeasurementModel : public bfl::MeasurementModel
{
public:
    virtual ~AdditiveMeasurementModel() noexcept;
};

#endif /* ADDITIVEMEASUREMENTMODEL_H */
