#include "ChannelAspects.hpp"

ChannelAspects::ChannelAspects( void )
{
	this->_admin = false;
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

bool	ChannelAspects::getAdmin( void )
{
	if(this->_admin)
		return true;
	return false;
}

void	ChannelAspects::setAdmin( bool new_right )
{
	this->_admin = new_right;
}