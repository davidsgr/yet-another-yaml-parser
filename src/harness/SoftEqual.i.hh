//---------------------------------*-C++-*-----------------------------------//
/*!
 * \file   src/harness/SoftEqual.i.hh
 * \brief  SoftEqual inline method definitions.
 * \note   Copyright (c) 2023 Oak Ridge National Laboratory, UT-Battelle, LLC.
 */
//---------------------------------------------------------------------------//
#ifndef YAYP_HARNESS_SOFTEQUAL_I_HH
#define YAYP_HARNESS_SOFTEQUAL_I_HH

#include "DBC.hh"

#include <cmath>
#include <typeinfo>

namespace
{
//---------------------------------------------------------------------------//
/*!
 * @{
 * \brief Return the default absolute tolerance for the given type
 *
 * \tparam T  The floating point type to return
 * \return The absolute tolerance
 */
template<typename T>
constexpr T default_abs_tol() = delete;
template<>
constexpr double default_abs_tol<double>()
{
    return 1.0e-14;
}
template<>
constexpr float default_abs_tol<float>()
{
    return 1.0e-8f;
}
//!@}

//---------------------------------------------------------------------------//
/*!
 * @{
 * \brief Return the default relative tolerance for the given type
 *
 * \tparam T  The floating point type to return
 * \return The relative tolerance
 */
// >>> DEFINE DEFAULT RELATIVE PRECISIONS
template<typename T>
constexpr T default_rel_tol() = delete;
template<>
constexpr double default_rel_tol<double>()
{
    return 1.0e-12;
}
template<>
constexpr float default_rel_tol<float>()
{
    return 1.0e-6f;
}
//!@}

//---------------------------------------------------------------------------//
} // end anonymous namespace

namespace yayp
{
//---------------------------------------------------------------------------//
// CONSTRUCTORS
//---------------------------------------------------------------------------//
/*!
 * \brief Constructor with default relative and absolute tolerances
 */
template<typename T1, typename T2>
SoftEqual<T1, T2>::SoftEqual()
    : SoftEqual(default_rel_tol<value_type>(), default_abs_tol<value_type>())
{
    /* * */
}

//---------------------------------------------------------------------------//
/*!
 * \brief Constructor with scaled absolute tolerance
 *
 * \param[in] rel_tol  The allowed tolerance of the relative difference
 */
template<typename T1, typename T2>
SoftEqual<T1, T2>::SoftEqual(value_type rel_tol)
    : SoftEqual(rel_tol,
                default_abs_tol<value_type>() / default_rel_tol<value_type>()
                    * rel_tol)
{
    /* * */
}

//---------------------------------------------------------------------------//
/*!
 * \brief Constructor with provided absolute and relative tolerances
 *
 * \param[in] rel_tol  The allowed tolerance of the relative difference
 * \param[in] abs_tol  The allowed tolerance of the absolute difference
 */
template<typename T1, typename T2>
SoftEqual<T1, T2>::SoftEqual(value_type rel_tol, value_type abs_tol)
    : m_abs_tol(abs_tol)
    , m_rel_tol(rel_tol)
{
    YAYP_REQUIRE(m_abs_tol > 0);
    YAYP_REQUIRE(m_rel_tol > 0 && m_rel_tol < 1);
}

//---------------------------------------------------------------------------//
// PUBLIC FUNCTIONS
//---------------------------------------------------------------------------//
/*!
 * \brief Performs soft comparison between two values
 *
 * \param[in] expected  The expected value to test
 * \param[in] actual    The actual value to test
 * \return True if the values are approximately equal, false otherwise.
 */
template<typename T1, typename T2>
bool SoftEqual<T1, T2>::operator()(value_type expected, value_type actual) const
{
    value_type eps_rel = m_rel_tol;
    value_type abs_e   = std::abs(expected);

    // Typical case: relative difference between two values less than relative
    // tolerance
    if (std::abs(actual - expected) < eps_rel * abs_e)
    {
        return true;
    }

    value_type eps_abs = m_abs_tol;
    value_type abs_a   = std::fabs(actual);

    // If one is within the absolute threshold of zero, and the other within
    // relative of zero, they're equal
    if ((abs_e < eps_abs) && (abs_a < eps_rel))
    {
        return true;
    }
    if ((abs_a < eps_abs) && (abs_e < eps_rel))
    {
        return true;
    }

    // If they're both infinite and share a sign, they're equal
    if (std::isinf(expected) && std::isinf(actual)
        && std::signbit(expected) == std::signbit(actual))
    {
        return true;
    }

    return false;
}

//---------------------------------------------------------------------------//
// HELPER FUNCTIONS
//---------------------------------------------------------------------------//
/*!
 * \brief Perform a soft equal comparison between two values
 *
 * \tparam T1          The type of the expected value
 * \tparam T2          The type of the actual value
 * \tparam Tolerances  Optional relative and absolute tolerances
 * \param[in] expected  The expected value to test
 * \param[in] actual    The actual value to test
 * \param[in] prec      Possibly contains the relative and absolute tolerances
 * \return Whether the two given values are approximately equal.
 */
template<typename T1, typename T2, typename... Tolerances>
inline bool softEqual(T1 expected, T2 actual, Tolerances... tol)
{
    using value_type = std::common_type_t<T1, T2>;
    static_assert(std::is_floating_point_v<value_type>,
                  "Soft equal operands must be convertible to a floating "
                  "point type");

    SoftEqual<T1, T2> se(tol...);
    return se(expected, actual);
}

//---------------------------------------------------------------------------//
/*!
 * \brief Perform a soft equal comparison between two containers.
 *
 * Two containers are considered "approximately" equal if:
 * -# the elements in the containers are convertible to a common type,
 * -# the containers contain the same number of elements, and
 * -# each element in the first container is approximately equal to the
 *    corresponding element in the second container.
 *
 * It is not necessary that the two containers be the same type (i.e., it is
 * permissible to test a vector against a list).
 *
 * \tparam Container1  The type of the container of expected values
 * \tparam Container2  The type of the container of actual values
 * \tparam Tolerances  Optional absolute and relative tolerances
 * \param[in] expected  The container of expected values
 * \param[in] actual    The container of actual values
 * \param[in] tol       Possibly contains the relative and absolute tolerances
 * \return Whether the two given containers are approximately equal
 */
template<class Container1, class Container2, typename... Tolerances>
inline bool softContainerEqual(const Container1& expected,
                               const Container2& actual,
                               Tolerances... tol)
{
    using VT1        = typename Container1::value_type;
    using VT2        = typename Container2::value_type;
    using value_type = std::common_type_t<VT1, VT2>;
    static_assert(std::is_floating_point_v<value_type>,
                  "Soft equal operands must be convertible to a floating "
                  "point type");

    // First, check length
    if (std::size(expected) != std::size(actual))
    {
        return false;
    }
    else
    {
        SoftEqual<VT1, VT2> se(tol...);
        auto                iter1 = expected.cbegin();
        auto                iter2 = actual.cbegin();
        while (iter1 != std::cend(expected))
        {
            if (!se(*iter1++, *iter2++))
            {
                return false;
            }
        }
    }

    // If we get down here the containers are equal
    return true;
}

//---------------------------------------------------------------------------//
} // namespace yayp

#endif // YAYP_HARNESS_SOFTEQUAL_I_HH

//---------------------------------------------------------------------------//
// end of src/harness/SoftEqual.i.hh
//---------------------------------------------------------------------------//
