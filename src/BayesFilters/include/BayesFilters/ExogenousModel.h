/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#ifndef EXOGENOUSMODEL_H
#define EXOGENOUSMODEL_H

#include <BayesFilters/ExogenousProcess.h>
#include <BayesFilters/Skippable.h>

namespace bfl {
    class ExogenousModel;
}


class bfl::ExogenousModel : public ExogenousProcess, public Skippable
{
public:
    virtual ~ExogenousModel() noexcept = default;

    bool skip(const std::string& what_step, const bool status) override;

    bool getSkipState() override;


protected:
    ExogenousModel() noexcept = default;

    ExogenousModel(const ExogenousModel& exogenous_model) noexcept = delete;

    ExogenousModel& operator=(const ExogenousModel& exogenous_model) noexcept = delete;

    ExogenousModel(ExogenousModel&& exogenous_model) noexcept = default;

    ExogenousModel& operator=(ExogenousModel&& exogenous_model) noexcept = default;


private:
    /**
     * Skip status.
     */
    bool skip_ = false;
};

#endif /* EXOGENOUSMODEL_H */
