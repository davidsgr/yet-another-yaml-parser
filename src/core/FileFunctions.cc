//---------------------------------*-C++-*-----------------------------------//
/*!
 * \file   src/core/FileFunctions.cc
 * \brief  FileFunctions function definitions.
 * \note   Copyright (c) 2023 Oak Ridge National Laboratory, UT-Battelle, LLC.
 */
//---------------------------------------------------------------------------//

#include "FileFunctions.hh"

#include <fstream>

namespace yayp
{
//---------------------------------------------------------------------------//
/*!
 * \brief Return whether the given file exists
 *
 * \param[in] filename  The filename (possibly including a path) of the file to
 *                      test for existence.
 * \return Boolean indicating whether the file exists
 */
bool fileExists(const std::string& filename)
{
    std::ifstream input(filename.c_str());
    bool          success = !input.fail();
    input.close();
    return success;
}

//---------------------------------------------------------------------------//
/*!
 * \brief Split the given filename into path, basename, and extension
 *
 * \param[in] filepath  The filepath to split
 * \return The split filepath
 */
FilePath splitFilepath(const std::string& filepath)
{
    FilePath result;

    // First, find the *last* '/' character (if it exists).  Everything
    // *before* that (including the final '/') is the path.  If there is no
    // '/', then it is assumed the filepath contains only the filename
    auto path_last = filepath.find_last_of('/');
    if (path_last != std::string::npos)
    {
        result.path = filepath.substr(0, path_last + 1);
    }

    // Extract the filename from the filepath
    std::string filename = filepath.substr(path_last + 1);

    // Search for the last '.' in the filename, and extract the base name
    auto extension_pos = filename.find_last_of('.');
    result.basename    = filename.substr(0, extension_pos);

    // Get the extension
    if (extension_pos != std::string::npos)
    {
        result.extension = filename.substr(extension_pos + 1);
    }
    return result;
}

//---------------------------------------------------------------------------//
} // namespace yayp

//---------------------------------------------------------------------------//
// end of src/core/FileFunctions.cc
//---------------------------------------------------------------------------//