/**
@file       cDiskCore.cpp
@brief      Core functionality needed for Disk Image Handling.
@defgroup   NBLibraryFileHandling File Handling
@copyright  Copyright (c) 2025, Neil Beresford
*/

//-----------------------------------------------------------------------------
// Libraries
// ----------------------------------------------------------------------------

#include "cDiskCore.h"

//-----------------------------------------------------------------------------
// Class functionality
// ----------------------------------------------------------------------------

/**
    @brief      Default constructor for DiskCore class.
    @details    Initializes member variables to default values.
    @ingroup    NBLibraryFileHandling
*/
cDiskCore::DiskCore()
{
    // initialize member variables
    DiskTitle   = "NBDisk";
    DiskName    = "NBDisk.d64";
    OutDiskName = DiskName;
}

//-----------------------------------------------------------------------------
// End of file: cDiskCore.cpp
// ----------------------------------------------------------------------------
