//---------------------------------*-C++-*-----------------------------------//
/*!
 * \file   src/core/StringFunctions.cc
 * \brief  StringFunctions function definitions.
 * \note   Copyright (c) 2023 Oak Ridge National Laboratory, UT-Battelle, LLC.
 */
//---------------------------------------------------------------------------//

#include "StringFunctions.hh"

#include <algorithm>
#include <cctype>
#include <iterator>

#include "harness/DBC.hh"

namespace yayp
{
//---------------------------------------------------------------------------//
/*!
 * \fn toLower
 * \brief Convert the given string to all lowercase characters
 *
 * \param[in] s  The string to process
 * \return All lowercase version of \a s.
 */
std::string toLower(const std::string& s)
{
    std::string result(s);
    std::transform(result.begin(), result.end(), result.begin(), [](char c) {
        return std::tolower(c);
    });
    return result;
}

//---------------------------------------------------------------------------//
/*!
 * \fn toUpper
 * \brief Convert the given string to all uppercase characters
 *
 * \param[in] s  The string to process
 * \return All uppercase version of \a s.
 */
std::string toUpper(const std::string& s)
{
    std::string result(s);
    std::transform(result.begin(), result.end(), result.begin(), [](char c) {
        return std::toupper(c);
    });
    return result;
}

//---------------------------------------------------------------------------//
/*!
 * \fn lstrip
 * \brief Strip leading whitespace from the given string and return the result.
 *
 * \param[in] s  The string to process
 * \return The string s with leading whitespace removed.
 */
std::string lstrip(const std::string& s)
{
    auto iter = std::find_if(
        s.cbegin(), s.cend(), [](char c) { return !std::isspace(c); });
    return s.substr(iter - s.cbegin());
}

//---------------------------------------------------------------------------//
/*!
 * \fn lstrip
 * \brief Strip leading characters belonging to the given character set from
 * the given string and return result.
 *
 * \param[in] char_set  The set of characters to strip
 * \param[in] s The string to process
 * \return The string s with the leading characters removed
 */
std::string lstrip(const std::string& char_set, const std::string& s)
{
    auto pos = s.find_first_not_of(char_set);
    return (pos == std::string::npos) ? std::string() : s.substr(pos);
}

//---------------------------------------------------------------------------//
/*!
 * \fn rstrip
 * \brief Strip trailing whitespace from the given string and return the
 * result.
 *
 * \param[in] s  The string to process
 * \return The string s with trailing whitespace removed
 */
std::string rstrip(const std::string& s)
{
    auto iter = std::find_if(
        s.crbegin(), s.crend(), [](char c) { return !std::isspace(c); });
    auto num_chars = s.size() - (iter - s.crbegin());
    return s.substr(0, num_chars);
}

//---------------------------------------------------------------------------//
/*!
 * \fn rstrip
 * \brief Strip trailing characters from the given character set from the
 * string and return the result
 *
 * \param[in] char_set  Character set to remove
 * \param[in] s  The string to process
 * \return The string s with the trailing characters removed
 */
std::string rstrip(const std::string& char_set, const std::string& s)
{
    auto pos = s.find_last_not_of(char_set);
    return s.substr(0, pos + 1);
}

//---------------------------------------------------------------------------//
/*!
 * \fn strip
 * \brief Strip both leading and trailing whitespace from the given string
 *
 * \param[in] s  The string to process
 * \return The string s with leading and trailing whitespace removed
 */
std::string strip(const std::string& s)
{
    std::string result = yayp::lstrip(s);
    result             = yayp::rstrip(result);
    return result;
}

//---------------------------------------------------------------------------//
/*!
 * \fn strip
 * \brief Strip leading and trailing characters from the given character set
 * from the given string
 *
 * \param[in] char_set  The character set to remove
 * \param[in] s  The string to process
 * \return The string s with leading and trailing characters removed
 */
std::string strip(const std::string& char_set, const std::string& s)
{
    std::string result = yayp::lstrip(char_set, s);
    result             = yayp::rstrip(char_set, result);
    return result;
}

//---------------------------------------------------------------------------//
/*!
 * \fn split
 * \brief Split the given string using whitespace as the separator
 *
 * \param[in] s  The string to split
 * \param[in] max_splits  Maximum number of splits to perform
 * \return  A vector of strings split from s
 */
std::vector<std::string> split(const std::string& s, std::size_t max_splits)
{
    // Create vector to hold result
    std::vector<std::string> result;

    // Split all but max_splits - 1
    std::size_t num_splits = 0;
    auto        iter       = s.cbegin();
    while (iter != s.cend() && num_splits < max_splits)
    {
        // Search for first non-whitespace character starting from iter
        iter = std::find_if(
            iter, s.cend(), [](char c) { return !std::isspace(c); });

        // Search for next whitespace character
        auto iter_end = std::find_if(
            iter, s.cend(), [](char c) { return std::isspace(c); });

        // If not an empty string, copy into a new string and push result
        if (iter != iter_end)
        {
            result.emplace_back(iter, iter_end);
        }

        // Update iter and num splits
        iter = iter_end;
        ++num_splits;
    }

    // If we have not hit the end, then the last segment is the remainder
    // of the string (excepting whitespace)
    if (iter != s.cend())
    {
        // Find next non-whitespace character
        iter = std::find_if(
            iter, s.cend(), [](char c) { return !std::isspace(c); });

        // Find last non-whitespace character
        auto iter_end = std::find_if(s.crbegin(),
                                     std::make_reverse_iterator(iter),
                                     [](char c) { return !std::isspace(c); });

        if (iter != iter_end.base())
        {
            result.emplace_back(iter, iter_end.base());
        }
    }

    return result;
}

//---------------------------------------------------------------------------//
/*!
 * \brief Split the string along some separator
 *
 * \param[in] s  The string to split
 * \param[in] sep The separator
 * \param[in] max_splits The maximum number of splits to perform
 * \return A vector of split strings
 */
std::vector<std::string>
split(const std::string& s, const std::string& sep, std::size_t max_splits)
{
    // Create a vector to hold the results
    std::vector<std::string> result;

    // Loop and find all of the substrings
    std::size_t num_splits = 0;
    std::size_t begin_pos  = 0;
    while (begin_pos != std::string::npos && num_splits < max_splits)
    {
        // Search for the beginning of the substring and push into results
        std::size_t end_pos = s.find(sep, begin_pos);
        result.push_back(s.substr(begin_pos, end_pos - begin_pos));

        // Update beginning search position and splits counter
        begin_pos = (end_pos == std::string::npos) ? std::string::npos
                                                   : end_pos + sep.size();
        ++num_splits;
    }

    // Do the last split
    if (begin_pos != std::string::npos)
    {
        result.push_back(s.substr(begin_pos));
    }

    return result;
}

//---------------------------------------------------------------------------//
/*!
 * \brief Find and replace the given substring with a new substring
 *
 * \param[in] s  The string to process
 * \param[in] find_str  The substring to find
 * \param[in] replace_str The substring that replaces the \a find_str substring
 * \param[in] max_replace  The maximum number of replacements to perform
 * \return A new string with \a find_str substrings replaced with
 *         \a replace_str substrings
 */
std::string findAndReplace(const std::string& s,
                           const std::string& find_str,
                           const std::string& replace_str,
                           std::size_t        max_replace)
{
    YAYP_REQUIRE(!find_str.empty());

    // Create result string
    std::string result;

    // Loop and find all of the substrings
    std::size_t num_replacements = 0;
    std::size_t begin_pos        = 0;
    while (begin_pos != std::string::npos && num_replacements < max_replace)
    {
        // Search for the beginning of the find string and append to result
        // string
        std::size_t end_pos = s.find(find_str, begin_pos);
        result += s.substr(begin_pos, end_pos - begin_pos);

        // Append the replace string if we found the find_str
        if (end_pos != std::string::npos)
        {
            result += replace_str;
        }

        // Update beginning search position and replacements counter
        begin_pos = (end_pos == std::string::npos) ? std::string::npos
                                                   : end_pos + find_str.size();
        ++num_replacements;
    }

    // Append any remaining part of the string
    if (begin_pos != std::string::npos)
    {
        result += s.substr(begin_pos);
    }

    return result;
}

//---------------------------------------------------------------------------//
} // namespace yayp

//---------------------------------------------------------------------------//
// end of src/core/StringFunctions.cc
//---------------------------------------------------------------------------//
