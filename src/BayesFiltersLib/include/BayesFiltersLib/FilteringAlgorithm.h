#ifndef FILTERINGALGORITHM_H
#define FILTERINGALGORITHM_H

namespace bfl {
    class FilteringAlgorithm;
}


class bfl::FilteringAlgorithm
{
public:
    virtual ~FilteringAlgorithm() noexcept { };

    virtual void runFilter() = 0;

    virtual void getResult() = 0;

};

#endif /* FILTERINGALGORITHM_H */
