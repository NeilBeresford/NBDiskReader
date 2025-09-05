
#include <windows.h>
#include <iostream>
#include <thread>
#include <vector>
#include <string>

void drawFrame( HANDLE hBuffer, int frame )
{
    // Query the current buffer size
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if ( !GetConsoleScreenBufferInfo( hBuffer, &csbi ) )
    {
        return;
    }
    SHORT width  = csbi.dwSize.X;
    SHORT height = csbi.dwSize.Y;
    if ( width < 2 || height < 2 )
        return;

    std::vector<CHAR_INFO> buffer( width * height );

    // Fill with spaces and blue background
    for ( int i = 0; i < width * height; ++i )
    {
        buffer[ i ].Char.AsciiChar = ' ';
        buffer[ i ].Attributes     = BACKGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
    }

    // Draw a white border
    for ( int x = 0; x < width; ++x )
    {
        buffer[ x ].Char.AsciiChar                          = '-';
        buffer[ x ].Attributes                              = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
        buffer[ x + width * ( height - 1 ) ].Char.AsciiChar = '-';
        buffer[ x + width * ( height - 1 ) ].Attributes     = buffer[ x ].Attributes;
    }
    for ( int y = 0; y < height; ++y )
    {
        buffer[ y * width ].Char.AsciiChar                 = '|';
        buffer[ y * width ].Attributes                     = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
        buffer[ y * width + ( width - 1 ) ].Char.AsciiChar = '|';
        buffer[ y * width + ( width - 1 ) ].Attributes     = buffer[ y * width ].Attributes;
    }

    // Write frame number in the middle
    std::string msg   = "Frame: " + std::to_string( frame );
    int         msg_x = ( width - (int)msg.size() ) / 2;
    int         msg_y = height / 2;
    if ( msg_x > 0 && msg_y > 0 && msg_x + msg.size() < (size_t)width )
    {
        for ( size_t i = 0; i < msg.size(); ++i )
        {
            buffer[ msg_y * width + msg_x + i ].Char.AsciiChar = msg[ i ];
            buffer[ msg_y * width + msg_x + i ].Attributes     = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
        }
    }

    SMALL_RECT rect     = { 0, 0, (SHORT)( width - 1 ), (SHORT)( height - 1 ) };
    COORD      bufSize  = { width, height };
    COORD      bufCoord = { 0, 0 };
    WriteConsoleOutputA( hBuffer, buffer.data(), bufSize, bufCoord, &rect );
}

int main()
{
    // Create two screen buffers
    HANDLE buffers[ 2 ];
    for ( int i = 0; i < 2; ++i )
    {
        buffers[ i ] = CreateConsoleScreenBuffer( GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL );
        // Set buffer size to match the current window size
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        HANDLE                     hStd = GetStdHandle( STD_OUTPUT_HANDLE );
        if ( GetConsoleScreenBufferInfo( hStd, &csbi ) )
        {
            SetConsoleScreenBufferSize( buffers[ i ], csbi.dwSize );
        }
    }

    int active = 0;
    for ( int frame = 0; frame < 100; ++frame )
    {
        int next = 1 - active;
        drawFrame( buffers[ next ], frame );
        SetConsoleActiveScreenBuffer( buffers[ next ] );
        active = next;
        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
    }

    // Restore default buffer before exit
    SetConsoleActiveScreenBuffer( GetStdHandle( STD_OUTPUT_HANDLE ) );
    return 0;
}
