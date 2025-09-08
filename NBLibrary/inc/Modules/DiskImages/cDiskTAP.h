/**
    @file       cDiskTAP.h
    @brief      Class for tape image type TAP (Commodore 64 tape image).
    @defgroup   NBLibraryFileHandling Disk Image Handling
    @copyright  Copyright (c) 2025, Neil Beresford
*/

#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include "cDiskCore.h"

class DiskTAP : protected DiskCore
{
  public:
    enum class TAPItemStatus : uint8_t
    {
        Available,
        InUse,
        Corrupted
    };
    enum class TAPItemType : uint8_t
    {
        File,
        Directory
    };
    struct TAPItem
    {
        uint32_t      ID;
        TAPItemStatus Status;
        TAPItemType   Type;
        uint8_t       FileType;
        std::string   Name;
        uint32_t      Size;
        uint32_t      StartTrack;
        uint32_t      StartSector;
    };
    struct TAPInfo
    {
        std::string          DiskName;
        std::string          DiskTitle;
        uint32_t             nTotalFiles;
        std::vector<TAPItem> DiskItems;
        TAPInfo() : DiskName( "NBDisk" ), DiskTitle( "NBDisk.tap" ), nTotalFiles( 0 )
        {
            DiskItems.clear();
        }
    };
    TAPInfo DiskContents;

  private:
    static constexpr uint32_t         FileNameLength  = 16;
    static constexpr uint8_t          FileNamePadding = 0xA0;
    std::vector<std::vector<uint8_t>> fileEntries;

  public:
    DiskTAP();
    virtual ~DiskTAP() = default;
    bool open( const std::string& filename ) override;
    void close() override;
    bool readDirectory() override;
    bool parseHeader();
    bool loadEntries();
};
