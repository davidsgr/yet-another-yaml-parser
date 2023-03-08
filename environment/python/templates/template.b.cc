//---------------------------------*-C++-*-----------------------------------//
/*!
 * \file   {filename}
 * \brief  {base} python bindings definitions.
 * \note   Copyright (c) {year} Oak Ridge National Laboratory, UT-Battelle, LLC.
 */
//---------------------------------------------------------------------------//

#include "{base}.hh"

#include <pybind11/pybind11.h>

namespace py = pybind11;

{begin_namespace}
//---------------------------------------------------------------------------//
/*!
 * \brief Brief description of python module here
 */
PYBIND11_MODULE({lowerbase}, mod)
{{
    mod.doc() = "Module docstring here!";

    // Add python binding code here...
}}

//---------------------------------------------------------------------------//
{end_namespace}

//---------------------------------------------------------------------------//
// end of {filename}
//---------------------------------------------------------------------------//
