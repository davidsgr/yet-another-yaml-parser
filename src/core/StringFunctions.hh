//---------------------------------*-C++-*-----------------------------------//
/*!
 * \file   src/core/StringFunctions.hh
 * \brief  Various string-related functions declarations.
 * \note   Copyright (c) 2023 Oak Ridge National Laboratory, UT-Battelle, LLC.
 */
//---------------------------------------------------------------------------//
#ifndef YAYP_CORE_STRINGFUNCTIONS_HH
#define YAYP_CORE_STRINGFUNCTIONS_HH

#include <limits>
#include <string>
#include <vector>

namespace yayp
{
// >>> CONVERT TO LOWER OR UPPERCASE
// Convert the given string to all lowercase letters
std::string toLower(const std::string& s);

// Convert the given string to all uppercase
std::string toUpper(const std::string& s);

// >>> STRIPPING LEADING AND TRAILING CHARACTERS FROM STRING
// Strip leading whitespace from the string
std::string lstrip(const std::string& s);

// Strip leading characters belonging to the given set from the string
std::string lstrip(const std::string& char_set, const std::string& s);

// Strip trailing whitespace from the string
std::string rstrip(const std::string& s);

// Strip tralining characters belonging to the given set from the string
std::string rstrip(const std::string& char_set, const std::string& s);

// Strip leading and trailing whitespace from the string
std::string strip(const std::string& s);

// Strip leading and trailing characters from the string
std::string strip(const std::string& char_set, const std::string& s);

// >>> SPLITTING A STRING ALONG SOME SEPARATOR
// Split the string using whitespace as the separator up to max_splits number
// of splits
std::vector<std::string>
split(const std::string& s,
      std::size_t        max_splits = std::numeric_limits<std::size_t>::max());

// Split the string using the given separator string up to max_splits number of
// splits
std::vector<std::string>
split(const std::string& s,
      const std::string& sep,
      std::size_t        max_splits = std::numeric_limits<std::size_t>::max());

// >>> JOIN A SERIES OF STRINGS
// Join a container of strings into a single string, separated by an
// optional separator.
template<class Container>
inline std::string
join(const Container& cont, const std::string& separator = "");

// Join a container of strings using iterators, spearated by an optional
// separator
template<class BiDirectionalIterator>
inline std::string join(BiDirectionalIterator begin,
                        BiDirectionalIterator end,
                        const std::string&    separator = "");

// >>> FIND AND REPLACE
// Find the given substring within the string and replace with the replacement
// string
std::string findAndReplace(const std::string& s,
                           const std::string& find_str,
                           const std::string& replace_str,
                           std::size_t        max_replace
                           = std::numeric_limits<std::size_t>::max());

//---------------------------------------------------------------------------//
} // namespace yayp

//---------------------------------------------------------------------------//
// INLINE DEFINITIONS
//---------------------------------------------------------------------------//
#include "StringFunctions.i.hh"

//---------------------------------------------------------------------------//
#endif // YAYP_CORE_STRINGFUNCTIONS_HH
//---------------------------------------------------------------------------//
// end of src/core/StringFunctions.hh
//---------------------------------------------------------------------------//
