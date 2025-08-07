/**
    @file       cDiskCore.h
    @brief      This file contains the adstract class for the different Disk Handling.
    @defgroup   NBLibraryFileHandling File Handling
    @copyright  Copyright (c) 2025, Neil Beresford
*/

//-----------------------------------------------------------------------------
// Libraries
// ----------------------------------------------------------------------------

#pragma once

#include <cstdint>
#include <string>

//-----------------------------------------------------------------------------
// Claess
// ----------------------------------------------------------------------------

/**
    @ingroup    NBLibraryFileHandling
    @brief      Abstract class for disk core functionality.
*/
class DiskCore
{
  public:
    DiskCore();
    virtual ~DiskCore() = default;

    // Prevent copying
    DiskCore( const DiskCore& )            = delete;
    DiskCore& operator=( const DiskCore& ) = delete;
    DiskCore( DiskCore&& )                 = delete;
    DiskCore& operator=( DiskCore&& )      = delete;

    // Storage...
    std::string DiskName;    //!< Name of the disk
    std::string OutDiskName; //!< Output disk name
    std::string DiskTitle;   //!< Title of the disk

    // Core functions
    virtual bool open( const std::string& filename ) = 0;
    virtual void close()                             = 0;
    virtual bool readDirectory()                     = 0;
};

//-----------------------------------------------------------------------------
// End of file: cDiskCore.h
// ----------------------------------------------------------------------------
