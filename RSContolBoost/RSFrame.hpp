#ifndef RSFRAME_HPP
#define RSFRAME_HPP

#include <boost/cstdint.hpp>


class RSFrame
{

public:

    struct Desc
    {        
        typedef boost::uint16_t id_type;

        boost::uint16_t ack;
        boost::uint16_t seq;
        boost::uint16_t package_id;
        boost::uint16_t number_of_packages;
        id_type dest_port_id;
        id_type port_id;
    };

    template < typename ForwardIterator >
    RSFrame( Desc desc, ForwardIterator first, ForwardIterator last )
    {
        assert( std::distance( first, last ) < max_data_size );

        buffer.reserve( std::distance( first, last ) + header_size );

        buffer.push_back( preambule );
        buffer.push_back( desc.dest_port_id >> 8 );
        buffer.push_back( desc.dest_port_id );
        buffer.push_back( desc.port_id >> 8 );
        buffer.push_back( desc.port_id );
        buffer.push_back( desc.seq >> 8 );
        buffer.push_back( desc.seq );
        buffer.push_back( desc.ack >> 8 );
        buffer.push_back( desc.ack );
        buffer.push_back( desc.package_id >> 8 );
        buffer.push_back( desc.package_id );
        buffer.push_back( desc.number_of_packages >> 8 );
        buffer.push_back( desc.number_of_packages );

        boost::uint8_t checksum_pos = buffer.size();

        buffer.push_back( 0 );
        buffer.push_back( 0 );
        
        buffer.push_back( std::distance( first, last ) );

        buffer.insert( buffer.end(), first, last );
        
        checksum_type ch = checksum();

        buffer[ checksum_pos ] = ch.first;
        buffer[ checksum_pos + 1 ] = ch.second;
    }

    boost::asio::const_buffer constBuffer() const
    {
        return boost::asio::const_buffer( &buffer.front(), buffer.size() );
    }

    static const unsigned max_data_size = 255;   

private:


    static const unsigned header_size = 16;   
    static const boost::uint8_t preambule = 215;
    
    typedef std::vector< boost::uint8_t > buffer_type;
    typedef std::pair< boost::uint8_t, boost::uint8_t > checksum_type;

    buffer_type buffer;

    checksum_type checksum() const
    {
        checksum_type ch( 0, 0 );

        for ( buffer_type::size_type i = 0; i < buffer.size(); i += 2 )
        {
            ch.first ^= buffer[ i ];
            ch.second ^= buffer[ i + 1 ];
        }

        if ( buffer.size() % 2 ) 
            ch.first ^= buffer.back();
    
        return ch;
    }


};

#endif /* RSFRAME_HPP */
