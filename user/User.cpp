/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: will <will@student.42lyon.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 15:54:01 by wmonacho          #+#    #+#             */
/*   Updated: 2023/05/08 16:22:19 by will             ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

User::User( void )
{
	this->_nickname = "";
	this->_username = "";
	this->_realname = "";
	this->_password = "";
	this->_admin = 0;
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
	this->_admin = rhs._admin;
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

std::string	User::getRealname( void ) const
{
	return (this->_realname);
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

void	User::setAdmin( int admin)
{
	this->_admin = admin;
}

bool User::operator==(const User& other) const
{
	return this->_nickname == other._nickname;
}

bool	User::getAdmin( void ) const
{
	if (this->_admin)
		return true;
	return false;
}