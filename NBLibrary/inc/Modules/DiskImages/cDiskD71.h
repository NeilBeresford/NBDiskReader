/**
    @file       cDiskD71.h
    @brief      Class for disk type D71 (Commodore 1571 disk image).
    @defgroup   NBLibraryFileHandling Disk Image Handling
    @copyright  Copyright (c) 2025, Neil Beresford
*/

#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include "cDiskCore.h"

class DiskD71 : protected DiskCore
{
  public:
    enum class D71ItemStatus : uint8_t
    {
        Available,
        InUse,
        Corrupted
    };
    enum class D71ItemType : uint8_t
    {
        File,
        Directory
    };
    struct D71Item
    {
        uint32_t      ID;
        D71ItemStatus Status;
        D71ItemType   Type;
        uint8_t       FileType;
        std::string   Name;
        uint32_t      Size;
        uint32_t      StartTrack;
        uint32_t      StartSector;
    };
    struct D71Info
    {
        std::string          DiskName;
        std::string          DiskTitle;
        uint32_t             nTotalFiles;
        std::vector<D71Item> DiskItems;
        D71Info() : DiskName( "NBDisk" ), DiskTitle( "NBDisk.d71" ), nTotalFiles( 0 )
        {
            DiskItems.clear();
        }
    };
    D71Info DiskContents;

  private:
    static constexpr uint32_t         DiskSignatureSize = 8;
    static constexpr uint32_t         TrackCount        = 70;
    static constexpr uint32_t         BytesPerTrack     = 256;
    static constexpr uint8_t          FileNameLength    = 16;
    static constexpr uint8_t          FileNamePadding   = 0xA0;
    std::vector<uint32_t>             trackOffsets;
    std::vector<std::vector<uint8_t>> trackData;

  public:
    DiskD71();
    virtual ~DiskD71() = default;
    bool open( const std::string& filename ) override;
    void close() override;
    bool readDirectory() override;
    bool parseHeader();
    bool loadTracks();
};
