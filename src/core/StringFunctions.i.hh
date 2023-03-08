//---------------------------------*-C++-*-----------------------------------//
/*!
 * \file   src/core/StringFunctions.i.hh
 * \brief  StringFunctions inline function definitions.
 * \note   Copyright (c) 2023 Oak Ridge National Laboratory, UT-Battelle, LLC.
 */
//---------------------------------------------------------------------------//
#ifndef YAYP_CORE_STRINGFUNCTIONS_I_HH
#define YAYP_CORE_STRINGFUNCTIONS_I_HH

#include <iterator>

namespace yayp
{
//---------------------------------------------------------------------------//
/*!
 * \fn join
 * \brief Join a container of strings into a single string adding the separator
 *
 * \tparam Container  The Container type
 * \param[in] cont  Container of strings
 * \param[in] separator  The separator to insert between the strings
 * \return The joined string
 */
template<class Container>
std::string join(const Container& cont, const std::string& separator)
{
    static_assert(std::is_same_v<typename Container::value_type, std::string>,
                  "Join requires string values");
    return yayp::join(std::cbegin(cont), std::cend(cont), separator);
}

//---------------------------------------------------------------------------//
/*!
 * \fn join
 * \brief Join a container of strings into a single string adding the separator
 *
 * \tparam BiDirectionalIterator  A bidirectional iterator pointing to strings
 * \param[in] begin  Beginning iterator into container
 * \param[in] end    Ending iterator into container
 * \param[in] separator  The separator to insert between the strings
 * \return The joined string
 */
template<class BiDirectionalIterator>
std::string join(BiDirectionalIterator begin,
                 BiDirectionalIterator end,
                 const std::string&    separator)
{
    static_assert(
        std::is_same_v<
            typename std::iterator_traits<BiDirectionalIterator>::value_type,
            std::string>,
        "Join requires string values");

    std::string result;

    // Short circuit for empty container
    if (begin == end)
    {
        return result;
    }

    // Loop and append strings, adding separator between
    BiDirectionalIterator iter_end = --end;
    while (begin != iter_end)
    {
        result += *(begin++) + separator;
    }
    result += *iter_end;

    return result;
}

//---------------------------------------------------------------------------//
} // namespace yayp

#endif // YAYP_CORE_STRINGFUNCTIONS_I_HH

//---------------------------------------------------------------------------//
// end of src/core/StringFunctions.i.hh
//---------------------------------------------------------------------------//
