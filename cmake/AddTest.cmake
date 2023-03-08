##---------------------------------------------------------------------------##
## cmake/AddTests.cmake
## Copyright (c) 2022 Oak Ridge National Laboratory, UT-Battelle, LLC.
##---------------------------------------------------------------------------##

include_guard()

#[=======================================================================[.rst:
vulcan_add_tests
----------------

Add a unit test to the Vulcan test framework.  This function requires that PACKAGE and SUBPACKAGE be defined.  These will be prepended to the test target.

.. cmake:command:: vulcan_add_tests()

   TEST_FILENAME Specifies the C++ test filename

#]=======================================================================]

function(add_test TEST_FILENAME)

  # Compute test name and add test executable
  string(REGEX REPLACE "\\.[^.]*$" "" TEST_NAME ${TEST_FILENAME})
  #set(_TEST_NAME "${_TEST_PREFIX}_${_TEST_NAME}")
  add_executable(${TEST_NAME} ${TEST_FILENAME})

  # Set include and link directories and libraries
  target_include_directories(
      ${TEST_NAME}
      PUBLIC ${PROJECT_SOURCE_DIR}/src)
  target_link_libraries(${TEST_NAME}
                          PUBLIC
                          PRIVATE GTest::gtest GTest::gtest_main yayp)

  # Register test
  include(GoogleTest)
  gtest_discover_tests(
    ${TEST_NAME} XML_OUTPUT_DIR
    "${PROJECT_BINARY_DIR}/Testing/Temporary")
endfunction()

##---------------------------------------------------------------------------##
## end of cmake/AddTests.cmake
##---------------------------------------------------------------------------##
