//---------------------------------*-C++-*-----------------------------------//
/*!
 * \file   src/core/tests/tstStringFunctions.cc
 * \brief  Tests for StringFunctions functions.
 * \note   Copyright (c) 2023 Oak Ridge National Laboratory, UT-Battelle, LLC.
 */
//---------------------------------------------------------------------------//

#include "../StringFunctions.hh"

#include "harness/Testing.hh"

//---------------------------------------------------------------------------//
// TESTS
//---------------------------------------------------------------------------//

TEST(StringFunctionsTest, toLower)
{
    std::string test_str_1 = "ALL UPPERCASE";
    EXPECT_EQ("all uppercase", yayp::toLower(test_str_1));

    std::string test_str_2 = "Mixed CasE";
    EXPECT_EQ("mixed case", yayp::toLower(test_str_2));

    std::string test_str_3 = "all lowercase";
    EXPECT_EQ(test_str_3, yayp::toLower(test_str_3));

    std::string test_str_4 = "";
    EXPECT_EQ(test_str_4, yayp::toLower(test_str_4));
}

//---------------------------------------------------------------------------//

TEST(StringFunctionsTest, toUpper)
{
    std::string test_str_1 = "ALL UPPERCASE";
    EXPECT_EQ(test_str_1, yayp::toUpper(test_str_1));

    std::string test_str_2 = "Mixed CasE";
    EXPECT_EQ("MIXED CASE", yayp::toUpper(test_str_2));

    std::string test_str_3 = "all lowercase";
    EXPECT_EQ("ALL LOWERCASE", yayp::toUpper(test_str_3));

    std::string test_str_4 = "";
    EXPECT_EQ(test_str_4, yayp::toUpper(test_str_4));
}

//---------------------------------------------------------------------------//

TEST(StringFunctionsTest, lstrip_whitespace)
{
    std::string test_str_1 = "    Test 1";
    EXPECT_EQ("Test 1", yayp::lstrip(test_str_1));

    std::string test_str_2 = "Test 2";
    EXPECT_EQ(test_str_2, yayp::lstrip(test_str_2));

    std::string test_str_3 = "     ";
    EXPECT_EQ("", yayp::lstrip(test_str_3));

    std::string test_str_4 = "";
    EXPECT_EQ("", yayp::lstrip(test_str_4));
}

//---------------------------------------------------------------------------//

TEST(StringFunctionsTest, lstrip_chars)
{
    std::string test_str_1 = "aabbccTest 1";
    EXPECT_EQ("Test 1", yayp::lstrip("abc", test_str_1));

    std::string test_str_2 = "Test 2";
    EXPECT_EQ(test_str_2, yayp::lstrip("abc", test_str_2));

    std::string test_str_3 = "aaccbbaa";
    EXPECT_EQ("", yayp::lstrip("abc", test_str_3));

    std::string test_str_4 = "";
    EXPECT_EQ("", yayp::lstrip("abc", test_str_4));
}

//---------------------------------------------------------------------------//

TEST(StringFunctionsTest, rstrip_whitespace)
{
    std::string test_str_1 = "Test 1     ";
    EXPECT_EQ("Test 1", yayp::rstrip(test_str_1));

    std::string test_str_2 = "Test 2";
    EXPECT_EQ(test_str_2, yayp::rstrip(test_str_2));

    std::string test_str_3 = "     ";
    EXPECT_EQ("", yayp::rstrip(test_str_3));

    std::string test_str_4 = "";
    EXPECT_EQ("", yayp::rstrip(test_str_4));
}

//---------------------------------------------------------------------------//

TEST(StringFunctionsTest, rstrip_chars)
{
    std::string test_str_1 = "Test 1aabbcc";
    EXPECT_EQ("Test 1", yayp::rstrip("abc", test_str_1));

    std::string test_str_2 = "Test 2";
    EXPECT_EQ(test_str_2, yayp::rstrip("abc", test_str_2));

    std::string test_str_3 = "aaccbbaa";
    EXPECT_EQ("", yayp::rstrip("abc", test_str_3));

    std::string test_str_4 = "";
    EXPECT_EQ("", yayp::rstrip("abc", test_str_4));
}

//---------------------------------------------------------------------------//

TEST(StringFunctionsTest, strip_whitespace)
{
    std::string test_str_1 = "    Test 1     ";
    EXPECT_EQ("Test 1", yayp::strip(test_str_1));

    std::string test_str_2 = "Test 2";
    EXPECT_EQ(test_str_2, yayp::strip(test_str_2));

    std::string test_str_3 = "     ";
    EXPECT_EQ("", yayp::strip(test_str_3));

    std::string test_str_4 = "";
    EXPECT_EQ("", yayp::strip(test_str_4));
}

//---------------------------------------------------------------------------//

TEST(StringFunctionsTest, strip_chars)
{
    std::string test_str_1 = "aaccbbTest 1bbccaa";
    EXPECT_EQ("Test 1", yayp::strip("abc", test_str_1));

    std::string test_str_2 = "Test 2";
    EXPECT_EQ(test_str_2, yayp::strip("abc", test_str_2));

    std::string test_str_3 = "aabbccbb";
    EXPECT_EQ("", yayp::strip("abc", test_str_3));

    std::string test_str_4 = "";
    EXPECT_EQ("", yayp::strip("abc", test_str_4));
}

//---------------------------------------------------------------------------//

TEST(StringFunctionsTest, split_whitespace)
{
    std::string test_str_1     = "  This is the first test  ";
    auto        split_result_1 = yayp::split(test_str_1);
    EXPECT_EQ(5, split_result_1.size());
    EXPECT_EQ("This", split_result_1[0]);
    EXPECT_EQ("is", split_result_1[1]);
    EXPECT_EQ("the", split_result_1[2]);
    EXPECT_EQ("first", split_result_1[3]);
    EXPECT_EQ("test", split_result_1[4]);

    // Try again, with limited splits
    auto split_result_2 = yayp::split(test_str_1, 3);
    EXPECT_EQ(4, split_result_2.size());
    EXPECT_EQ("This", split_result_2[0]);
    EXPECT_EQ("is", split_result_2[1]);
    EXPECT_EQ("the", split_result_2[2]);
    EXPECT_EQ("first test", split_result_2[3]);

    // Try again with no separators
    std::string test_str_2     = "  No_Split!  ";
    auto        split_result_3 = yayp::split(test_str_2);
    EXPECT_EQ(1, split_result_3.size());
    EXPECT_EQ("No_Split!", split_result_3[0]);

    // Try again with empty string
    std::string test_str_3     = "";
    auto        split_result_4 = yayp::split(test_str_3);
    EXPECT_EQ(0, split_result_4.size());
}

//---------------------------------------------------------------------------//

TEST(StringFunctionsTest, split_separator)
{
    std::string test_str_1     = "  This is the first test  ";
    auto        split_result_1 = yayp::split(test_str_1, "is");
    EXPECT_EQ(3, split_result_1.size());
    EXPECT_EQ("  Th", split_result_1[0]);
    EXPECT_EQ(" ", split_result_1[1]);
    EXPECT_EQ(" the first test  ", split_result_1[2]);

    // Try again, with limited splits
    test_str_1          = "This*!is*!another*!split*!test";
    auto split_result_2 = yayp::split(test_str_1, "*!", 3);
    EXPECT_EQ(4, split_result_2.size());
    EXPECT_EQ("This", split_result_2[0]);
    EXPECT_EQ("is", split_result_2[1]);
    EXPECT_EQ("another", split_result_2[2]);
    EXPECT_EQ("split*!test", split_result_2[3]);

    // Try again with no separators
    std::string test_str_2     = "  No_Split!  ";
    auto        split_result_3 = yayp::split(test_str_2, "hi");
    EXPECT_EQ(1, split_result_3.size());
    EXPECT_EQ("  No_Split!  ", split_result_3[0]);

    // Try again with empty string
    std::string test_str_3     = "";
    auto        split_result_4 = yayp::split(test_str_3);
    EXPECT_EQ(0, split_result_4.size());
}

//---------------------------------------------------------------------------//

TEST(StringFunctionsTest, join)
{
    std::vector<std::string> test_str_1 = {"This", "is", "a", "test"};
    EXPECT_EQ("Thisisatest", yayp::join(test_str_1));
    EXPECT_EQ("This is a test", yayp::join(test_str_1, " "));
    EXPECT_EQ("This, is, a, test", yayp::join(test_str_1, ", "));
    EXPECT_EQ("is**a",
              yayp::join(test_str_1.begin() + 1, test_str_1.end() - 1, "**"));

    std::vector<std::string> test_str_2 = {"This"};
    EXPECT_EQ("This", yayp::join(test_str_2));
    EXPECT_EQ("This", yayp::join(test_str_2, " "));

    std::vector<std::string> test_str_3 = {""};
    EXPECT_EQ("", yayp::join(test_str_3));
}

//---------------------------------------------------------------------------//

TEST(StringFunctionsTest, findAndReplace)
{
    // Simple case
    std::string test_str = "This is the first test string";
    EXPECT_EQ("This is the second test string",
              yayp::findAndReplace(test_str, "first", "second"));

    // More complex with multiple replacements
    test_str = "This**is**another**test*";
    EXPECT_EQ("This is another test*",
              yayp::findAndReplace(test_str, "**", " "));

    // Do again, but with fewer replacements
    EXPECT_EQ("This is another**test*",
              yayp::findAndReplace(test_str, "**", " ", 2));
}

//---------------------------------------------------------------------------//
// end of src/core/tests/tstStringFunctions.cc
//---------------------------------------------------------------------------//
