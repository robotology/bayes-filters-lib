/*
 * Copyright (C) 2016-2019 Istituto Italiano di Tecnologia (IIT)
 *
 * This software may be modified and distributed under the terms of the
 * BSD 3-Clause license. See the accompanying LICENSE file for details.
 */

#ifndef HISTORYBUFFER_H
#define HISTORYBUFFER_H

#include <deque>

#include <Eigen/Core>

namespace bfl {
    class HistoryBuffer;
}


class bfl::HistoryBuffer
{
public:
    HistoryBuffer(const std::size_t state_size) noexcept;

    HistoryBuffer(HistoryBuffer&& history_buffer) noexcept;

    HistoryBuffer& operator=(HistoryBuffer&& history_buffer) noexcept;

    ~HistoryBuffer() noexcept = default;

    void addElement(const Eigen::Ref<const Eigen::VectorXd>& element);

    Eigen::MatrixXd getHistoryBuffer() const;

    bool setHistorySize(const unsigned int window);

    inline unsigned int getHistorySize() const { return window_; };

    bool decreaseHistorySize();

    bool increaseHistorySize();

    bool clear();

private:
    unsigned int window_ = 5;

    const unsigned int max_window_ = 30;

    std::deque<Eigen::VectorXd> history_buffer_;

    std::size_t state_size_;
};

#endif /* HISTORYBUFFER_H */
