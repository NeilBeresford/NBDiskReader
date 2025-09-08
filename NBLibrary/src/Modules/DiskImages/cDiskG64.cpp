/**
    @file       cDiskG64.cpp
    @brief      Class for disk type G64 (Commodore 1541 GCR disk image).
    @defgroup   NBLibraryFileHandling Disk Image Handling
    @copyright  Copyright (c) 2025, Neil Beresford
*/

//-----------------------------------------------------------------------------
// Libraries
// ----------------------------------------------------------------------------

#include "../../../inc/Modules/DiskImages/cDiskG64.h"
#include <fstream>
#include <cstring>
#include <set>

//-----------------------------------------------------------------------------
// Class functionality
// ----------------------------------------------------------------------------

/**
    @ingroup    NBLibraryFileHandling
    @brief      Default constructor for DiskG64.
*/
bool DiskG64::readDirectory()
{
    constexpr uint8_t                     DIR_TRACK          = 18;
    constexpr uint8_t                     DIR_SECTOR         = 1;
    constexpr uint8_t                     DIR_ENTRY_SIZE     = 32;
    constexpr uint8_t                     ENTRIES_PER_SECTOR = 8;
    constexpr uint8_t                     FILE_NAME_LENGTH   = 16;
    constexpr uint8_t                     FILE_NAME_START    = 5;
    constexpr uint8_t                     FILE_NAME_PADDING  = 0xA0;

    uint8_t                               track              = DIR_TRACK;
    uint8_t                               sector             = DIR_SECTOR;
    std::set<std::pair<uint8_t, uint8_t>> visited;

    // Initialize disk contents
    DiskContents             = G64Info();
    DiskContents.nTotalFiles = 0;

    while ( track != 0 && sector != 255 )
    {
        // Defensive: avoid infinite loops
        if ( !visited.insert( { track, sector } ).second )
            break;

        if ( track == 0 || track > trackData.size() )
            break;
        const std::vector<uint8_t>& dirTrack     = trackData[ track - 1 ];
        size_t                      sectorOffset = 256 * sector;
        if ( dirTrack.size() < sectorOffset + 256 )
            break;
        const uint8_t* sectorData = dirTrack.data() + sectorOffset;

        // Next track/sector for directory
        uint8_t nextTrack  = sectorData[ 0 ];
        uint8_t nextSector = sectorData[ 1 ];

        // Parse directory entries
        for ( uint8_t i = 0; i < ENTRIES_PER_SECTOR; ++i )
        {
            size_t   entryOffset = i * DIR_ENTRY_SIZE;
            uint8_t  fileType    = sectorData[ entryOffset + 2 ];
            uint8_t  trackFile   = sectorData[ entryOffset + 3 ];
            uint8_t  sectorFile  = sectorData[ entryOffset + 4 ];
            uint16_t fileSize    = ( sectorData[ entryOffset + 30 ] | ( sectorData[ entryOffset + 31 ] << 8 ) );

            if ( fileType == 0x00 )
                continue; // Unused entry

            // File name: 16 bytes, padded with 0xA0
            std::string name;
            for ( uint8_t j = 0; j < FILE_NAME_LENGTH; ++j )
            {
                uint8_t c = sectorData[ entryOffset + FILE_NAME_START + j ];
                if ( c == FILE_NAME_PADDING )
                    break;
                name += static_cast<char>( c );
            }

            DiskG64::G64Item item;
            item.ID          = DiskContents.DiskItems.size() + 1;
            item.Status      = DiskG64::G64ItemStatus::Available;
            item.Type        = DiskG64::G64ItemType::File;
            item.FileType    = fileType;
            item.Name        = name;
            item.Size        = fileSize;
            item.StartTrack  = trackFile;
            item.StartSector = sectorFile;

            DiskContents.DiskItems.push_back( item );
            DiskContents.nTotalFiles++;
        }

        track  = nextTrack;
        sector = nextSector;
    }
    return true;
}

/**
    @ingroup    NBLibraryFileHandling
    @brief  Parse the G64 header.
    @return true if header is valid, false otherwise.
*/
bool DiskG64::parseHeader()
{
    if ( !DiskImageStream.is_open() )
        return false;
    G64Header header{};
    DiskImageStream.seekg( 0, std::ios::beg );
    DiskImageStream.read( reinterpret_cast<char*>( &header ), sizeof( G64Header ) );
    if ( std::strncmp( header.signature, "GCR-1541", 8 ) != 0 )
    {
        return false;
    }
    // Store track count, etc. as needed
    return true;
}

/**
    @brief  Load all track data from the G64 image.
    @return true if successful, false otherwise.
*/
bool DiskG64::loadTracks()
{
    // Stub: actual implementation would parse track offsets and load data
    return true;
}

//-----------------------------------------------------------------------------
// End of file: cDiskG64.cpp
// ----------------------------------------------------------------------------
