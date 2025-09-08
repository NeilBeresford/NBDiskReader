/**
    @file       cDiskG64.h
    @brief      Class for disk type G64 (Commodore 1541 GCR disk image).
    @defgroup   NBLibraryFileHandling Disk Image Handling
    @copyright  Copyright (c) 2025, Neil Beresford
*/

#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include "cDiskCore.h"

//-----------------------------------------------------------------------------
// Classes
// ----------------------------------------------------------------------------

/**
    @ingroup    NBLibraryFileHandling
    @brief      Class for disk type G64 (Commodore 1541 GCR disk image).
*/
class DiskG64 : protected DiskCore
{
  public:
    // G64-specific constants
    static constexpr uint32_t DiskSize       = ( 84 * 7925 ); // 84 tracks * 7925 bytes (max) per track
    static constexpr uint16_t TotalTracks    = 84;
    static constexpr uint16_t TotalSectors   = 84;
    static constexpr uint16_t BytesPerSector = 7925;
    static constexpr uint8_t  BAMTrack       = 18; // G64 BAM is on track 18
    static constexpr uint8_t  BAMSector      = 1;

    // G64 directory item status/type enums (mirroring D64)
    enum class G64ItemStatus : uint8_t
    {
        Available,
        InUse,
        Corrupted
    };

    enum class G64ItemType : uint8_t
    {
        File,
        Directory
    };

    struct G64Item
    {
        uint32_t      ID;          ///< Unique identifier for the disk item
        G64ItemStatus Status;      ///< Status of the disk item (available, in use, corrupted)
        G64ItemType   Type;        ///< Type of the disk item (file or directory)
        uint8_t       FileType;    ///< File type of the disk item (raw byte)
        std::string   Name;        ///< Name of the disk item
        uint32_t      Size;        ///< Size of the disk item in bytes
        uint32_t      StartTrack;  ///< Starting track of the disk item
        uint32_t      StartSector; ///< Starting sector of the disk item
    };

    struct G64Info
    {
        std::string          DiskName;    ///< Name of the disk
        std::string          DiskTitle;   ///< Title of the disk
        uint32_t             nTotalFiles; ///< Total number of files on the disk
        std::vector<G64Item> DiskItems;   ///< List of items on the disk

        G64Info()
        {
            DiskName    = "NBDisk";
            DiskTitle   = "NBDisk.g64";
            nTotalFiles = 0;
            DiskItems.clear();
        }
    };

    G64Info DiskContents; ///< Information and contents of disk
  private:
    // G64-specific constants
    static constexpr uint32_t DiskSignatureSize = 8;    //!< Size of the G64 signature
    static constexpr uint32_t TrackCount        = 84;   //!< Maximum number of tracks in G64
    static constexpr uint32_t BytesPerTrack     = 7928; //!< Maximum bytes per track (varies)
    static constexpr uint8_t  FileNameLength    = 16;   //!< File name length (for directory emulation)
    static constexpr uint8_t  FileNamePadding   = 0xA0; //!< Padding byte for file name

    // G64 header structure
    struct G64Header
    {
        char    signature[ 8 ]; //!< 'GCR-1541' signature
        uint8_t version;        //!< G64 version
        uint8_t tracks;         //!< Number of tracks
        uint8_t sides;          //!< Number of sides (usually 1)
        uint8_t reserved;       //!< Reserved
    };

    // Track offsets and data
    std::vector<uint32_t>             trackOffsets; //!< Offsets to each track's data
    std::vector<std::vector<uint8_t>> trackData;    //!< Raw track data

  public:
    DiskG64();
    virtual ~DiskG64() = default;

    // Core functions
    bool open( const std::string& filename ) override;
    void close() override;
    bool readDirectory() override;

    // G64-specific helpers
    bool parseHeader();
    bool loadTracks();
};

//-----------------------------------------------------------------------------
