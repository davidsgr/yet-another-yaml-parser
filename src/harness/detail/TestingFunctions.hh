//---------------------------------*-C++-*-----------------------------------//
/*!
 * \file   src/harness/detail/TestingFunctions.hh
 * \brief  Miscellaneous functions for testing support
 * \note   Copyright (c) 2023 Oak Ridge National Laboratory, UT-Battelle, LLC.
 */
//---------------------------------------------------------------------------//
#ifndef YAYP_HARNESS_DETAIL_TESTINGFUNCTIONS_HH
#define YAYP_HARNESS_DETAIL_TESTINGFUNCTIONS_HH

#include <string>
#include <type_traits>
#include <vector>

namespace utils
{
namespace detail
{
// Build a vector of relative differences between elements of the two
// containers at the given indices.
template<class Container1, class Container2>
inline auto calc_rel_diffs(const std::vector<unsigned int>& indices,
                           const Container1&                expected,
                           const Container2&                actual)
    -> std::vector<std::common_type_t<typename Container1::value_type,
                                      typename Container2::value_type>>;

// Compute the maximum field width to print select values from the container
template<class Container>
inline std::size_t
find_max_field_width(const std::string&               header,
                     const std::vector<unsigned int>& indices,
                     const Container&                 cont,
                     unsigned int                     precision);

// Write a string containing a table of the unequal values in the given two
// containers at the given bad indices
template<class Container1, class Container2>
inline std::string
write_unequal_values(const std::vector<unsigned int>& bad_indices,
                     const char*                      expected_expr,
                     const char*                      actual_expr,
                     const Container1&                expected,
                     const Container2&                actual);

//---------------------------------------------------------------------------//
} // namespace detail
} // namespace utils

//---------------------------------------------------------------------------//
// INLINE DEFINITIONS
//---------------------------------------------------------------------------//
#include "TestingFunctions.i.hh"

//---------------------------------------------------------------------------//
#endif // YAYP_HARNESS_DETAIL_TESTINGFUNCTIONS_HH
//---------------------------------------------------------------------------//
// end of src/harness/detail/TestingFunctions.hh
//---------------------------------------------------------------------------//
