#ifndef EIGENMATRIXDATA_H
#define EIGENMATRIXDATA_H

#include <BayesFilters/GenericData.h>

#include <complex>
#include <string>

#include <Eigen/Core>

namespace bfl {
    template <typename Scalar, int Rows, int Cols> class EigenMatrixData;

    typedef EigenMatrixData<float, Eigen::Dynamic, 1>                             EigenVectorXfData;
    typedef EigenMatrixData<float, Eigen::Dynamic, Eigen::Dynamic>                EigenMatrixXfData;
    typedef EigenMatrixData<std::complex<float>, Eigen::Dynamic, Eigen::Dynamic>  EigenMatrixXcfData;
    typedef EigenMatrixData<double, Eigen::Dynamic, 1>                            EigenVectorXdData;
    typedef EigenMatrixData<double, Eigen::Dynamic, Eigen::Dynamic>               EigenMatrixXdData;
    typedef EigenMatrixData<std::complex<double>, Eigen::Dynamic, Eigen::Dynamic> EigenMatrixXcdData;
}


template <typename Scalar, int Rows, int Cols>
class bfl::EigenMatrixData : public bfl::GenericData, public Eigen::Matrix<Scalar, Rows, Cols>
{
public:
    EigenMatrixData() :
        Eigen::Matrix<Scalar, Rows, Cols>()
    { }

    template<typename Derived>
    EigenMatrixData(const Eigen::MatrixBase<Derived>& derived) :
        Eigen::Matrix<Scalar, Rows, Cols>(derived)
    { }

    template<typename Derived>
    EigenMatrixData& operator=(const Eigen::MatrixBase<Derived>& derived)
    {
        this->Eigen::Matrix<Scalar, Rows, Cols>::operator=(derived);
        return *this;
    }

    bool setProperty(const std::string property) override
    {
        return false;
    };
};


#endif /* EIGENMATRIXDATA_H */
