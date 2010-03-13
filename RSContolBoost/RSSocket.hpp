#ifndef RSSOCKET_HPP
#define RSSOCKET_HPP

#include <boost/asio.hpp>

#include "RSPort.hpp"
#include "RSFrame.hpp"


class RSSocket : boost::noncopyable
{

public:

    RSSocket( RSPort port1, RSPort port2, boost::asio::io_service &io_service ) : io_service( io_service ), port1( io_service, port1.name ),
                                                                                  port2( io_service, port2.name ), port_id1( port1.id ), port_id2( port2.id )
    {
		const boost::asio::serial_port::baud_rate br(9600);
		const boost::asio::serial_port::flow_control fc(boost::asio::serial_port::flow_control::none);
		const boost::asio::serial_port::parity p(boost::asio::serial_port::parity::none);
		const boost::asio::serial_port::stop_bits sb(boost::asio::serial_port::stop_bits::one);
		const boost::asio::serial_port::character_size cs(8);
		
        this->port1.set_option(br);
		this->port1.set_option(fc);
		this->port1.set_option(p);
		this->port1.set_option(sb);
        this->port1.set_option(cs);

		this->port2.set_option(br);
		this->port2.set_option(fc);
		this->port2.set_option(p);
		this->port2.set_option(sb);
        this->port2.set_option(cs);

        ack1 = 0;
        seq1 = 0;
    }
 
    /*
    *@param handler pointer to a function or an object whose class overloads operator(), 
    *template < typename ForwardIterator >
    *void handler( ForwardIterator first, ForwardIterator last, bool ok );
    **/
    template< typename Handler >
    void asyncReceive( Handler handler );
    		
    /*
    *@param handler pointer to a function or an object whose class overloads operator(), 
    *void handler( bool ok );
    **/
    template< typename ForwardIterator, typename Handler > 
    void asyncSend( ForwardIterator first, ForwardIterator last, Handler handler )
    {
        RSFrame::Desc desc;

        std::iterator_traits< ForwardIterator >::difference_type data_size = std::distance( first, last );

        desc.number_of_packages = (data_size + RSFrame::max_data_size - 1) / RSFrame::max_data_size;
        
        desc.ack = ack1;
        desc.seq = seq1;
        desc.package_id = 1;

        while ( data_size > 0 )
        {
            
            RSFrame frame( desc,  )

        }

    }   
    
    /*
    *void handler( bool ok );
    **/
    template< typename Handler >
    void connect( Handler handler );

private:

    boost::asio::io_service &io_service;
    boost::asio::serial_port port1;
    boost::asio::serial_port port2;
    RSFrame::Desc::id_type port_id1;
    RSFrame::Desc::id_type port_id2;
    RSFrame::Desc::id_type dest_port_id1;
    RSFrame::Desc::id_type dest_port_id2;
    boost::uint16_t ack1;
    boost::uint16_t ack2;
    boost::uint16_t seq1;
    boost::uint16_t seq2;


    
};


#endif //RSSOCKET_HPP