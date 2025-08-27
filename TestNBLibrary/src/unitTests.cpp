/**
    @file       unitTests.cpp
    @defgroup   ModernCppTests Modern C++ Tests
    @brief      Main file for running unittests for GenTests.
    @copyright  Copyright (c) 2025, Neil Beresford
*/

//-----------------------------------------------------------------------------
// Libraries
// ----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

// doctest unit testing framework setup
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../../NBLibrary/inc/Modules/Core/cFile.h"
#include "../../doctest/doctest/doctest.h"

//-----------------------------------------------------------------------------
// Namespace access
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Unit Tests
//-----------------------------------------------------------------------------

TEST_SUITE( "Test - NB Library" )
{
    //-----------------------------------------------------------------------------
    TEST_CASE( "Testing File Operations" )
    //-----------------------------------------------------------------------------
    {
        SUBCASE( "Testing text file operations" )
        {
            TextFile textFile;

            CHECK( textFile.open( "test.txt" ) == true );
            CHECK( textFile.write( "Hello, World!" ) == true );
            textFile.close();
            CHECK( textFile.open( "test.txt" ) == true );
            std::string content;
            CHECK( textFile.read( content ) == true );
            CHECK( content == "Hello, World!" );
            textFile.close();
        } // End of Text File Operations test
        SUBCASE( "Testing Binary File Operations" )
        {
            BinaryFile binaryFile;

            CHECK( binaryFile.open( "test.bin" ) == true );
            CHECK( binaryFile.write( "Hello, World!", 13 ) == true );
            binaryFile.close();
            CHECK( binaryFile.open( "test.bin" ) == true );
            char buffer[ 14 ] = { 0 }; // Buffer to read 13 bytes +
            CHECK( binaryFile.read( buffer, 13 ) == true );
            CHECK( std::string( buffer ) == "Hello, World!" );
            binaryFile.close();
        } // End of Binary File Operations test
    } // End of File Operations test
    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
} // Test - NBLibrary

//-----------------------------------------------------------------------------
// End of file: unitTests.cpp
//-----------------------------------------------------------------------------
