//---------------------------------*-C++-*-----------------------------------//
/*!
 * \file   src/harness/tests/tstDBC.cc
 * \brief  Tests for class DBC.
 * \note   Copyright (c) 2023 Oak Ridge National Laboratory, UT-Battelle, LLC.
 */
//---------------------------------------------------------------------------//

#include "../DBC.hh"

#include <gtest/gtest.h>

//---------------------------------------------------------------------------//
// TESTS
//---------------------------------------------------------------------------//
TEST(DBCTest, GeneralException)
{
    try
    {
        throw yayp::Exception("test");
    }
    catch (const yayp::Exception& e)
    {
        std::string error_msg = e.what();
        EXPECT_EQ("test", error_msg);
    }

    try
    {
        throw yayp::Exception("test", "filename.cc", 100);
    }
    catch (const yayp::Exception& e)
    {
        std::string error_msg = e.what();
        EXPECT_EQ(error_msg,
                  "Caught YAYP exception: test\n ^^^ at filename.cc:100");
    }
}

//---------------------------------------------------------------------------//
#if YAYP_DBC > 0
TEST(DBCTest, Require)
{
    // Check basic functionality of REQUIRE
    EXPECT_NO_THROW(YAYP_REQUIRE(true));
    EXPECT_THROW(YAYP_REQUIRE(false), yayp::DBCException);

    std::string file(__FILE__);

    // Check error message
    try
    {
        YAYP_REQUIRE(false);
    }
    catch (const yayp::DBCException& e)
    {
        std::string error_msg = e.what();
        std::string ref_error_msg
            = "Failed DBC precondition test: false\n ^^^ at " + file + ":53";
        std::cout << "DBC Error message:" << std::endl;
        std::cout << error_msg << std::endl;
        EXPECT_EQ(ref_error_msg, error_msg);
    }
}

//---------------------------------------------------------------------------//
TEST(DBCTest, NotReachable)
{
    // Check NOT_REACHABLE functionality
    EXPECT_THROW(YAYP_NOT_REACHABLE(), yayp::NotReachableException);

    std::string file(__FILE__);

    // Check error message
    try
    {
        YAYP_NOT_REACHABLE();
    }
    catch (const std::exception& e)
    {
        std::string error_msg     = e.what();
        std::string ref_error_msg = "Encountered 'unreachable' code point at "
                                    + file + ":77";
        std::cout << "Not Reachable Error message:" << std::endl;
        std::cout << error_msg << std::endl;
        EXPECT_EQ(ref_error_msg, error_msg);
    }
}
#endif

//---------------------------------------------------------------------------//
#if YAYP_DBC > 1
TEST(DBCTest, Ensure)
{
    // Check basic functionality of ENSURE
    EXPECT_NO_THROW(YAYP_ENSURE(true));
    EXPECT_THROW(YAYP_ENSURE(false), yayp::DBCException);

    std::string file(__FILE__);

    // Check error message
    try
    {
        YAYP_ENSURE(false);
    }
    catch (const yayp::DBCException& e)
    {
        std::string error_msg = e.what();
        std::string ref_error_msg
            = "Failed DBC postcondition test: false\n ^^^ at " + file + ":104";
        std::cout << "DBC Error message:" << std::endl;
        std::cout << error_msg << std::endl;
        EXPECT_EQ(ref_error_msg, error_msg);
    }

    YAYP_REMEMBER(int a = 1);
    EXPECT_NO_THROW(YAYP_ENSURE(a == 1));
}
#endif

//---------------------------------------------------------------------------//
#if YAYP_DBC > 2
TEST(DBCTest, Check)
{
    // Check basic functionality of CHECK
    EXPECT_NO_THROW(YAYP_CHECK(true));
    EXPECT_THROW(YAYP_CHECK(false), yayp::DBCException);

    std::string file(__FILE__);

    // Check error message
    try
    {
        YAYP_CHECK(false);
    }
    catch (const yayp::DBCException& e)
    {
        std::string error_msg = e.what();
        std::string ref_error_msg
            = "Failed DBC intermediate test: false\n ^^^ at " + file + ":134";
        std::cout << "DBC Error message:" << std::endl;
        std::cout << error_msg << std::endl;
        EXPECT_EQ(ref_error_msg, error_msg);
    }
}
#endif

//---------------------------------------------------------------------------//
TEST(DBCTest, NotImplemented)
{
    // Check NOT_IMPLEMENTED functionality
    EXPECT_THROW(YAYP_NOT_IMPLEMENTED("Dummy Message"),
                 yayp::NotImplementedException);

    std::string file(__FILE__);

    // Check error message
    try
    {
        YAYP_NOT_IMPLEMENTED("Dummy message");
    }
    catch (const std::exception& e)
    {
        std::string error_msg = e.what();
        std::string ref_error_msg
            = "Unfortunately, Dummy message is not currently implemented.\n "
              "^^^ at "
              + file + ":160";
        std::cout << "Not Implemented Error message:" << std::endl;
        std::cout << error_msg << std::endl;
        EXPECT_EQ(ref_error_msg, error_msg);
    }
}

//---------------------------------------------------------------------------//
// end of src/harness/tests/tstDBC.cc
//---------------------------------------------------------------------------//
