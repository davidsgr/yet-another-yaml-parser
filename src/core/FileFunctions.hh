//---------------------------------*-C++-*-----------------------------------//
/*!
 * \file   src/core/FileFunctions.hh
 * \brief  Provides file-system relevant portable functions
 * \note   Copyright (c) 2023 Oak Ridge National Laboratory, UT-Battelle, LLC.
 */
//---------------------------------------------------------------------------//
#ifndef YAYP_CORE_FILEFUNCTIONS_HH
#define YAYP_CORE_FILEFUNCTIONS_HH

#include <string>

namespace yayp
{
// >>> FILE-IO RELATED TYPES
//! Defines a filepath
struct FilePath
{
    std::string path;
    std::string basename;
    std::string extension;
};

// >>> FILE_IO FUNCTIONS
// Return whether the given file exists
bool fileExists(const std::string& filename);

// Split the given filename into path, basename, and extension
FilePath splitFilepath(const std::string& filepath);

//---------------------------------------------------------------------------//
} // namespace yayp

//---------------------------------------------------------------------------//
#endif // YAYP_CORE_FILEFUNCTIONS_HH
//---------------------------------------------------------------------------//
// end of src/core/FileFunctions.hh
//---------------------------------------------------------------------------//
