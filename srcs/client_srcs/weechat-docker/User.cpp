/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmonacho <wmonacho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 15:54:01 by wmonacho          #+#    #+#             */
/*   Updated: 2023/04/27 12:50:40 by wmonacho         ###   ########.fr       */
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
	return (*this);
}

User::~User( void )
{

}

std::string	User::getNickname( void )
{
	return (this->_nickname);
}

std::string	User::getUsername( void )
{
	return (this->_username);
}

std::string	User::getPassword( void )
{
	return (this->_password);
}

std::string	User::getFullname( void )
{
	return (this->_full_name);
}

std::string	User::getEmailaddress( void )
{
	return (this->_email_address);
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

void	User::setEmailaddress( std::string new_string )
{
	this->_email_address = new_string;
}