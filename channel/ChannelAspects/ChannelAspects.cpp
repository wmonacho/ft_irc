#include "ChannelAspects.hpp"

ChannelAspects::ChannelAspects( void )
{
}


ChannelAspects::ChannelAspects( ChannelAspects const & obj)
{
	*this = obj;
}

ChannelAspects	&ChannelAspects::operator=( ChannelAspects const & rhs)
{
	(void)rhs;
	return (*this);
}

ChannelAspects::~ChannelAspects( void )
{

}