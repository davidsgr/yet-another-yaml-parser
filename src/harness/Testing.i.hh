//---------------------------------*-C++-*-----------------------------------//
/*!
 * \file   src/harness/Testing.i.hh
 * \brief  Testing inline method definitions.
 * \note   Copyright (c) 2023 Oak Ridge National Laboratory, UT-Battelle, LLC.
 */
//---------------------------------------------------------------------------//
#ifndef YAYP_UTILITIES_HARNESS_TESTING_I_HH
#define YAYP_UTILITIES_HARNESS_TESTING_I_HH

#include <numeric>
#include <sstream>
#include <type_traits>
#include <vector>

#include "harness/SoftEqual.hh"
#include "harness/detail/TestingFunctions.hh"

namespace testing_detail
{
//---------------------------------------------------------------------------//
/*!
 * \brief Return whether two values are approximately equal
 *
 * \tparam T1  The type of the expected value
 * \tparam T2  The type of the actual value
 * \param[in] expected_expr  The name of the expected value
 * \param[in] actual_expr    The name of the actual value
 * \param[in] v1             The expected value to test
 * \param[in] v2             The actual value to test
 * \return A GTest AssertionResult object indicating if the test passed
 */
template<typename T1, typename T2>
::testing::AssertionResult isSoftEqual(const char* expected_expr,
                                       const char* actual_expr,
                                       T1          expected_val,
                                       T2          actual_val)
{
    // Create soft-equality object with default precisions
    yayp::SoftEqual<T1, T2> se;
    return isSoftEqual(
        expected_expr, actual_expr, "", expected_val, actual_val, se.rel_tol());
}

//---------------------------------------------------------------------------//
/*!
 * \brief Return whether two values are approximately equal within some
 * relative precision
 *
 * \tparam T1  The type of the expected value
 * \tparam T2  The type of the actual value
 * \tparam Tolerance  The relative tolerance type
 * \param[in] expected_expr  The name of the expected value
 * \param[in] actual_expr    The name of the actual value
 * \param[in] expected_val   The expected value to test
 * \param[in] actual_val     The actual value to test
 * \param[in] rel_tol        Tolerance for relative difference to use in the
 *                           test
 * \return A GTest AssertionResult object indicating if the test passed
 */
template<typename T1, typename T2, typename Tolerance>
::testing::AssertionResult isSoftEqual(const char* expected_expr,
                                       const char* actual_expr,
                                       const char*,
                                       T1        expected_val,
                                       T2        actual_val,
                                       Tolerance rel_tol)
{
    using SoftEqual_t = yayp::SoftEqual<T1, T2>;

    // Set the absolute difference tolerance to epsilon so that we only
    // compare against the relative difference tolerance.
    SoftEqual_t se(
        rel_tol,
        std::numeric_limits<typename SoftEqual_t::value_type>::epsilon());
    if (se(expected_val, actual_val))
    {
        return ::testing::AssertionSuccess();
    }
    else
    {
        constexpr unsigned int precision = 16;

        std::ostringstream msg;
        msg << std::setprecision(precision)
            << "Expected soft equality between these values: "
            << "\n  " << expected_expr << "\n"
            << "    which is " << expected_val << "\n  " << actual_expr << "\n"
            << "    which is " << actual_val;
        if (actual_val != 0)
        {
            msg << "    (rel diff = "
                << std::abs(expected_val - actual_val) / std::abs(actual_val)
                << ")\n";
        }
        else
        {
            msg << "    (rel diff = inf)\n";
        }
        msg << "  with relative tolerance " << se.rel_tol();
        return ::testing::AssertionFailure() << msg.str();
    }
}

//---------------------------------------------------------------------------//
/*!
 * \brief Helper function compatible with GTest for testing container soft
 * equality
 *
 * \tparam Container1  The type of the container of expected values
 * \tparam Container2  The type of the container of actual values
 * \param[in] expected_expr  The name of the expected container
 * \param[in] actual_expr    The name of the actual container
 * \param[in] expected       The container with the expected values
 * \param[in] actual         The container with the actual values
 * \return A GTest AssertionResult object indicating if the test passed
 */
template<class Container1, class Container2>
::testing::AssertionResult isSoftContainerEqual(const char* expected_expr,
                                                const char* actual_expr,
                                                const Container1& expected,
                                                const Container2& actual)
{
    using T1 = typename Container1::value_type;
    using T2 = typename Container2::value_type;

    // Create a soft equals value
    yayp::SoftEqual<T1, T2> se;

    // Call with the default relative precision
    return isSoftContainerEqual(
        expected_expr, actual_expr, "", expected, actual, se.rel_tol());
}

//---------------------------------------------------------------------------//
/*!
 * \brief Helper function compatible with GTest for testing container soft
 * equality
 *
 * \tparam Container1  The type of the container of expected values
 * \tparam Container2  The type of the container of actual values
 * \tparam Tolerance   The relative tolerance type
 * \param[in] expected_expr  The name of the expected container
 * \param[in] actual_expr    The name of the actual container
 * \param[in] expected       The container with the expected values
 * \param[in] actual         The container with the actual values
 * \param[in] rel_tol        The tolerance of the relative difference
 * \return A GTest AssertionResult object indicating if the test passed
 */
template<class Container1, class Container2, typename Tolerance>
::testing::AssertionResult isSoftContainerEqual(const char* expected_expr,
                                                const char* actual_expr,
                                                const char*,
                                                const Container1& expected,
                                                const Container2& actual,
                                                Tolerance         rel_tol)
{
    using T1 = typename Container1::value_type;
    using T2 = typename Container2::value_type;

    // Check whether the containers are the same size
    if (std::size(expected) != std::size(actual))
    {
        return ::testing::AssertionFailure()
               << "Expected soft equality between two containers of unequal "
                  "size\n"
               << "  " << expected_expr << "\n"
               << "    which is size " << std::size(expected) << "\n"
               << "  " << actual_expr << "\n"
               << "    which is size " << std::size(actual);
    }

    // Loop over the containers and find all of the values that are not equal
    std::vector<unsigned int> bad_indices;
    bad_indices.reserve(std::size(actual));
    yayp::SoftEqual<T1, T2> se(rel_tol);
    auto                    iter2 = std::cbegin(actual);
    for (auto iter1 = std::cbegin(expected); iter1 != std::cend(expected);
         ++iter1, ++iter2)
    {
        if (!se(*iter1, *iter2))
        {
            bad_indices.push_back(iter1 - std::cbegin(expected));
        }
    }
    bad_indices.shrink_to_fit();

    // If all positions were good, return success, otherwise return failure
    // with an error message
    if (bad_indices.empty())
    {
        return ::testing::AssertionSuccess();
    }
    else
    {
        return ::testing::AssertionFailure()
               << "Expected soft equality between two containers which differ "
                  "in "
               << bad_indices.size() << " element(s)\n"
               << yayp::detail::write_unequal_values(
                      bad_indices, expected_expr, actual_expr, expected, actual)
               << "\n  tested with relative tolerance " << se.rel_tol();
    }
}

//---------------------------------------------------------------------------//
/*!
 * \brief Helper function compatible with GTest for testing container equality
 *
 * \tparam Container1  The type of the container of expected values
 * \tparam Container2  The type of the container of actual values
 * \param[in] expected_expr  The name of the expected container
 * \param[in] actual_expr    The name of the actual container
 * \param[in] expected       The container with the expected values
 * \param[in] actual         The container with the actual values
 * \return A GTest AssertionResult object indicating if the test passed
 */
template<class Container1, class Container2>
::testing::AssertionResult isContainerEqual(const char*       expected_expr,
                                            const char*       actual_expr,
                                            const Container1& expected,
                                            const Container2& actual)
{
    // Check whether the containers are the same size
    if (std::size(expected) != std::size(actual))
    {
        return ::testing::AssertionFailure()
               << "Expected equality between two containers of unequal "
                  "size\n"
               << "  " << expected_expr << "\n"
               << "    which is size " << std::size(expected) << "\n"
               << "  " << actual_expr << "\n"
               << "    which is size " << std::size(actual);
    }

    // Loop over the containers and find all of the values that are not equal
    std::vector<unsigned int> bad_indices;
    bad_indices.reserve(std::size(actual));
    auto iter2 = std::cbegin(actual);
    for (auto iter1 = std::cbegin(expected); iter1 != std::cend(expected);
         ++iter1, ++iter2)
    {
        if (*iter1 != *iter2)
        {
            bad_indices.push_back(std::distance(std::cbegin(expected), iter1));
        }
    }
    bad_indices.shrink_to_fit();

    // If all positions were good, return success, otherwise return failure
    // with an error message
    if (bad_indices.empty())
    {
        return ::testing::AssertionSuccess();
    }
    else
    {
        return ::testing::AssertionFailure()
               << "Expected equality between two containers which differ in "
               << bad_indices.size() << " elements\n"
               << yayp::detail::write_unequal_values(
                      bad_indices, expected_expr, actual_expr, expected, actual);
    }
}

//---------------------------------------------------------------------------//
} // namespace testing_detail

#endif // YAYP_HARNESS_TESTING_I_HH

//---------------------------------------------------------------------------//
// end of src/harness/Testing.i.hh
//---------------------------------------------------------------------------//
