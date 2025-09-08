#include "../../../inc/Modules/DiskImages/cDiskD71.h"
#include <fstream>
#include <cstring>
#include <set>

DiskD71::DiskD71() : DiskCore()
{
    trackOffsets.clear();
    trackData.clear();
}

bool DiskD71::open( const std::string& filename )
{
    close();
    DiskImageStream.open( filename, std::ios::binary );
    if ( !DiskImageStream.is_open() )
        return false;
    if ( !parseHeader() || !loadTracks() )
    {
        close();
        return false;
    }
    return true;
}

void DiskD71::close()
{
    if ( DiskImageStream.is_open() )
        DiskImageStream.close();
    trackOffsets.clear();
    trackData.clear();
}

bool DiskD71::readDirectory()
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
    DiskContents             = D71Info();
    DiskContents.nTotalFiles = 0;
    while ( track != 0 && sector != 255 )
    {
        if ( !visited.insert( { track, sector } ).second )
            break;
        if ( track == 0 || track > trackData.size() )
            break;
        const std::vector<uint8_t>& dirTrack     = trackData[ track - 1 ];
        size_t                      sectorOffset = 256 * sector;
        if ( dirTrack.size() < sectorOffset + 256 )
            break;
        const uint8_t* sectorData = dirTrack.data() + sectorOffset;
        uint8_t        nextTrack  = sectorData[ 0 ];
        uint8_t        nextSector = sectorData[ 1 ];
        for ( uint8_t i = 0; i < ENTRIES_PER_SECTOR; ++i )
        {
            size_t   entryOffset = i * DIR_ENTRY_SIZE;
            uint8_t  fileType    = sectorData[ entryOffset + 2 ];
            uint8_t  trackFile   = sectorData[ entryOffset + 3 ];
            uint8_t  sectorFile  = sectorData[ entryOffset + 4 ];
            uint16_t fileSize    = ( sectorData[ entryOffset + 30 ] | ( sectorData[ entryOffset + 31 ] << 8 ) );
            if ( fileType == 0x00 )
                continue;
            std::string name;
            for ( uint8_t j = 0; j < FILE_NAME_LENGTH; ++j )
            {
                uint8_t c = sectorData[ entryOffset + FILE_NAME_START + j ];
                if ( c == FILE_NAME_PADDING )
                    break;
                name += static_cast<char>( c );
            }
            D71Item item;
            item.ID          = DiskContents.DiskItems.size() + 1;
            item.Status      = D71ItemStatus::Available;
            item.Type        = D71ItemType::File;
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

bool DiskD71::parseHeader()
{
    return true;
}
bool DiskD71::loadTracks()
{
    return true;
}
