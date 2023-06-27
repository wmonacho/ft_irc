/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmonacho <wmonacho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 15:54:01 by wmonacho          #+#    #+#             */
/*   Updated: 2023/06/23 18:12:24 by wmonacho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

User::User( void )
{
	this->_nickname = "";
	this->_username = "";
	this->_realname = "";
	this->_password = "";
}

User::User( User const & obj)
{
	*this = obj;
}

User	&User::operator=( User const & rhs)
{
	this->_nickname = rhs._nickname;
	this->_username = rhs._username;
	this->_realname = rhs._realname;
	this->_password = rhs._password;
	this->_socket	= rhs._socket;
	return (*this);
}

User::~User( void )
{
	//std::cout << "USER DESTRUCTOR" << std::endl;
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

std::string	User::getRealname( void ) const
{
	return (this->_realname);
}

int	User::getSocket( void ) const
{
	return (this->_socket);
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

void	User::setRealname( std::string new_string )
{
	this->_realname = new_string;
}

void	User::setSocket( int fd )
{
	this->_socket = fd;
}

bool User::operator==(const User* other) const
{
	return this->_nickname == other->_nickname;
}
