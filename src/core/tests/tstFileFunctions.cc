//---------------------------------*-C++-*-----------------------------------//
/*!
 * \file   src/core/tests/tstFileFunctions.cc
 * \brief  Tests for functions in FileFunctions.
 * \note   Copyright (c) 2023 Oak Ridge National Laboratory, UT-Battelle, LLC.
 */
//---------------------------------------------------------------------------//

#include "../FileFunctions.hh"

#include "harness/Testing.hh"

#include <fstream>
#include <iostream>

//---------------------------------------------------------------------------//
// TESTS
//---------------------------------------------------------------------------//

TEST(FileFunctionsTest, fileExists)
{
    // Write a dummy file
    std::ofstream out("TestFile.dat");
    out << "Dummy file for fileExists test";
    out.close();

    // Test that the file exists
    std::string source_path = "TestFile.dat";
    EXPECT_TRUE(yayp::fileExists(source_path));
    source_path = "./data/ThisWontWork.ini";
    EXPECT_FALSE(yayp::fileExists(source_path));
}

//---------------------------------------------------------------------------//

TEST(FileFunctionsTest, splitFilePath)
{
    // Test basic path example
    std::string    path1   = "/usr/local/bin/file.txt";
    yayp::FilePath result1 = yayp::splitFilepath(path1);
    EXPECT_EQ("/usr/local/bin/", result1.path);
    EXPECT_EQ("file", result1.basename);
    EXPECT_EQ("txt", result1.extension);

    // Test file with no prepended path
    std::string    path2   = "file.txt";
    yayp::FilePath result2 = yayp::splitFilepath(path2);
    EXPECT_EQ("", result2.path);
    EXPECT_EQ("file", result2.basename);
    EXPECT_EQ("txt", result2.extension);

    // Test file with path and no extension
    std::string    path3   = "./file";
    yayp::FilePath result3 = yayp::splitFilepath(path3);
    EXPECT_EQ("./", result3.path);
    EXPECT_EQ("file", result3.basename);
    EXPECT_EQ("", result3.extension);

    // Test file with no path or extension
    std::string path4   = "file";
    auto        result4 = yayp::splitFilepath(path4);
    EXPECT_EQ("", result4.path);
    EXPECT_EQ("file", result4.basename);
    EXPECT_EQ("", result4.extension);

    // Empty filepath
    std::string path5   = "";
    auto        result5 = yayp::splitFilepath(path5);
    EXPECT_EQ("", result5.path);
    EXPECT_EQ("", result5.basename);
    EXPECT_EQ("", result5.extension);

    // Weird path with a '.' in it
    std::string path6   = "/usr/local.dir/file.txt";
    auto        result6 = yayp::splitFilepath(path6);
    EXPECT_EQ("/usr/local.dir/", result6.path);
    EXPECT_EQ("file", result6.basename);
    EXPECT_EQ("txt", result6.extension);
}

//---------------------------------------------------------------------------//
// end of src/core/tests/tstFileFunctions.cc
//---------------------------------------------------------------------------//
