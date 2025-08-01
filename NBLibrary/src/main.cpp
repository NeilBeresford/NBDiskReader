/**
    @file       main.cpp
    @defgroup   ModernCppTests Modern C++ Tests
    @brief      Main file for running tests using the doctest framework.
    @copyright   Copyright (c) 2025, Neil Beresford
*/

//-----------------------------------------------------------------------------
// Defines
// ----------------------------------------------------------------------------

#define TEST 0 //!< Set to 1 to run the test with a hardcoded D64 file, or 0 to use command line arguments

//-----------------------------------------------------------------------------
// Libraries
// ----------------------------------------------------------------------------

#include <fstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <cstdint>
#include <format>

//-----------------------------------------------------------------------------
//  External Functionality
// ----------------------------------------------------------------------------

// Constants for D64 format
constexpr uint32_t SECTOR_SIZE      = 256;         //!< Size of a D64 sector in bytes
constexpr uint32_t SECTOR_DATA_SIZE = SECTOR_SIZE; //!< 256 bytes Size of the data in each sector
constexpr uint32_t BAM_TRACK        = 18;          //!< Track location of the BAM (Block Allocation Map) on a D64 disk
constexpr uint32_t BAM_SECTOR       = 0;           //!< Sector location of the BAM on a D64 disk

// Sectors per track for 1541 disk
constexpr uint32_t sectors_per_track[ 35 ] = {
    21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, // 1-17
    19, 19, 19, 19, 19, 19, 19,                                         // 18-24
    18, 18, 18, 18, 18, 18,                                             // 25-30
    17, 17, 17, 17, 17                                                  // 31-35
};

struct DiskFileType
{
    uint8_t  file_type; //!< File type (0x00 = DEL, 0x01 = SEQ, 0x02 = PRG, 0x03 = USR, 0x04 = REL etc)
    uint8_t* name;      //!< Pointer to the file type name, null-terminated

} _DiskFileType;

/**

HEX      File type                 Directory shows

---      ---------                 ----------------------
$00      Scratched                 Does not show
$01      Unclosed sequential       *SEQ
$02      Unclosed program          *PRG
$03      Unclosed user             *USR
$04      Unclosed relative         Cannot occur
$80      Deleted                   DEL
$81      Sequential                SEQ
$82      Program                   PRG
$83      User                      USR
$84      Relative                  REL
$A0      Deleted @ replacement     DEL
$A1      Sequential @ replacement  SEQ
$A2      Program @ replacement     PRG
$A3      User @ replacement        USR
$A4      Relative @ replacement    Cannot occur
$C0      Locked deleted            DEL<
$C1      Locked sequential         SEQ<
$C2      Locked program            PRG<
$C3      Locked user               USR<
$C4      Locked relative           REL<

*/

DiskFileType disk_file_types[ 20 ] = {

    {0x00, (uint8_t*)"SCRATCH"}, //!< Deleted file
    {0x01, (uint8_t*)"*SEQ"   }, //!< Sequential file
    {0x02, (uint8_t*)"*PRG"   }, //!< Program file
    {0x03, (uint8_t*)"*USR"   }, //!< User file
    {0x04, (uint8_t*)"ERROR"  }, //!< unclosed reative cannot accure
    {0x80, (uint8_t*)"DEL"    }, //!< Deleted file
    {0x81, (uint8_t*)"SEQ"    }, //!< Sequential file
    {0x82, (uint8_t*)"PRG"    }, //!< Program file
    {0x83, (uint8_t*)"USR"    }, //!< User file
    {0x84, (uint8_t*)"REL"    }, //!< Relative file
    {0xA0, (uint8_t*)"DEL"    }, //!< Deleted file @ replacement
    {0xA1, (uint8_t*)"SEQ"    }, //!< Sequential file @ replacement
    {0xA2, (uint8_t*)"PRG"    }, //!< Program file @ replacement
    {0xA3, (uint8_t*)"USR"    }, //!< User file @ replacement
    {0xA4, (uint8_t*)"REL"    }, //!< Relative file @ replacement
    {0xC0, (uint8_t*)"DEL<"   }, //!< Locked deleted file
    {0xC1, (uint8_t*)"SEQ<"   }, //!< Locked sequential file
    {0xC2, (uint8_t*)"PRG<"   }, //!< Locked program file
    {0xC3, (uint8_t*)"USR<"   }, //!< Locked user file
    {0xC4, (uint8_t*)"REL<"   }  //!< Locked relative file
};

// Calculate the offset in the file for a given track and sector
size_t d64_offset( uint32_t track, uint32_t sector )
{
    size_t offset = 0;
    for ( uint32_t t = 0; t < track - 1; ++t ) offset += sectors_per_track[ t ];
    offset += sector;
    return ( offset * SECTOR_SIZE );
}

void read_d64_directory( const std::string& filename )
{
    std::ifstream d64( filename, std::ios::binary );
    if ( !d64 )
    {
        std::cerr << "Failed to open D64 file.\n";
        return;
    }

    uint32_t track  = BAM_TRACK;
    uint32_t sector = BAM_SECTOR;

    std::cout << "Disk File Name: " << filename << " opened\n";

    while ( track != 0 )
    {
        size_t offset = d64_offset( track, sector );
        d64.seekg( offset );
        std::vector<uint8_t> sector_data( SECTOR_SIZE );
        d64.read( reinterpret_cast<char*>( sector_data.data() ), SECTOR_SIZE );

        if ( track == BAM_TRACK && sector == BAM_SECTOR )
        {

            // The name of the disk, plus the sectors that have been used...
            std::string disk_name;
            for ( uint32_t i = 0; i < 16; ++i )
            {
                uint8_t c = sector_data[ 144 + i ];
                if ( c == 0xA0 )
                    break; // End of name
                disk_name += static_cast<char>( c );
            }
            std::cout << "Disk Name: " << disk_name << "\n";
            std::cout << "Disk usage per track (35)\n";

            // now displaythe disk sector usage...
            // 140*8 sectors = 1120 sectors for a full disk
            for ( uint32_t bamByte = 0; bamByte <= 34; bamByte++ )
            {
                if ( bamByte % 7 == 0 && bamByte != 0 )
                    std::cout << "\n";
                std::cout << "" << std::format( "{:02X}", sector_data[ ( bamByte << 2 ) + 4 + 1 ] );
                std::cout << "" << std::format( "{:02X}", sector_data[ ( bamByte << 2 ) + 4 + 2 ] );
                std::cout << "" << std::format( "{:02X} ", sector_data[ ( bamByte << 2 ) + 4 + 3 ] );
            }
            std::cout << "\n";
        }
        // Next track/sector for directory
        track  = sector_data[ 0 ];
        sector = sector_data[ 1 ];

        offset = d64_offset( track, sector );
        d64.seekg( offset );
        d64.read( reinterpret_cast<char*>( sector_data.data() ), SECTOR_SIZE );

        // 8 entries per sector, 32 bytes each, starting at offset 2 (30 bytes directory entry with 2 bytes padding)
        for ( uint32_t i = 0; i < 8; ++i )
        {
            uint32_t entry_offset = i * 32;
            uint8_t  file_type    = sector_data[ entry_offset + 2 ];
            if ( file_type == 0x00 )
                continue; // Unused entry

            // File name: 16 bytes, padded with 0xA0
            std::string name;
            for ( uint32_t j = 0; j < 16; ++j )
            {
                uint8_t c = sector_data[ entry_offset + 5 + j ];
                if ( c == 0xA0 )
                    break;
                name += static_cast<char>( c );
            }

            // get the type of file
            std::string type = "ERROR";
            for ( const auto& file_type_entry : disk_file_types )
            {
                if ( file_type_entry.file_type == file_type )
                {
                    type = std::string( reinterpret_cast<const char*>( file_type_entry.name ) );
                    break;
                }
            }

            // File size in sectors (little endian)
            uint32_t size_lo = sector_data[ entry_offset + 30 ];
            uint32_t size_hi = sector_data[ entry_offset + 31 ];
            uint32_t size    = size_lo + ( size_hi << 8 );

            std::cout << std::setw( 18 ) << std::left << name << std::setw( 7 ) << type << size << " blocks\n";
        }
    }
}

uint32_t main( uint32_t argc, char** argv )
{
#if TEST == 1
    read_d64_directory( "test.d64" );

#else
    if ( argc != 2 || ( argc >= 2 && argv[ 1 ] == "/?" ) )
    {
        std::cout << "D64 Directory Reader\n";
        std::cout << "Usage: " << argv[ 0 ] << " <d64_file>\n";
        return 1;
    }
    read_d64_directory( argv[ 1 ] );
#endif
    return 0;
}
//-----------------------------------------------------------------------------
// End of main.cpp
// ----------------------------------------------------------------------------
