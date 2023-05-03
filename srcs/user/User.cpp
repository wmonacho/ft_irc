/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmonacho <wmonacho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 15:54:01 by wmonacho          #+#    #+#             */
/*   Updated: 2023/05/03 17:30:47 by wmonacho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

User::User( void )
{

}

User::User( User const & obj)
{
	*this = obj;
}

User	&User::operator=( User const & rhs)
{
	(void)rhs;
	return (*this);
}

User::~User( void )
{

}

std::string	User::getNickname( void ) const
{
	return (this->_nickname);
}

std::string	User::getUsername( void ) const
{
	return (this->_username);
}

std::string	User::getPassword( void ) const
{
	return (this->_password);
}

std::string	User::getFullname( void ) const
{
	return (this->_full_name);
}

void	User::setNickname( std::string new_string )
{
	this->_nickname = new_string;
}

void	User::setUsername( std::string new_string )
{
	this->_username = new_string;
}

void	User::setPassword( std::string new_string )
{
	this->_password = new_string;
}

void	User::setFullname( std::string new_string )
{
	this->_full_name = new_string;
}

void	User::setAdmin( int admin)
{
	this->_admin = admin;
}
