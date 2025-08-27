#pragma once

#include <cstdint>
#include <string>

#include "cDiskCore.h"

//-----------------------------------------------------------------------------
// Classes
// ----------------------------------------------------------------------------

/**
    @ingroup    NBLibraryFileHandling
    @brief      Class for disk type D64.
*/
class DiskD64 : protected DiskCore
{
  private:
    // constants for descriptor
    static constexpr uint32_t DiskSize                        = 174336; // !< Disk size in bytes
    static constexpr uint32_t TotalTracks                     = 35;     // !< Total number of tracks
    static constexpr uint32_t TotalSectors                    = 681;    // !< Total number of sectors
    static constexpr uint32_t BytesPerSector                  = 256;    // !< Number of bytes per sector
    static constexpr uint16_t BAMTrack                        = 18;     // !< Track number of the BAM
    static constexpr uint16_t BAMSector                       = 1;      // !< Sector number of the BAM

    static constexpr uint8_t  TrackSectorTable[ TotalTracks ] = {
        21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, // 1-17  (357)
        19, 19, 19, 19, 19, 19, 19,                                         // 18-24 (133)
        18, 18, 18, 18, 18, 18,                                             // 25-30 (108)
        17, 17, 17, 17, 17                                                  // 31-35 (85)
                                                                            // Total (681)
    };

    // Internal enums and structures
    enum class D64ItemStatus : uint8_t
    {
        Available,
        InUse,
        Corrupted
    };

    enum class D64ItemType : uint8_t
    {
        File,
        Directory
    };

    enum class D64ItemFileType : uint8_t
    {
        Scratch        = 0x00, //!< Scratch file type
        Sequential     = 0x01, //!< Sequential file type
        Program        = 0x02, //!< Program file type
        User           = 0x03, //!< User file type
        Error          = 0x04, //!< Error file type
        Deleted        = 0x80, //!< Deleted file type
        DelSeq         = 0x81, //!< Deleted Sequential file type
        DelProg        = 0x82, //!< Deleted Program file type
        DelUser        = 0x83, //!< Deleted User file type
        Relative       = 0x84, //!< Relative file type
        DelRep         = 0xA0, //!< Deleted Replaced file type
        DelRepSeq      = 0xA1, //!< Deleted Replaced Sequential file type
        DelRepProg     = 0xA2, //!< Deleted Replaced Program file type
        DelRepUser     = 0xA3, //!< Deleted Replaced User file type
        RepRelative    = 0xA4, //!< Replaced Relative file type
        LockedDel      = 0xc0, //!< Locked Deleted file type
        LockedDelSeq   = 0xc1, //!< Locked Deleted Sequential file type
        LockedDelProg  = 0xc2, //!< Locked Deleted Program file type
        LockedDelUser  = 0xc3, //!< Locked Deleted User file type
        LockedRelative = 0xc4, //!< Locked Relative file type

        Total          = 20
    };

    struct D64Item
    {
        uint32_t        ID;          ///< Unique identifier for the disk item
        D64ItemStatus   Status;      ///< Status of the disk item (available, in use, corrupted)
        D64ItemType     Type;        ///< Type of the disk item (file or directory)
        D64ItemFileType FileType;    ///< File type of the disk item
        std::string     Name;        ///< Name of the disk item
        uint32_t        Size;        ///< Size of the disk item in bytes
        uint32_t        StartTrack;  ///< Starting track of the disk item
        uint32_t        StartSector; ///< Starting sector of the disk item
    };

    struct D64Info
    {
        std::string          DiskName;    ///< Name of the disk
        std::string          DiskTitle;   ///< Title of the disk
        uint32_t             nTotalFiles; ///< Total number of files on the disk
        std::vector<D64Item> DiskItems;   ///< List of items on the disk

        D64Info()
        {
            DiskName    = "NBDisk";
            DiskTitle   = "NBDisk.d64";
            nTotalFiles = 0;
            DiskItems.clear();
        }
    };

    // clang-format off
    //!< Strings for the disk item type (taken from D64)
    std::string D64ItemTypeStrings[ static_cast<size_t>( D64ItemFileType::Total ) ] =
    {
        "Scratch",     "Sequential", "Program",   "User",       "Error",      "Deleted",       "DelSeq",
        "DelProg",     "DelUser",    "Relative",  "DelRep",     "DelRepSeq",  "DelRepProg",    "DelRepUser",
        "RepRelative", "LockedDel",  "LockedSeq", "LockedProg", "LockedUser", "LockedRelative"
    };

    std::string D64ItemTypeStringShorts[ static_cast<size_t>( D64ItemFileType::Total ) ] =
    {
        "SCR",  "SEQ",  "PRG",  "USR",  "ERR",  "DEL",  "SEQ",  "PRG",  "USR",  "REL",
        "DEL>", "SEQ>", "PRG>", "USR>", "REL>", "DEL<", "SEQ<", "PRG<", "USR<", "REL<"
    };
    // clang-format on

    // Data members
    D64Info DiskContents; ///< Information and contents of disk

  public:
    DiskD64();
    virtual ~DiskD64() = default;

    // Prevent copying
    DiskD64( const DiskD64& )            = delete;
    DiskD64& operator=( const DiskD64& ) = delete;
    DiskD64( DiskD64&& )                 = delete;
    DiskD64& operator=( DiskD64&& )      = delete;

    // Initialization
    void ResetDiskContents();

    // Core functions
    virtual bool open( const std::string& filename ) override;
    virtual void close() override;
    virtual bool readDirectory() override;
};

//-----------------------------------------------------------------------------
// End of file: cDiskCore.h
// ----------------------------------------------------------------------------
