#ifndef RSPORT_HPP
#define RSPORT_HPP

#include <boost/cstdint.hpp>

#include "RSFrame.hpp"



struct RSPort 
{
	RSPort() {}
    RSPort( const std::string &name, RSFrame::Desc::id_type id ) : name( name ) , id( id ) {} 

    std::string name;
    RSFrame::Desc::id_type id;
};


#endif //RSPort_HPP