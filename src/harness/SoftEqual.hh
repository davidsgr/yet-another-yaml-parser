//---------------------------------*-C++-*-----------------------------------//
/*!
 * \file   src/harness/SoftEqual.hh
 * \brief  SoftEqual class declaration.
 * \note   Copyright (c) 2023 Oak Ridge National Laboratory, UT-Battelle, LLC.
 */
//---------------------------------------------------------------------------//
#ifndef YAYP_HARNESS_SOFTEQUAL_HH
#define YAYP_HARNESS_SOFTEQUAL_HH

#include <type_traits>

namespace yayp
{
//===========================================================================//
/*!
 * \class SoftEqual
 * \brief Performs a soft comparison between two floating point types
 *
 * The soft equal class tests whether two values are "approximately" equal. Two
 * values are considered approximately equal if:
 * -  The relative difference between the two values is less than the specified
 *    relative precision; or
 * -  One of the values is close to zero within the specified absolute
 *    precision, and the other value is within zero within the relative
 *    precision.
 * Both values must be floating point types
 *
 * \tparam T1  The type of the first object to compare
 * \tparam T2  The type of the second object to compare
 * \example Utilities/harness/test/tstSoftEqual.cc
 */
//===========================================================================//

template<typename T1, typename T2>
class SoftEqual
{
  public:
    //@{
    //! Public type aliases
    using value_type_1 = T1;
    using value_type_2 = T2;
    using value_type   = std::common_type_t<T1, T2>;
    //@}
    static_assert(std::is_floating_point_v<value_type>,
                  "SoftEqual operands must be convertible to a floating point "
                  "type");

  public:
    // Constructor with default relative and absolute tolerances
    inline SoftEqual();

    // Constructor with default absolute tolerance
    inline explicit SoftEqual(value_type rel_tol);

    // Constructor taking both absolute and relative tolerances
    inline SoftEqual(value_type rel_tol, value_type abs_tol);

    // >>> OPERATOR
    // Performs comparison between two values
    bool operator()(value_type expected, value_type actual) const;

    // >>> ACCESSORS
    //! Return the tolerance of the absolute difference
    value_type abs_tol() const { return m_abs_tol; }

    //! Return the tolerance of the relative difference
    value_type rel_tol() const { return m_rel_tol; }

  private:
    // >>> DATA
    //! Tolerance of absolute difference between the two tested values
    const value_type m_abs_tol;

    //! Tolerance of the relative difference between the two tested values
    const value_type m_rel_tol;
};

//===========================================================================//
// HELPER FUNCTIONS
//===========================================================================//
// Perform soft equal comparison between two values
template<typename T1, typename T2, typename... Tolerances>
inline bool softEqual(T1 expected, T2 actual, Tolerances... t);

// Perform soft equal comparison between two containers of floating point
// values
template<class Container1, class Container2, typename... Tolerances>
inline bool softContainerEqual(const Container1& expected,
                               const Container2& actual,
                               Tolerances... t);

//---------------------------------------------------------------------------//
} // namespace yayp

//---------------------------------------------------------------------------//
// INLINE DEFINITIONS
//---------------------------------------------------------------------------//
#include "SoftEqual.i.hh"

//---------------------------------------------------------------------------//
#endif // YAYP_HARNESS_SOFTEQUAL_HH
//---------------------------------------------------------------------------//
// end of src/harness/SoftEqual.hh
//---------------------------------------------------------------------------//
