#ifndef EIGENMATRIXDATA_H
#define EIGENMATRIXDATA_H

#include <BayesFilters/GenericData.h>

#include <complex>
#include <string>

#include <Eigen/Core>

namespace bfl {
    template <typename Scalar, int Rows, int Cols, int Options = Eigen::AutoAlign | ((Rows == 1 && Cols != 1) ? Eigen::RowMajor : (Cols == 1 && Rows != 1) ? Eigen::ColMajor : Eigen::ColMajor), int MaxRows = Rows, int MaxCols = Cols>
    class EigenMatrixData;

    typedef EigenMatrixData<int, Eigen::Dynamic, 1>                               EigenVectorXiData;
    typedef EigenMatrixData<int, Eigen::Dynamic, Eigen::Dynamic>                  EigenMatrixXiData;
    typedef EigenMatrixData<std::complex<int>, Eigen::Dynamic, Eigen::Dynamic>    EigenMatrixXciData;
    typedef EigenMatrixData<float, Eigen::Dynamic, 1>                             EigenVectorXfData;
    typedef EigenMatrixData<float, Eigen::Dynamic, Eigen::Dynamic>                EigenMatrixXfData;
    typedef EigenMatrixData<std::complex<float>, Eigen::Dynamic, Eigen::Dynamic>  EigenMatrixXcfData;
    typedef EigenMatrixData<double, Eigen::Dynamic, 1>                            EigenVectorXdData;
    typedef EigenMatrixData<double, Eigen::Dynamic, Eigen::Dynamic>               EigenMatrixXdData;
    typedef EigenMatrixData<std::complex<double>, Eigen::Dynamic, Eigen::Dynamic> EigenMatrixXcdData;
}


template <typename Scalar, int Rows, int Cols, int Options, int MaxRows, int MaxCols>
class bfl::EigenMatrixData : public bfl::GenericData, public Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols>
{
public:
    EigenMatrixData() :
        Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols>()
    { }

    template<typename Derived>
    EigenMatrixData(const Eigen::MatrixBase<Derived>& derived) :
        Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols>(derived)
    { }

    template<typename Derived>
    EigenMatrixData& operator=(const Eigen::MatrixBase<Derived>& derived)
    {
        this->Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols>::operator=(derived);
        return *this;
    }
};


#endif /* EIGENMATRIXDATA_H */
