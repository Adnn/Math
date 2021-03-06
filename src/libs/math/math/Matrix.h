#pragma once


#include "commons.h"
#include "MatrixBase.h"


namespace ad {
namespace math {


template <int N_rows, int N_cols, class T_number=real_number>
class Matrix : public MatrixBase<Matrix<N_rows, N_cols, T_number>, N_rows, N_cols, T_number>
{
    typedef MatrixBase<Matrix<N_rows, N_cols, T_number>, N_rows, N_cols, T_number> base_type;
    using base_type::base_type;
    using base_type::should_noexcept;

public:
    template<class T>
    using derived_type = Matrix<N_rows, N_cols, T>;

    static constexpr bool is_square_value{N_rows == N_cols};

    static constexpr Matrix Identity() noexcept(should_noexcept);

    constexpr Matrix<N_cols, N_rows, T_number> transpose() const noexcept(should_noexcept);

    using base_type::operator*=;
    constexpr Matrix & operator*=(const Matrix & aRhs) noexcept(should_noexcept);
};


template <class T_result, int N_lRows, int N_lCols, int N_rRows, int N_rCols, class T_lDerived, class T_number>
constexpr T_result multiplyBase(const MatrixBase<T_lDerived, N_lRows, N_lCols, T_number> &aLhs,
                                const Matrix<N_rRows, N_rCols, T_number> &aRhs)
{
    T_result result = T_result::Zero();
    for(std::size_t row = 0; row != N_lRows; ++row)
    {
        for(std::size_t col = 0; col != N_rCols; ++col)
        {
            // Inner multiplication
            for(std::size_t index = 0; index != N_rRows; ++index)
            {
                // Uses at() on result instead of double subscript, because T_result may be Vector type.
                result.at(row, col) += aLhs[row][index] * aRhs[index][col];
            }

        }
    }
    return result;
}


template <int N_lRows, int N_lCols, int N_rRows, int N_rCols, class T_number>
constexpr Matrix<N_lRows, N_rCols, T_number>
operator*(const Matrix<N_lRows, N_lCols, T_number> &aLhs,
          const Matrix<N_rRows, N_rCols, T_number> &aRhs)
{
    static_assert(N_lCols == N_rRows, "Matrix multiplication dimension mismatch.");
    return multiplyBase<Matrix<N_lRows, N_rCols, T_number>>(aLhs, aRhs);
}


template <int N_rows, int N_cols, class T_number>
constexpr auto Matrix<N_rows, N_cols, T_number>::operator*=(const Matrix & aRhs)
noexcept(should_noexcept) -> Matrix &
{
    static_assert(is_square_value,
                  "Matrix multiplication assignment only available for square Matrices");
    *this = *this * aRhs;
    return *this;
}


template <int N_rows, int N_cols, class T_number>
constexpr Matrix<N_rows, N_cols, T_number> Matrix<N_rows, N_cols, T_number>::Identity()
noexcept(should_noexcept)
{
    static_assert(is_square_value, "Only square matrices can be identity.");

    Matrix result = Matrix::Zero();
    for(std::size_t index = 0; index != N_rows; ++index)
    {
        result[index][index] = 1;
    }
    return result;
}


template <int N_rows, int N_cols, class T_number>
constexpr Matrix<N_cols, N_rows, T_number>
Matrix<N_rows, N_cols, T_number>::transpose() const noexcept(should_noexcept)
{
    using result_type = Matrix<N_cols, N_rows, T_number>;
    result_type result{typename result_type::UninitializedTag{}};
    for(std::size_t sourceRow = 0; sourceRow != N_rows; ++sourceRow)
    {
        for(std::size_t sourceCol = 0; sourceCol != N_cols; ++sourceCol)
        {
            result[sourceCol][sourceRow] = (*this)[sourceRow][sourceCol];
        }
    }
    return result;
}


}} // namespace ad::math
