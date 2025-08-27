/**
    @file       cFile.cpp
    @brief      This file contains file handling tests for the project.
    @defgroup   NBLibraryFileHandling File Handling
    @copyright  Copyright (c) 2025, Neil Beresford
*/

//-----------------------------------------------------------------------------
// Libraries
// ----------------------------------------------------------------------------

#include <cstdint>
#include <string>
#include <iostream>
#include <vector>
#include <filesystem>                        // For file existence check
#include "../../../inc/Modules/Core/cFile.h" // Include the header file for file handling tests

//-----------------------------------------------------------------------------
// Class Functionality
// ----------------------------------------------------------------------------

/**
    @ingroup    NBLibraryFileHandling
    @brief      Opens a text file.
    @param      filename The name of the file to open.
    @return     True if the file was opened successfully, false otherwise.
 */
bool TextFile::open( const std::string& filename )
{
    if ( filename.empty() )
    {
        std::cout << "Error: Cannot open an empty filename." << std::endl;
        return false; // Cannot open an empty filename
    }
    // check for file existence
    if ( !std::filesystem::exists( filename ) )
    {
        // create the file if it does not exist
        std::cout << "File does not exist. Creating a new file: " << filename << std::endl;
        std::ofstream newFile( filename );
        if ( !newFile )
        {
            std::cout << "Error: Failed to create the file. Name: " << filename << std::endl;
            return false; // Failed to create the file
        }
        newFile.close();
    }

    // Check if the file is already open
    if ( file.is_open() )
    {
        std::cout << "Warning: File is already open. Closing the previous file." << std::endl;
        file.close(); // Close the previously opened file
    }

    // store file name
    this->filename = filename;

    // Open the file
    file.open( this->filename, std::ios::in | std::ios::out );
    if ( !file.is_open() )
    {
        std::cout << "Error: Failed to open the file. Name: " << this->filename << std::endl;
        return false; // Failed to open the file
    }

    // Successfully opened the file, removed cout statement to avoid cluttering output
    // Uncomment the line below if you want to see the file name in the output
    // std::cout << "File opened successfully. Name: " << this->filename << std::endl;
    return true;
}

/**
    @ingroup    NBLibraryFileHandling
    @brief      Closes the text file.
    @return     void
*/
void TextFile::close()
{
    if ( !file.is_open() )
    {
        std::cout << "Warning: File is not open. Nothing to close." << std::endl;
        return; // File is not open
    }
    file.close();
}

/**
    @ingroup    NBLibraryFileHandling
    @brief      Reads content from the text file.
    @param      content The string to store the read content.
    @return     True if reading was successful, false otherwise.
*/
bool TextFile::read( std::string& content )
{
    std::getline( file, content );
    return true;
}

/**
    @ingroup    NBLibraryFileHandling
    @brief      Writes content to the text file.
    @param      content The string to write to the file.
    @return     True if writing was successful, false otherwise.
*/
bool TextFile::write( const std::string& content )
{
    file << content << std::endl; // Adds newline
    return true;
}
//-----------------------------------------------------------------------------
// Binary File Implementation
//-----------------------------------------------------------------------------

/**
    @ingroup    NBLibraryFileHandling
    @brief      Opens a binary file.
    @param      filename The name of the file to open.
    @return     True if the file was opened successfully, false otherwise.
*/
bool BinaryFile::open( const std::string& filename )
{
    if ( filename.empty() )
    {
        std::cout << "Error: Cannot open an empty filename." << std::endl;
        return false; // Cannot open an empty filename
    }
    // check for file existence
    if ( !std::filesystem::exists( filename ) )
    {
        // create the file if it does not exist
        std::cout << "File does not exist. Creating a new file: " << filename << std::endl;
        std::ofstream newFile( filename, std::ios::binary );
        if ( !newFile )
        {
            std::cout << "Error: Failed to create the file. Name: " << filename << std::endl;
            return false; // Failed to create the file
        }
        newFile.close();
    }

    // Check if the file is already open
    if ( file.is_open() )
    {
        std::cout << "Warning: File is already open. Closing the previous file." << std::endl;
        file.close(); // Close the previously opened file
    }

    // store file name
    this->filename = filename;

    // Open the file
    file.open( this->filename, std::ios::in | std::ios::out | std::ios::binary );
    if ( !file.is_open() )
    {
        std::cout << "Error: Failed to open the file. Name: " << this->filename << std::endl;
        return false; // Failed to open the file
    }

    // Successfully opened the file, removed cout statement to avoid cluttering output
    // Uncomment the line below if you want to see the file name in the output
    // std::cout << "File opened successfully. Name: " << this->filename << std::endl;
    return true;
}

/**
    @brief      Closes the binary file.
    @ingroup    NBLibraryFileHandling
    @return     void
*/
void BinaryFile::close()
{
    if ( !file.is_open() )
    {
        std::cout << "Warning: File is not open. Nothing to close." << std::endl;
        return; // File is not open
    }
    file.close();
}

/**
    @brief      Reads content from the binary file.
    @ingroup    NBLibraryFileHandling
    @param      content The string to store the read content.
    @return     False, as binary files cannot be read as strings.
*/
bool BinaryFile::read( std::string& content )
{
    std::cout << "Error: BinaryFile does not support reading as string." << std::endl;
    return false; // Binary files cannot be read as strings
}

/**
    @brief      Writes content to the binary file.
    @ingroup    NBLibraryFileHandling
    @param      content The string to write to the file.
    @return     False, as binary files cannot be written as strings.
*/
bool BinaryFile::write( const std::string& content )
{
    std::cout << "Error: BinaryFile does not support writing as string." << std::endl;
    return false; // Binary files cannot be written as strings
}

/**
    @brief      Reads content from the binary file.
    @ingroup    NBLibraryFileHandling
    @param      buffer The buffer to store the read content.
    @param      size The size of the buffer.
    @return     True if reading was successful, false otherwise.
*/
bool BinaryFile::read( char* buffer, std::size_t size )
{
    if ( !file.is_open() )
    {
        std::cout << "Error: File is not open. Cannot read." << std::endl;
        return false;
    }
    file.read( buffer, size );
    return true;
}

/**
    @brief      Writes content to the binary file.
    @ingroup    NBLibraryFileHandling
    @param      buffer The buffer containing the data to write.
    @param      size The size of the data to write.
    @return     True if writing was successful, false otherwise.
*/
bool BinaryFile::write( const char* buffer, std::size_t size )
{
    if ( !file.is_open() )
    {
        std::cout << "Error: File is not open. Cannot write." << std::endl;
        return false;
    }
    file.write( buffer, size );
    return true;
}

//-----------------------------------------------------------------------------
// End of file Test_File.cpp
//-----------------------------------------------------------------------------
