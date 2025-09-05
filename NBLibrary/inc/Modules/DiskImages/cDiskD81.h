#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include "cDiskCore.h"

/**
    @ingroup    NBLibraryFileHandling
    @brief      Class for disk type D81 (Commodore 1581 disk image).
*/
class DiskD81 : protected DiskCore
{
  private:
    // D81-specific constants
    static constexpr uint32_t         DiskSize        = 819200; // 80 tracks * 40 sectors * 256 bytes
    static constexpr uint16_t         TotalTracks     = 80;
    static constexpr uint16_t         TotalSectors    = 1600; // 80 * 40
    static constexpr uint16_t         BytesPerSector  = 256;
    static constexpr uint8_t          BAMTrack        = 40; // D81 BAM is on track 40
    static constexpr uint8_t          BAMSector       = 0;
    static constexpr uint8_t          FileNameLength  = 16;
    static constexpr uint8_t          FileNameStart   = 5;
    static constexpr uint8_t          FileNamePadding = 0xA0;

    static const std::vector<uint8_t> TrackSectorTable;

    struct D81Item
    {
        uint32_t    ID;
        std::string Name;
        uint32_t    Size;
        uint32_t    StartTrack;
        uint32_t    StartSector;
        // Add more fields as needed
    };

    struct D81Info
    {
        std::string          DiskName;
        std::string          DiskTitle;
        uint32_t             nTotalFiles;
        std::vector<D81Item> DiskItems;
        D81Info() : DiskName( "NBDisk" ), DiskTitle( "NBDisk.d81" ), nTotalFiles( 0 )
        {
        }
    };

    struct D81NextEntry
    {
        uint8_t Track;
        uint8_t Sector;
        uint8_t Index;
    };

    D81Info DiskContents;
    size_t  calcDiskPosition( uint32_t track, uint32_t sector );

  public:
    DiskD81();
    virtual ~DiskD81()                   = default;
    DiskD81( const DiskD81& )            = delete;
    DiskD81& operator=( const DiskD81& ) = delete;
    DiskD81( DiskD81&& )                 = delete;
    DiskD81&     operator=( DiskD81&& )  = delete;

    void         ResetDiskContents();
    virtual bool open( const std::string& filename ) override;
    virtual void close() override;
    virtual bool readDirectory() override;
};
