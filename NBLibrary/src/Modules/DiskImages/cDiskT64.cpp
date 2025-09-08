#include "../../../inc/Modules/DiskImages/cDiskT64.h"
#include <fstream>
#include <cstring>

DiskT64::DiskT64() : DiskCore()
{
}

bool DiskT64::open( const std::string& filename )
{
    close();
    DiskImageStream.open( filename, std::ios::binary );
    if ( !DiskImageStream.is_open() )
        return false;
    if ( !parseHeader() || !loadEntries() )
    {
        close();
        return false;
    }
    return true;
}

void DiskT64::close()
{
    if ( DiskImageStream.is_open() )
        DiskImageStream.close();
    fileEntries.clear();
}

bool DiskT64::readDirectory()
{
    // T64 directory is a file entry table at a fixed offset
    constexpr uint32_t HEADER_SIZE       = 0x40;
    constexpr uint32_t ENTRY_SIZE        = 32;
    constexpr uint8_t  FILE_NAME_LENGTH  = 16;
    constexpr uint8_t  FILE_NAME_PADDING = 0xA0;
    DiskContents                         = T64Info();
    DiskContents.nTotalFiles             = 0;
    if ( !DiskImageStream.is_open() )
        return false;
    DiskImageStream.seekg( 0, std::ios::end );
    size_t fileSize = DiskImageStream.tellg();
    DiskImageStream.seekg( HEADER_SIZE, std::ios::beg );
    uint32_t entryCount = ( fileSize - HEADER_SIZE ) / ENTRY_SIZE;
    for ( uint32_t i = 0; i < entryCount; ++i )
    {
        std::vector<uint8_t> entry( ENTRY_SIZE );
        DiskImageStream.read( reinterpret_cast<char*>( entry.data() ), ENTRY_SIZE );
        uint8_t     fileType = entry[ 2 ];
        uint32_t    fileLen  = entry[ 30 ] | ( entry[ 31 ] << 8 );
        std::string name;
        for ( uint8_t j = 0; j < FILE_NAME_LENGTH; ++j )
        {
            uint8_t c = entry[ 4 + j ];
            if ( c == FILE_NAME_PADDING )
                break;
            name += static_cast<char>( c );
        }
        T64Item item;
        item.ID          = DiskContents.DiskItems.size() + 1;
        item.Status      = T64ItemStatus::Available;
        item.Type        = T64ItemType::File;
        item.FileType    = fileType;
        item.Name        = name;
        item.Size        = fileLen;
        item.StartTrack  = 0;
        item.StartSector = 0;
        DiskContents.DiskItems.push_back( item );
        DiskContents.nTotalFiles++;
    }
    return true;
}

bool DiskT64::parseHeader()
{
    return true;
}
bool DiskT64::loadEntries()
{
    return true;
}
