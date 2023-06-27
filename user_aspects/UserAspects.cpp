#include "UserAspects.hpp"

UserAspects::UserAspects( void )
{
	this->_admin = false;
}

UserAspects::UserAspects( int right )
{
	this->_admin = right;
}

UserAspects::UserAspects( UserAspects const & obj)
{
	*this = obj;
}

UserAspects	&UserAspects::operator=( UserAspects const & rhs)
{
	this->_admin = rhs._admin;
	return (*this);
}

UserAspects::~UserAspects( void )
{

}

bool	UserAspects::getAdmin( void ) const
{
	if (this->_admin)
		return true;
	return false;
}

void	UserAspects::setAdmin( int right )
{
	this->_admin = right;
}