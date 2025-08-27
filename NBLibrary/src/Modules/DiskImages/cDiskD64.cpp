/**
    @file       cDiskD64.cpp
    @brief      Core functionality for disk type D64.
    @defgroup   NBLibraryFileHandling File Handling
    @copyright  Copyright (c) 2025, Neil Beresford
*/

//-----------------------------------------------------------------------------
// Libraries
// ----------------------------------------------------------------------------

#include "../../../inc/Modules/DiskImages/cDiskD64.h"

//-----------------------------------------------------------------------------
// Globals
// ----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Class functionality
// ----------------------------------------------------------------------------

// Initialization

/**
    @ingroup    NBLibraryFileHandling
    @brief      Default constructor for DiskCore class.
    @details    Initializes member variables to default values.
*/
DiskD64::DiskD64()
{
    // initialize member variables
    DiskTitle   = "NBDisk";
    DiskName    = "NBDisk.d64";
    OutDiskName = DiskName;

    // Setup the disk descriptor
    Descriptor.SetupDescriptor( DiskSize, TotalTracks, std::vector<uint8_t>( TrackSectorTable, TrackSectorTable + sizeof( TrackSectorTable ) / sizeof( TrackSectorTable[ 0 ] ) ), BAMTrack, BAMSector );
}

void DiskD64::ResetDiskContents()
{
    // Reset the disk contents
    DiskContents = D64Info();
}

/**
    @ingroup    NBLibraryFileHandling
    @brief      Opens the disk image file.
    @param      filename  The name of the file to open.
    @return     True if successful, false otherwise.
*/
bool DiskD64::open( const std::string& filename )
{
    bool bSuccess = false;

    // Check and open the file stream for the disk file
    DiskImageStream.open( filename, std::ios::binary );
    if ( DiskImageStream.is_open() )
    {
        bSuccess = true;
    }

    return bSuccess;
}

/**
    @ingroup    NBLibraryFileHandling
    @brief      Closes the disk image file.
    @note       Does this function need to return status (error?)
*/
void DiskD64::close()
{
    if ( DiskImageStream.is_open() )
    {
        DiskImageStream.close();
    }
}

/**
    @ingroup    NBLibraryFileHandling
    @brief      Reads the directory of the disk image file.
    @return     True if successful, false otherwise.
*/
bool DiskD64::readDirectory()
{
    return false;
}

//-----------------------------------------------------------------------------
// End of file: cDiskD64.cpp
// ----------------------------------------------------------------------------
