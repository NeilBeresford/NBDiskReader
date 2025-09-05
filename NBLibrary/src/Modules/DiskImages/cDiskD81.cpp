/**
    @file       cDiskD81.cpp
    @brief      Core functionality for disk type D81.
    @defgroup   NBLibraryFileHandling File Handling
    @copyright  Copyright (c) 2025, Neil Beresford
*/

//-----------------------------------------------------------------------------
// Libraries
// ----------------------------------------------------------------------------

#include "../../../inc/Modules/DiskImages/cDiskD81.h"

//-----------------------------------------------------------------------------
// Class functionality
// ----------------------------------------------------------------------------

// 40 sectors per track for all 80 tracks
const std::vector<uint8_t> DiskD81::TrackSectorTable( 80, 40 );

/**
    @ingroup    NBLibraryFileHandling
    @brief      Default constructor for DiskD81 class.
    @details    Initializes member variables to default values.
*/
DiskD81::DiskD81()
{
    DiskTitle   = "NBDisk";
    DiskName    = "NBDisk.d81";
    OutDiskName = DiskName;
    Descriptor.SetupDescriptor( DiskSize, TotalTracks, TrackSectorTable, BAMTrack, BAMSector );
    ResetDiskContents();
}

/**
    @ingroup    NBLibraryFileHandling
    @brief      Resets the contents of the disk image.
    @details    Clears all data from the disk image and resets it to a blank state.
*/
void DiskD81::ResetDiskContents()
{
    DiskContents = D81Info();
}

/**
    @ingroup    NBLibraryFileHandling
    @brief      Calculates the byte offset in the D81 file for a given track and sector.
    @param      track  The track number (1-80).
    @param      sector The sector number (0-39).
    @return     Returns the byte offset from the start of the file, or -1 if invalid track/sector.
*/
size_t DiskD81::calcDiskPosition( uint32_t track, uint32_t sector )
{
    // D81: tracks 1-80, sectors 0-39
    if ( track < 1 || track > TotalTracks || sector >= 40 )
        return size_t( -1 );
    return ( ( track - 1 ) * 40 + sector ) * BytesPerSector;
}

/**
    @ingroup    NBLibraryFileHandling
    @brief      Opens the disk image file.
    @param      filename  The name of the file to open.
    @return     True if successful, false otherwise.
*/
bool DiskD81::open( const std::string& filename )
{
    close();
    DiskImageStream.open( filename, std::ios::binary );
    return DiskImageStream.is_open();
}

/**
    @ingroup    NBLibraryFileHandling
    @brief      Closes the disk image file.
*/
void DiskD81::close()
{
    if ( DiskImageStream.is_open() )
        DiskImageStream.close();
}

/**
    @ingroup    NBLibraryFileHandling
    @brief      Reads the directory of the disk image file.
    @return     True if successful, false otherwise.
*/
bool DiskD81::readDirectory()
{
    if ( !DiskImageStream.is_open() )
        return false;

    // D81 directory is on track 40, sector 3 and following
    // Each directory entry is 32 bytes, 8 entries per sector
    // Directory sectors are chained: first two bytes are next track/sector
    // D81 allows for subdirectories, which are indicated by file type 0x85
    // When a subdirectory is encountered, its track/sector/index is added to a stack and
    // processed after the current directory chain is complete

    const uint32_t            dir_track  = 40;
    uint32_t                  track      = dir_track;
    uint32_t                  sector     = 3;
    uint32_t                  startIndex = 0;
    std::vector<uint8_t>      sector_data( BytesPerSector );
    std::vector<D81NextEntry> dirChain;
    DiskContents             = D81Info();
    DiskContents.nTotalFiles = 0;

    while ( track != 0 && sector != 255 )
    {
        size_t offset = calcDiskPosition( track, sector );
        DiskImageStream.seekg( offset );
        DiskImageStream.read( reinterpret_cast<char*>( sector_data.data() ), BytesPerSector );

        // Next directory sector pointer
        uint8_t next_track  = sector_data[ 0 ];
        uint8_t next_sector = sector_data[ 1 ];

        // 8 entries per sector, 32 bytes each, starting at offset 2
        for ( uint32_t i = startIndex; i < 8; ++i )
        {
            uint32_t entry_offset = i * 32;
            uint8_t  file_type    = sector_data[ entry_offset + 2 ];
            uint8_t  file_track   = sector_data[ entry_offset + 3 ];
            uint8_t  file_sector  = sector_data[ entry_offset + 4 ];
            uint16_t file_size    = ( sector_data[ entry_offset + 30 ] | ( sector_data[ entry_offset + 31 ] << 8 ) );

            startIndex            = 0; // reset for next sector

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

            // Ignore files with sector 160 (deleted files) - this might be down to me when I created the D81
            // disk image
            if ( file_sector != 160 )
            {
                D81Item item;
                item.ID          = DiskContents.DiskItems.size() + 1;
                item.Name        = name;
                item.Size        = file_size;
                item.StartTrack  = file_track;
                item.StartSector = file_sector;
                DiskContents.DiskItems.push_back( item );
                DiskContents.nTotalFiles++;
            }

            // If it's a subdirectory, add current location to stack for later processing
            // and start processing the subdirectory
            if ( file_type == 0x85 )
            {
                dirChain.push_back( { static_cast<uint8_t>( track ), static_cast<uint8_t>( sector ), static_cast<uint8_t>( i + 1 ) } );
                next_track  = file_track;
                next_sector = file_sector;
                break;
            }
        }

        // Move to next directory sector
        track  = next_track;
        sector = next_sector;
        if ( track == 0 || sector == 255 )
        {
            if ( dirChain.size() > 0 )
            {
                D81NextEntry next = dirChain.back();
                dirChain.pop_back();
                track      = next.Track;
                sector     = next.Sector;
                startIndex = next.Index;
            }
            else
                break;
        }
    }
    return true;
}

//-----------------------------------------------------------------------------
// End of file: cDiskD81.cpp
// ----------------------------------------------------------------------------
