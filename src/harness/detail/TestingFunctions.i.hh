//---------------------------------*-C++-*-----------------------------------//
/*!
 * \file   src/harness/detail/TestingFunctions.i.hh
 * \brief  TestingFunctions inline method definitions.
 * \note   Copyright (c) 2023 Oak Ridge National Laboratory, UT-Battelle, LLC.
 */
//---------------------------------------------------------------------------//
#ifndef YAYP_HARNESS_DETAIL_TESTINGFUNCTIONS_I_HH
#define YAYP_HARNESS_DETAIL_TESTINGFUNCTIONS_I_HH

#include <algorithm>
#include <iomanip>
#include <limits>
#include <numeric>
#include <sstream>

#include "harness/DBC.hh"

namespace yayp
{
namespace detail
{
//---------------------------------------------------------------------------//
/*!
 * \brief Build a vector of relative differences between elements of the two
 *        containers at the given indices.
 *
 * \tparam Container1  The container type of expected values
 * \tparam Container2  The container type of actual values
 * \param[in] indices Elements in \a expected and \a actual to compute the
 *                    relative differences
 * \param[in] expected  A container of expected values
 * \param[in] actual    A container of actual values
 * \return A container of relative differences between the expected and actual
 *         values for the selected element indices
 */
template<class Container1, class Container2>
auto calc_rel_diffs(const std::vector<unsigned int>& indices,
                    const Container1&                expected,
                    const Container2&                actual)
    -> std::vector<std::common_type_t<typename Container1::value_type,
                                      typename Container2::value_type>>
{
    YAYP_REQUIRE(std::is_sorted(indices.cbegin(), indices.cend()));
    using value_type = std::common_type_t<typename Container1::value_type,
                                          typename Container2::value_type>;
    static_assert(std::is_floating_point<value_type>::value,
                  "Elements in Container1 and Container2 must be convertible "
                  "to a floating point type");

    // Create container to store relative differences
    std::vector<value_type> rel_diffs;
    rel_diffs.reserve(indices.size());

    auto         expected_iter = expected.cbegin();
    auto         actual_iter   = actual.cbegin();
    auto         idx_iter      = indices.cbegin();
    unsigned int index         = 0;
    while (idx_iter != indices.cend() && expected_iter != expected.cend()
           && actual_iter != actual.cend())
    {
        // If this is one of the requested indices, compute the relative
        // difference
        if (index == *idx_iter)
        {
            if (*actual_iter != 0)
            {
                rel_diffs.push_back(std::abs(*expected_iter - *actual_iter)
                                    / std::abs(*actual_iter));
            }
            else
            {
                rel_diffs.push_back(
                    std::numeric_limits<value_type>::infinity());
            }
            ++idx_iter;
        }

        ++expected_iter, ++actual_iter, ++index;
    }

    rel_diffs.shrink_to_fit();
    return rel_diffs;
}

//---------------------------------------------------------------------------//
/*!
 * \brief Get the maximum field width for the values at the given indices
 * in the container
 *
 * Get the maximum number characters necessary to write the values at given
 * indices in the given container with the given precision.
 *
 * \tparam Container  The type of the element container
 * \param[in] header    A string containing the table header for these elements
 * \param[in] indices   The indices of the elements in the container to
 *                      consider
 * \param[in] cont      The container of elements
 * \param[in] precision The precision to use when considering the elements
 * \return The maximum field width necessary to print the elements at the
 *         given indices in the container.
 */
template<class Container>
std::size_t find_max_field_width(const std::string&               header,
                                 const std::vector<unsigned int>& indices,
                                 const Container&                 cont,
                                 unsigned int                     precision)
{
    std::size_t max_field_size = header.size();

    auto         elem_iter = cont.cbegin();
    auto         idx_iter  = indices.cbegin();
    unsigned int index     = 0;
    while (elem_iter != cont.cend() && idx_iter != indices.cend())
    {
        // If the current element index corresponds to one of the listed
        // indices, then we'll consider it's string length
        if (index == *idx_iter)
        {
            // Convert the element to a string using the given precision and
            // update the max field size
            std::ostringstream os;
            os << std::setprecision(precision) << *elem_iter;
            max_field_size = std::max(max_field_size, os.str().size());

            // Increment to the next element index
            ++idx_iter;
        }

        ++elem_iter, ++index;
    }

    // Add one to put a space between table columns
    return max_field_size + 1;
}

//---------------------------------------------------------------------------//
/*!
 * \brief Write the values from the unequal containers if the containers aren't
 * too long.
 *
 * This function is specialized for non-floating-point types.
 *
 * \tparam Container1  The type of the expected values container
 * \tparam Container2  The type of the actual values container
 * \param[in] bad_indices  The indices into the two containers where the
 *                         unequal values are located
 * \param[in] expected_expr  The name of the expected container
 * \param[in] actual_expr    The name of the actual container
 * \param[in] expected       The container with the expected values
 * \param[in] actual         The container with the actual values
 * \return A string containing the unequal values from the containers
 */
template<class Container1, class Container2>
std::string write_unequal_values(const std::vector<unsigned int>& bad_indices,
                                 const char*       expected_expr,
                                 const char*       actual_expr,
                                 const Container1& expected,
                                 const Container2& actual)
{
    using value_type = std::common_type_t<typename Container1::value_type,
                                          typename Container1::value_type>;
    std::ostringstream msg;

    // If we have a modest number of differences (<30) we will print them out
    // explicitly (otherwise do nothing)
    if (bad_indices.size() < 30)
    {
        // Hard-code the output precision
        const std::size_t precision = 16;

        // Get whether we are actually writing floating-point values
        constexpr bool is_floating_point
            = std::is_floating_point_v<typename Container1::value_type>;

        // Get the maximum field width for the indices, the expected
        // values, the actual values, and the relative differences
        std::size_t max_index_width = find_max_field_width(
            "Index", bad_indices, bad_indices, precision);
        std::size_t max_expected_width = find_max_field_width(
            expected_expr, bad_indices, expected, precision);
        std::size_t max_actual_width = find_max_field_width(
            actual_expr, bad_indices, actual, precision);

        // Print the table header
        msg << std::setw(max_index_width) << "Index"
            << std::setw(max_expected_width) << expected_expr
            << std::setw(max_actual_width) << actual_expr;

        // Add relative differences if this is a floating point type
        std::vector<value_type> rel_diffs;
        std::size_t             max_rel_diff_width = 0;
        if constexpr (is_floating_point)
        {
            // Calculate all of the relative differences and a (dummy) list of
            // rel diff indices
            rel_diffs = calc_rel_diffs(bad_indices, expected, actual);
            std::vector<unsigned int> rel_diff_indices(rel_diffs.size());
            std::iota(rel_diff_indices.begin(), rel_diff_indices.end(), 0);
            YAYP_CHECK(rel_diffs.size() == bad_indices.size());

            // Get the maximum field width of the relative differences
            max_rel_diff_width = find_max_field_width(
                "Rel. Diff.", rel_diff_indices, rel_diffs, precision);
            msg << std::setw(max_rel_diff_width) << "Rel. Diff.";
        }

        // The bad indices are sorted, so we will just iterated until we
        // find the next bad index
        auto         bad_idx_iter  = bad_indices.cbegin();
        auto         expected_iter = expected.cbegin();
        auto         actual_iter   = actual.cbegin();
        unsigned int index         = 0;
        auto         rel_diff_iter = rel_diffs.cbegin();
        while (bad_idx_iter != bad_indices.cend()
               && expected_iter != expected.cend()
               && actual_iter != actual.cend())
        {
            if (index == *bad_idx_iter)
            {
                // Write the table row
                msg << std::endl
                    << std::setprecision(precision)
                    << std::setw(max_index_width) << index
                    << std::setw(max_expected_width) << *expected_iter
                    << std::setw(max_actual_width) << *actual_iter;
                ++bad_idx_iter;
                if constexpr (is_floating_point)
                {
                    YAYP_CHECK(rel_diff_iter != rel_diffs.cend());
                    msg << std::setw(max_rel_diff_width) << *rel_diff_iter;
                    ++rel_diff_iter;
                }
            }

            ++index, ++expected_iter, ++actual_iter;
        }
    }
    return msg.str();
}

//---------------------------------------------------------------------------//
} // namespace detail
} // namespace yayp

#endif // YAYP_HARNESS_DETAIL_TESTINGFUNCTIONS_I_HH

//---------------------------------------------------------------------------//
// end of src/harness/detail/TestingFunctions.i.hh
//---------------------------------------------------------------------------//
