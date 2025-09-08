/**
    @file       cDiskT64.h
    @brief      Class for tape image type T64 (Commodore 64 tape image).
    @defgroup   NBLibraryFileHandling Disk Image Handling
    @copyright  Copyright (c) 2025, Neil Beresford
*/

#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include "cDiskCore.h"

class DiskT64 : protected DiskCore
{
  public:
    enum class T64ItemStatus : uint8_t
    {
        Available,
        InUse,
        Corrupted
    };
    enum class T64ItemType : uint8_t
    {
        File,
        Directory
    };
    struct T64Item
    {
        uint32_t      ID;
        T64ItemStatus Status;
        T64ItemType   Type;
        uint8_t       FileType;
        std::string   Name;
        uint32_t      Size;
        uint32_t      StartTrack;
        uint32_t      StartSector;
    };
    struct T64Info
    {
        std::string          DiskName;
        std::string          DiskTitle;
        uint32_t             nTotalFiles;
        std::vector<T64Item> DiskItems;
        T64Info() : DiskName( "NBDisk" ), DiskTitle( "NBDisk.t64" ), nTotalFiles( 0 )
        {
            DiskItems.clear();
        }
    };
    T64Info DiskContents;

  private:
    static constexpr uint32_t         FileNameLength  = 16;
    static constexpr uint8_t          FileNamePadding = 0xA0;
    std::vector<std::vector<uint8_t>> fileEntries;

  public:
    DiskT64();
    virtual ~DiskT64() = default;
    bool open( const std::string& filename ) override;
    void close() override;
    bool readDirectory() override;
    bool parseHeader();
    bool loadEntries();
};
