/*
 * Vector implementation
 */
template <class T_derived, int N_dimension, class T_number>
T_number & Vector<T_derived, N_dimension, T_number>::operator[](std::size_t aColumn)
{
    return this->at(aColumn);
}


template <class T_derived, int N_dimension, class T_number>
T_number Vector<T_derived, N_dimension, T_number>::operator[](std::size_t aColumn) const
{
    return this->at(aColumn);
}


template <class T_derived, int N_dimension, class T_number>
T_derived & Vector<T_derived, N_dimension, T_number>::operator*=(
            const Matrix<N_dimension, N_dimension, T_number> &aRhs)
{
    (*this) = (*this) * aRhs;
    return *this->derivedThis();
}


template <class T_derived, int N_dimension, class T_number>
T_derived operator*(const Vector<T_derived, N_dimension, T_number> aLhs,
                    const Matrix<N_dimension, N_dimension, T_number> &aRhs)
{
    return multiplyBase<T_derived>(aLhs, aRhs);
}


template <class T_derived, int N_dimension, class T_number>
T_number Vector<T_derived, N_dimension, T_number>::dot(const Vector &aRhs) const
{
    T_number result = 0;
    for(std::size_t col = 0; col != N_dimension; ++col)
    {
        result += (*this)[col] * aRhs[col];
    }
    return result;
}


/*
 * Vec3Base implementation
 */
template <class T_derived, class T_number>
T_derived & Vec3Base<T_derived, T_number>::crossAssign(const Vec3Base &aRhs)
{
    *this = this->cross(aRhs);
    return *this->derivedThis();
}


template <class T_derived, class T_number>
T_derived Vec3Base<T_derived, T_number>::cross(const Vec3Base &aRhs)
{
    return {
        y()*aRhs.z() - z()*aRhs.y(),
        z()*aRhs.x() - x()*aRhs.z(),
        x()*aRhs.y() - y()*aRhs.x(),
    };
}