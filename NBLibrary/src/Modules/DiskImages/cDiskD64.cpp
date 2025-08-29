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

/**
    @ingroup    NBLibraryFileHandling
    @brief      Resets the contents of the disk image.
    @details    Clears all data from the disk image and resets it to a blank state.
*/
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
    std::vector<uint8_t> sector_data( BytesPerSector );
    uint32_t             track  = BAMTrack;
    uint32_t             sector = BAMSector;

    // initialize
    DiskContents             = D64Info();
    DiskContents.nTotalFiles = 0;

    // Work through the BAM sectors retrieving the file details
    while ( track != 0 && sector != 255 )
    {
        size_t offset = calcDiskPosition( track, sector );

        DiskImageStream.seekg( offset );
        DiskImageStream.read( reinterpret_cast<char*>( sector_data.data() ), BytesPerSector );

        // if the first BAM sector, contains disk information
        if ( track == BAMTrack && sector == BAMSector )
        {
            /**
               BAM (Block Allocation Map) is located at track 18, sector 0
                It contains information about the disk's usage, including the disk name and the sectors that have been used.
                The BAM is structured as follows:
                - The first 2 bytes are the track and sector of the next BAM.
                - The next 2 bytes are the track and sector of the next directory.
                - The next 140 bytes are the BAM entries for each track (35 tracks, 4 bytes per track).
                - The next 16 bytes are the disk name, padded with 0xA0.
            */
            std::string disk_name;
            for ( uint32_t i = 0; i < 16; ++i )
            {
                uint8_t c = sector_data[ 144 + i ];
                if ( c == 0xA0 )
                    break; // End of name
                disk_name += static_cast<char>( c );
            }
            DiskContents.DiskName  = disk_name;
            DiskContents.DiskTitle = disk_name;
        }

        // Next track/sector for directory
        /*
            Directory sectors are located at track 18, sector 1 onwards.
            Each directory sector contains 8 entries, each entry is 32 bytes.
            The first 2 bytes of each entry are the track and sector of the file.
            The file type is in the 3rd byte, and the file name starts at the 6th byte.
            The file size in sectors is in the last 2 bytes of the entry.
        */

        track  = sector_data[ 0 ];
        sector = sector_data[ 1 ];
        if ( track == 0 && sector == 255 )
            break; // No more directory sectors

        offset = calcDiskPosition( track, sector );
        DiskImageStream.seekg( offset );
        DiskImageStream.read( reinterpret_cast<char*>( sector_data.data() ), BytesPerSector );

        // 8 entries per sector, 32 bytes each, starting at offset 2 (30 bytes directory entry with 2 bytes padding)
        for ( uint32_t i = 0; i < BAMEntities; ++i )
        {
            uint32_t entry_offset = i * BAMEntitySize + BAMLocationHeader;
            uint8_t  file_type    = sector_data[ entry_offset ];
            uint8_t  track_file   = sector_data[ entry_offset + 1 ];
            uint8_t  sector_file  = sector_data[ entry_offset + 2 ];
            uint16_t file_size    = ( sector_data[ entry_offset + 28 ] | ( sector_data[ entry_offset + 29 ] << 8 ) );

            if ( file_type == 0x00 )
                continue; // Unused entry

            // File name: 16 bytes, padded with 0xA0
            std::string name;
            for ( uint32_t j = 0; j < FileNameLength; ++j )
            {
                uint8_t c = sector_data[ entry_offset + FileNameStart + j ];
                if ( c == FileNamePadding )
                    break;
                name += static_cast<char>( c );
            }

            D64Item item;
            item.ID          = DiskContents.DiskItems.size() + 1;
            item.Status      = D64ItemStatus::Available;
            item.Type        = D64ItemType::File;
            item.FileType    = static_cast<D64ItemFileType>( file_type );
            item.Name        = name;
            item.Size        = file_size;
            item.StartTrack  = track_file;
            item.StartSector = sector_file;

            DiskContents.DiskItems.push_back( item );
            DiskContents.nTotalFiles++;
        }
    }

    return true;
}
//-----------------------------------------------------------------------------
// Support Functionality
// ----------------------------------------------------------------------------

/**
    @brief      Calculate the disk position for a given track and sector.
    @param      track   The track number (1-based).
    @param      sector  The sector number (1-based).
    @return     The calculated disk position in bytes.
*/
size_t DiskD64::calcDiskPosition( uint32_t track, uint32_t sector )
{
    size_t offset = 0;
    for ( uint32_t t = 0; t < track - 1; ++t )
    {
        offset += TrackSectorTable[ t ];
    }
    offset += sector;
    return ( offset * BytesPerSector );
}

//-----------------------------------------------------------------------------
// End of file: cDiskD64.cpp
// ----------------------------------------------------------------------------
