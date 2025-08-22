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

//-----------------------------------------------------------------------------
// Classes & Structures
// ----------------------------------------------------------------------------

struct DSK_HEADER
{
    uint8_t m_nHeaderSize;
    uint8_t m_nHeaderVersion;
    uint8_t m_nReserved[ 6 ];
};

//-----------------------------------------------------------------------------
// End of file cDiskStructure.h
// ----------------------------------------------------------------------------
