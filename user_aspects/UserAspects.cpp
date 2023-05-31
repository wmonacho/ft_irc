#include "UserAspects.hpp"

UserAspects::UserAspects( void )
{
	this->_admin = false;
}

UserAspects::UserAspects( bool right )
{
	this->_admin = right;
}

UserAspects::UserAspects( UserAspects const & obj)
{
	*this = obj;
}

UserAspects	&UserAspects::operator=( UserAspects const & rhs)
{
	(void)rhs;
	return (*this);
}

UserAspects::~UserAspects( void )
{

}

bool	UserAspects::getAdmin( void )
{
	if(this->_admin)
		return true;
	return false;
}

void	UserAspects::setAdmin( bool new_right )
{
	this->_admin = new_right;
}