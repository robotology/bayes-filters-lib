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
    HistoryBuffer() noexcept { };

    HistoryBuffer(HistoryBuffer&& history_buffer) noexcept;

    HistoryBuffer& operator=(HistoryBuffer&& history_buffer) noexcept;

    ~HistoryBuffer() noexcept { };

    void            addElement(const Eigen::Ref<const Eigen::VectorXf>& element);

    Eigen::MatrixXf getHistoryBuffer() const;

    bool            setHistorySize(const unsigned int window);

    unsigned int    getHistorySize() const { return window_; };

    bool            decreaseHistorySize();

    bool            increaseHistorySize();

    bool            clear();

private:
    unsigned int                window_     = 5;

    const unsigned int          max_window_ = 30;

    std::deque<Eigen::VectorXf> history_buffer_;
};

#endif /* HISTORYBUFFER_H */
