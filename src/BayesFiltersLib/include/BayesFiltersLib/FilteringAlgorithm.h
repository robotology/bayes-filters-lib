#ifndef FILTERINGALGORITHM_H
#define FILTERINGALGORITHM_H

namespace bfl
{

class FilteringAlgorithm {
public:
    virtual ~FilteringAlgorithm() noexcept { };

    virtual void runFilter() = 0;

    virtual void getResult() = 0;

};

} // namespace bfl

#endif /* FILTERINGALGORITHM_H */
