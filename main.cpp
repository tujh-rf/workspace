#include <cstddef>
#include <cstdarg>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <sys/ioctl.h>

struct Settings
{
    struct Baudrates
    {
        static const size_t b0      = 0;
        static const size_t b50     = 50;
        static const size_t b75     = 75;
        static const size_t b110    = 110;
        static const size_t b134    = 134;
        static const size_t b150    = 150;
        static const size_t b200    = 200;
        static const size_t b300    = 300;
        static const size_t b600    = 600;
        static const size_t b1200   = 1200;
        static const size_t b1800   = 1800;
        static const size_t b2400   = 2400;
        static const size_t b4800   = 4800;
        static const size_t b9600   = 9600;
        static const size_t b19200  = 19200;
        static const size_t b38400  = 38400;
        static const size_t b57600  = 57600;
        static const size_t b115200 = 115200;
    };
    struct DataBits
    {
        static const size_t db5 = 5;
        static const size_t db6 = 6;
        static const size_t db7 = 7;
        static const size_t db8 = 8;
    };
    struct StopBits
    {
        static const size_t sb1     = 0;
        static const size_t sb2     = 1;
    };
    struct Parities
    {
        static const size_t pNone = 0;
        static const size_t pOdd  = 1;
        static const size_t pEven = 2;
    };
    struct FlowControls
    {
        static const size_t fcNone  = 0;
        static const size_t fcSoft  = 1;
        static const size_t fcHard  = 2;
        static const size_t fcPS485 = 3;
    };

    std::string strPortName;
    size_t      szBaudrate;
    size_t      szDataBits;
    size_t      szStopBits;
    size_t      szParity;
    size_t      szFlowControl;

    Settings():
        strPortName( "/dev/ttyS1" )
      , szBaudrate( Settings::Baudrates::b9600 )
      , szDataBits( Settings::DataBits::db8 )
      , szStopBits( Settings::StopBits::sb1 )
      , szParity( Settings::Parities::pNone )
      , szFlowControl( Settings::FlowControls::fcNone )
    {
    }

};

Settings ReadSetting( const char *pname )
{
    Settings Result;

    FILE *pf = fopen( pname, "r" );
    if ( pf != NULL )
    {
        char buffer[512] = { 0 };
        char skey[256] = { 0 }, svalue[256] = { 0 };
        char *s = NULL;
        std::cout << "read settings..." << std::endl;

        while ( ( s = fgets( buffer, 500, pf ) ) != NULL )
        {
            if ( ( buffer[0] == '\n' ) || ( buffer[0] == '#' ) )
                continue;

            s = strtok( buffer, "=" );
            if ( s == NULL )
                continue;

            strncpy( skey, s, 255 );

            s = strtok( NULL, "=" );
            if ( s == NULL )
                continue;

            strncpy( svalue, s, 255 );

            // port name
            if ( strcmp( skey, "port" ) == 0 )
            {
                size_t l = strlen( svalue );
                if ( iscntrl( svalue[l - 1] ) )
                    svalue[l - 1] = '\x0';
                Result.strPortName = svalue;
                std::cout << "find port: " << Result.strPortName.c_str() << std::endl;
            }
            // baudrate
            if ( strcmp( skey, "baud" ) == 0 )
            {
                Result.szBaudrate = static_cast< size_t > ( atoi( svalue ) );
                std::cout << "find baudrate: " << Result.szBaudrate << std::endl;
            }
            // parity
            if ( strcmp( skey, "parity" ) == 0 )
            {
                std::cout << "find parity: ";
                if ( strcmp( svalue, "none" ) == 0 )
                    Result.szParity = Settings::Parities::pNone;
                if ( strcmp( svalue, "even" ) == 0 )
                    Result.szParity = Settings::Parities::pEven;
                if ( strcmp( svalue, "odd" ) == 0 )
                    Result.szParity = Settings::Parities::pOdd;
                switch ( Result.szParity )
                {
                case Settings::Parities::pNone:
                    std::cout << "none";
                    break;
                case Settings::Parities::pEven:
                    std::cout << "even";
                    break;
                case Settings::Parities::pOdd:
                    std::cout << "odd";
                    break;
                default:
                    std::cout << "<error>";
                    break;
                }
                std::cout << std::endl;
            }
            // databits
            if ( strcmp( skey, "databits" ) == 0 )
            {
                Result.szDataBits = static_cast< size_t > ( atoi( svalue ) );
                std::cout << "find databits setting: " << Result.szDataBits << std::endl;
            }
            // stopbits
            if ( strcmp( skey, "stopbits" ) == 0 )
            {
                std::cout << "find stopbits setting: ";
                if ( strcmp( svalue, "1" ) == 0 )
                    Result.szStopBits = Settings::StopBits::sb1;
                if ( strcmp( svalue, "2" ) == 0 )
                    Result.szStopBits = Settings::StopBits::sb2;
                switch ( Result.szStopBits )
                {
                case Settings::StopBits::sb1:
                    std::cout << "1";
                    break;
                case Settings::StopBits::sb2:
                    std::cout << "2";
                    break;
                default:
                    std::cout << "<error>";
                    break;
                }
                std::cout << std::endl;
            }
            // flow
            if ( strcmp( skey, "flow" ) == 0 )
            {
                std::cout << "find flow control settings: ";
                if ( strcmp( svalue, "none" ) == 0 )
                    Result.szFlowControl = Settings::FlowControls::fcNone;
                if ( strcmp( svalue, "soft" ) == 0 )
                    Result.szFlowControl = Settings::FlowControls::fcSoft;
                if ( strcmp( svalue, "hard" ) == 0 )
                    Result.szFlowControl = Settings::FlowControls::fcHard;
                if ( strcmp( svalue, "ps485" ) == 0 )
                    Result.szFlowControl = Settings::FlowControls::fcPS485;
                switch ( Result.szFlowControl )
                {
                case Settings::FlowControls::fcNone:
                    std::cout << "none";
                    break;
                case Settings::FlowControls::fcSoft:
                    std::cout << "software";
                    break;
                case Settings::FlowControls::fcHard:
                    std::cout << "hardware";
                    break;
                case Settings::FlowControls::fcPS485:
                    std::cout << "prosoft-systems rs-485";
                    break;
                default:
                    std::cout << "<error>";
                    break;
                    std::cout << std::endl;
                }
                std::cout << std::endl;
            }
        }

        fclose( pf );
    }
    else
        std::cout
            << "[error] file 'settings.dat' not found, use default settings:"
            << std::endl
            << "\t /dev/ttyS1 9600 8N1 flow=none"
            << std::endl;

    return Result;
}

int OpenPort( const Settings &sett )
{
    int Result = -1;

    std::cout << "try open port " << sett.strPortName.c_str() << std::endl;

    Result = open( sett.strPortName.c_str(), O_RDWR | O_NOCTTY | O_NDELAY );
    if ( Result != -1 )
    {
        std::cout << "port open" << std::endl;

        termios options;
        int value = 0;

        tcgetattr( Result, &options );

        options.c_cflag |= ( CLOCAL | CREAD );

        // baudrate
        switch ( sett.szBaudrate )
        {
        case Settings::Baudrates::b0:
            std::cout << "switch baudrate to: 0" << std::endl;
            value = B0;
            break;
        case Settings::Baudrates::b50:
            std::cout << "switch baudrate to: 50" << std::endl;
            value = B50;
            break;
        case Settings::Baudrates::b75:
            std::cout << "switch baudrate to: 75" << std::endl;
            value = B75;
            break;
        case Settings::Baudrates::b110:
            std::cout << "switch baudrate to: 110" << std::endl;
            value = B110;
            break;
        case Settings::Baudrates::b134:
            std::cout << "switch baudrate to: 134" << std::endl;
            value = B134;
            break;
        case Settings::Baudrates::b150:
            std::cout << "switch baudrate to: 150" << std::endl;
            value = B150;
            break;
        case Settings::Baudrates::b200:
            std::cout << "switch baudrate to: 200" << std::endl;
            value = B200;
            break;
        case Settings::Baudrates::b300:
            std::cout << "switch baudrate to: 300" << std::endl;
            value = B300;
            break;
        case Settings::Baudrates::b600:
            std::cout << "switch baudrate to: 600" << std::endl;
            value = B600;
            break;
        case Settings::Baudrates::b1200:
            std::cout << "switch baudrate to: 1200" << std::endl;
            value = B1200;
            break;
        case Settings::Baudrates::b1800:
            std::cout << "switch baudrate to: 1800" << std::endl;
            value = B1800;
            break;
        case Settings::Baudrates::b2400:
            std::cout << "switch baudrate to: 2400" << std::endl;
            value = B2400;
            break;
        case Settings::Baudrates::b4800:
            std::cout << "switch baudrate to: 4800" << std::endl;
            value = B4800;
            break;
        case Settings::Baudrates::b9600:
            std::cout << "switch baudrate to: 9600" << std::endl;
            value = B9600;
            break;
        case Settings::Baudrates::b19200:
            std::cout << "switch baudrate to: 19200" << std::endl;
            value = B19200;
            break;
        case Settings::Baudrates::b38400:
            std::cout << "switch baudrate to: 38400" << std::endl;
            value = B38400;
            break;
        case Settings::Baudrates::b57600:
            std::cout << "switch baudrate to: 57600" << std::endl;
            value = B57600;
            break;
        case Settings::Baudrates::b115200:
            std::cout << "switch baudrate to: 115200" << std::endl;
            value = B115200;
            break;
        }
        cfsetispeed( &options, value );
        cfsetospeed( &options, value );
        value = 0;

        // parity
        switch ( sett.szParity )
        {
        case Settings::Parities::pEven:
            std::cout << "switch parity to: 'even'" << std::endl;
            options.c_cflag |= PARENB;
            options.c_cflag &= ~PARODD;
            options.c_iflag |= ( INPCK | ISTRIP );
            break;
        case Settings::Parities::pOdd:
            std::cout << "switch parity to: 'odd'" << std::endl;
            options.c_cflag |= PARENB;
            options.c_cflag |= PARODD;
            options.c_iflag |= ( INPCK | ISTRIP );
            break;
        case Settings::Parities::pNone:
            std::cout << "switch parity to: 'none'" << std::endl;
            options.c_cflag &= ~PARENB;
            options.c_iflag &= ~( INPCK | ISTRIP );
            break;
        default:
            break;
        }

        // databits
        switch ( sett.szDataBits )
        {
        case Settings::DataBits::db5:
            std::cout << "switch databits to: 5" << std::endl;
            value = CS5;
            break;
        case Settings::DataBits::db6:
            std::cout << "switch databits to: 6" << std::endl;
            value = CS6;
            break;
        case Settings::DataBits::db7:
            std::cout << "switch databits to: 7" << std::endl;
            value = CS7;
            break;
        case Settings::DataBits::db8:
            std::cout << "switch databits to: 8" << std::endl;
            value = CS8;
            break;
        }
        options.c_cflag &= ~CSIZE;
        options.c_cflag |= value;
        value = 0;

        // stopbits
        switch ( sett.szStopBits )
        {
        case Settings::StopBits::sb1:
            std::cout << "switch stopbits to: 1" << std::endl;
            options.c_cflag &= ~CSTOPB;
            break;
        case Settings::StopBits::sb2:
            std::cout << "switch stopbits to: 2" << std::endl;
            options.c_cflag |= CSTOPB;
            break;
        default:
            break;
        }

        // flow control
        switch ( sett.szFlowControl )
        {
        case Settings::FlowControls::fcNone:
        case Settings::FlowControls::fcPS485:
            options.c_cflag &= ~CRTSCTS;
            options.c_iflag &= ~( IXON | IXOFF | IXANY );
            break;
        case Settings::FlowControls::fcSoft:
            options.c_iflag |= ( IXON | IXOFF | IXANY );
            break;
        case Settings::FlowControls::fcHard:
            options.c_cflag |= CRTSCTS;
            options.c_iflag &= ~( IXON | IXOFF | IXANY );
            break;
        }

        // port settings
        options.c_lflag &=  ~( ICANON | ECHO | ECHOE | ISIG );
        options.c_oflag &= ~OPOST;

        tcsetattr( Result, TCSANOW, &options );

    }
    else
    {
        std::cout << "error to open: " << errno << std::endl;
    }

    return Result;
}

int main()
{
    Settings settings = ReadSetting( "settings.dat" );

    int fdport;
    if ( ( fdport = OpenPort( settings ) ) != -1 )
    {
        std::cout << "port opened and configures" << std::endl;

        FILE *pfile = fopen( "message.dat", "rb" );
        if ( pfile != NULL )
        {
            size_t flength = 0;
            int result = 0, status = 0;

            fseek( pfile, 0, SEEK_END );
            flength = ftell( pfile );
            rewind( pfile );

            if ( flength )
            {
                char *buffer = new char[flength];

                result = fread( buffer, sizeof( char ), flength, pfile );
                if ( static_cast< size_t > ( result ) == flength )
                {
                    std::cout << "write " << flength << " bytes to port" << std::endl;

                    // set RTS to prosoft-485
                    if ( settings.szFlowControl == Settings::FlowControls::fcPS485 )
                    {
                        ioctl( fdport, TIOCMGET, &status );
                        status |= TIOCM_RTS;
                        ioctl( fdport, TIOCMSET, &status );
                    }

                    result = write( fdport, buffer, flength );

                    // drop RTS to prosoft-485
                    if ( settings.szFlowControl == Settings::FlowControls::fcPS485 )
                    {
                        if ( static_cast< size_t > ( result ) == flength )
                        {
                            int outq = 0;
                            do
                            {
                                ioctl( fdport, TIOCOUTQ, &outq );
                            } while( outq != 0 );
                        }

                        ioctl( fdport, TIOCMGET, &status );
                        status &= ~TIOCM_RTS;
                        ioctl( fdport, TIOCMSET, &status );
                    }

                    usleep( 1000000 );

                    ioctl( fdport, FIONREAD, &result );
                    if ( result )
                    {
                        flength = static_cast< size_t > ( result );
                        std::cout << "input buffer has " << flength << " bytes" << std::endl;

                        delete[] buffer;
                        buffer = new char[flength];

                        result = read( fdport, buffer, flength );
                        if ( static_cast< size_t > ( result ) == flength )
                        {
                            FILE *pf = fopen( "result.dat", "wb" );
                            if ( pf != NULL )
                            {
                                fwrite( buffer, sizeof( char ), flength, pf );

                                fclose( pf );
                            }
                        }
                    }
                }

                delete[] buffer;
            }

            fclose( pfile );
        }
        else
            std::cout << "cannot open 'message.dat'" << std::endl;

        std::cout << "close port" << std::endl;
        close( fdport );
    }

    return 0;
}

