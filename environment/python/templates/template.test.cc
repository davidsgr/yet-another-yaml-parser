//---------------------------------*-C++-*-----------------------------------//
/*!
 * \file   {filename}
 * \brief  Tests for class {base}.
 * \note   Copyright (c) {year} Oak Ridge National Laboratory, UT-Battelle,
 * LLC.
 */
//---------------------------------------------------------------------------//

#include "../{base}.hh"

#include "Utilities/harness/Testing.hh"

using {namespace}::{base};

//---------------------------------------------------------------------------//
// Test fixture
//---------------------------------------------------------------------------//
class {compressed}Test : public ::testing::Test
{{
  protected:
      // >>> TYPE ALIASES

  protected:

    static void SetUpTestCase()
    {{
          // Put static test case setup code here!
          // This code will be called *once* to setup the test cases
    }}

    void SetUp()
    {{
        // Put setup code here!
        // This code will be called at the beginning of each test
    }}

    void TearDown()
    {{
        // Put shutdown code here!
        // This code will be called at the end of each test
    }}

    static void TearDownTestCase()
    {{
        // Put static test case tear down code here
        // This code will be called *once* to tear down the test cases
    }}

  protected:
    // >>> DATA
    // Put test data here
}};

//---------------------------------------------------------------------------//
// TESTS
//---------------------------------------------------------------------------//

TEST_F({compressed}Test, subtest_description)
{{
    // Put your test code in here!
}}

//---------------------------------------------------------------------------//
// end of {filename}
//---------------------------------------------------------------------------//
