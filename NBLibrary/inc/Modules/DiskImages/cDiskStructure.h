/**
    @file       cDiskStructure.h
    @brief      Disk structre,, header position, disk size etc.
    @defgroup   NBDiskReader Disk Image Reader
    @copyright  Copyright (c) 2025, Neil Beresford
*/

#pragma once

//-----------------------------------------------------------------------------
// Libraries
// ----------------------------------------------------------------------------

#include <string>
#include <cstdint>
#include <vector>
#include <fstream>
#include <iostream>

//-----------------------------------------------------------------------------
// Classes & Structures
// ----------------------------------------------------------------------------

/**
    @ingroup    NBDiskReader
    @brief      Structure to hold disk descriptor information.
*/
struct DiskDescriptor
{
    // Members
    uint32_t             DiskSize;         ///< Size of the disk image in bytes
    uint32_t             TotalTracks;      ///< Total number of tracks on the disk
    uint32_t             BytesPerSector;   ///< Number of bytes per sector
    uint32_t             TotalSectors;     ///< Total number of sectors on the disk
    std::vector<uint8_t> TrackSectorTable; ///< Vector containing the sector number for each track
    uint16_t             BAMTrack;         ///< Block Allocation Map track
    uint16_t             BAMSector;        ///< Block Allocation Map sector

    // Initialization
    DiskDescriptor() : DiskSize( 0 ), TotalTracks( 0 ), TrackSectorTable(), BAMTrack( 0 ), BAMSector( 0 )
    {
    }

    DiskDescriptor( const DiskDescriptor& )            = default;
    DiskDescriptor& operator=( const DiskDescriptor& ) = default;

    /**
        @brief      Setup the disk descriptor.
        @param      uint32_t    diskSize          Size of the disk image in bytes.
        @param      uint32_t    totalTracks       Total number of tracks on the disk.
        @param      const std::vector<uint8_t>& trackSectorTable  Track sector table.
        @param      uint16_t    bamTrack         Block Allocation Map track.
        @param      uint16_t    bamSector        Block Allocation Map sector.
     */
    void SetupDescriptor( uint32_t diskSize, uint32_t totalTracks, const std::vector<uint8_t>& trackSectorTable, uint16_t bamTrack, uint16_t bamSector )
    {
        DiskSize         = diskSize;
        TotalTracks      = totalTracks;
        TrackSectorTable = trackSectorTable;
        BAMTrack         = bamTrack;
        BAMSector        = bamSector;
    }

    // Getters
    /**
        @ingroup    NBDiskReader
        @brief      Get the size of the disk image.
        @return     uint32_t    Size of the disk image in bytes.
    */
    uint32_t GetDiskSize() const
    {
        return DiskSize;
    }
    /**
        @ingroup    NBDiskReader
        @brief      Get the total number of tracks on the disk.
        @return     uint32_t    Total number of tracks on the disk.
    */
    uint32_t GetTotalTracks() const
    {
        return TotalTracks;
    }
    /**
        @ingroup    NBDiskReader
        @brief      Get the track sector table.
        @return     const std::vector<uint8_t>&    Track sector table.
    */
    const std::vector<uint8_t>& GetTrackSectorTable() const
    {
        return TrackSectorTable;
    }
    /**
        @ingroup    NBDiskReader
        @brief      Get the Block Allocation Map track.
        @return     uint16_t    Block Allocation Map track.
    */
    uint16_t GetBAMTrack() const
    {
        return BAMTrack;
    }
    /**
        @ingroup    NBDiskReader
        @brief      Get the Block Allocation Map sector.
        @return     uint16_t    Block Allocation Map sector.
    */
    uint16_t GetBAMSector() const
    {
        return BAMSector;
    }

    // Setters
    /**
        @ingroup    NBDiskReader
        @brief      Set the size of the disk image.
        @param      uint32_t    size    Size of the disk image in bytes.
    */
    void SetDiskSize( uint32_t size )
    {
        DiskSize = size;
    }

    /**
        @ingroup    NBDiskReader
        @brief      Set the total number of tracks on the disk.
        @param      uint32_t    tracks    Total number of tracks on the disk.
    */
    void SetTotalTracks( uint32_t tracks )
    {
        TotalTracks = tracks;
    }

    /**
        @ingroup    NBDiskReader
        @brief      Set the track sector table.
        @param      const std::vector<uint8_t>&    table    Track sector table.
    */
    void SetTrackSectorTable( const std::vector<uint8_t>& table )
    {
        TrackSectorTable = table;
    }

    /**
        @ingroup    NBDiskReader
        @brief      Set the Block Allocation Map track.
        @param      uint16_t    track    Block Allocation Map track.
    */
    void SetBAMTrack( uint16_t track )
    {
        BAMTrack = track;
    }

    /**
        @ingroup    NBDiskReader
        @brief      Set the Block Allocation Map sector.
        @param      uint16_t    sector    Block Allocation Map sector.
    */
    void SetBAMSector( uint16_t sector )
    {
        BAMSector = sector;
    }
};

//-----------------------------------------------------------------------------
// End of file cDiskStructure.h
// ----------------------------------------------------------------------------
