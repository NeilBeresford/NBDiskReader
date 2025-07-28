/**
    @file       Test_File.h
    @brief      This file contains file handling tests for the project.
    @defgroup   ModernCppTests Modern C++ Tests
    @copyright  Copyright (c) 2025, Neil Beresford
*/

//-----------------------------------------------------------------------------
// Libraries
// ----------------------------------------------------------------------------

#pragma once

#include <string>
#include <fstream> // For file handling
#include <vector>
#include <cstdint>
#include <iostream>

//-----------------------------------------------------------------------------
// Classes
// ----------------------------------------------------------------------------

class FileBase
{
  private:
    // Prevent copying
    FileBase( const FileBase& )            = delete;
    FileBase& operator=( const FileBase& ) = delete;
    FileBase( FileBase&& )                 = delete;
    FileBase& operator=( FileBase&& )      = delete;

  protected:
    std::fstream file;
    std::string  filename;
    std::string  fileContent;

  public:
    FileBase()                                       = default;
    ~FileBase()                                      = default;

    virtual bool open( const std::string& filename ) = 0;
    virtual void close()                             = 0;
    virtual bool read( std::string& content )        = 0;
    virtual bool write( const std::string& content ) = 0;
};

class TextFile : public FileBase
{
  public:
    bool open( const std::string& filename ) override;
    void close() override;
    bool read( std::string& content ) override;
    bool write( const std::string& content ) override;
};

class BinaryFile : public FileBase
{
  public:
    bool open( const std::string& filename ) override;
    void close() override;

    bool read( std::string& content ) override;
    bool write( const std::string& content ) override;
    bool read( char* buffer, std::size_t size );
    bool write( const char* buffer, std::size_t size );
};

//-----------------------------------------------------------------------------
// End of file Test_File.h
//-----------------------------------------------------------------------------
