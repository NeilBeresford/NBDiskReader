#include "../../../inc/Modules/DiskImages/cDiskTAP.h"
#include <fstream>
#include <cstring>

DiskTAP::DiskTAP() : DiskCore()
{
}

bool DiskTAP::open( const std::string& filename )
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

void DiskTAP::close()
{
    if ( DiskImageStream.is_open() )
        DiskImageStream.close();
    fileEntries.clear();
}

bool DiskTAP::readDirectory()
{
    // TAP directory is not standardized; this is a stub for future implementation
    DiskContents             = TAPInfo();
    DiskContents.nTotalFiles = 0;
    // Real implementation would parse tape blocks and extract file info
    return true;
}

bool DiskTAP::parseHeader()
{
    return true;
}
bool DiskTAP::loadEntries()
{
    return true;
}
