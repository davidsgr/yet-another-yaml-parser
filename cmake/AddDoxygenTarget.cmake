##---------------------------------------------------------------------------##
## cmake/AddDoxygenTarget.cmake
## Copyright (c) 2023 Oak Ridge National Laboratory, UT-Battelle, LLC.
##---------------------------------------------------------------------------##

include_guard()

#[=======================================================================[.rst:
add_doxygen_target
------------------

Enables Doxygen documentation target

.. cmake:command:: add_doxygen_target

  .. code-block:: cmake

  add_doxygen_target(<TARGET_NAME>
                     [NOPROJECTNAMEPREFIX]
                     [SOURCEDIR]
                     [DOXYGENEXCLUDEPATTERNS pattern1 pattern2 ...]
                    )

  Finds and sets up the Doxygen package

  TARGET_NAME specifies the build target name
  NOPROJECTNAMEPREFIX Disables the project name prefix to the build target
  SOURCEDIR Specifies the directory containing the source to document
  DOXYGENEXCLUDEPATTERNS Specifies globs of files and directories to ignore

#]=======================================================================]

function(add_doxygen_target TARGET_NAME)
  cmake_parse_arguments(PARSE "NOPROJECTNAMEPREFIX"
                       "SOURCEDIR"
                       "DOXYGENEXCLUDEPATTERNS" ${ARGN})

  # Check for doxygen package
  find_package(Doxygen)
  if (NOT DOXYGEN_FOUND)
    message(ERROR "Unable to find doxygen package.  Target ${TARGET_NAME} disabled.")
    return ()
  endif ()

  if (PARSE_DOXYGENEXCLUDEPATTERNS)
    set(DOXYGEN_EXCLUDE_PATTERNS ${PARSE_DOXYGENEXCLUDEPATTERNS})
  endif ()

  if (NOT PARSE_NOPROJECTNAMEPREFIX)
    set(TARGET_NAME "${PROJECT_NAME}-${TARGET_NAME}")
  endif ()

  if (NOT PARSE_SOURCEDIR)
    doxygen_add_docs(${TARGET_NAME}
      ${PROJECT_SOURCE_DIR}
      COMMENT "Generate doxygen pages"
    )
  else ()
    doxygen_add_docs(${TARGET_NAME}
      ${PARSE_SOURCEDIR}
      COMMENT "Generate doxygen pages"
    )
  endif ()
  message(STATUS "Created target '${TARGET_NAME}'")
endfunction()

##---------------------------------------------------------------------------##
## end of cmake/AddDoxygenTarget.cmake
##---------------------------------------------------------------------------##
