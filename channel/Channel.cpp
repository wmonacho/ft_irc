/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: will <will@student.42lyon.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 14:31:22 by wmonacho          #+#    #+#             */
/*   Updated: 2023/05/08 16:53:44 by will             ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel( void )
{
	this->_name = "";
	this->_topic = "";
}

Channel::Channel( std::string name)
{
	this->_name = name;
}

Channel::Channel( Channel const & obj)
{
	*this = obj;
}

Channel	&Channel::operator=( Channel const & rhs)
{
	(void)rhs;
	return (*this);
}

Channel::~Channel( void )
{

}

std::string	Channel::getName( void ) const
{
	return (this->_name);
}

void	Channel::setName( std::string new_string )
{
	this->_name = new_string;
}

std::string	Channel::getTopic( void ) const
{
	return (this->_topic);
}

void	Channel::setTopic( std::string new_string )
{
	this->_topic = new_string;
}

void	Channel::setUserList(const User & user)
{
	this->_channel_user_list.push_back(user);
}

std::vector<User>	Channel::getUserList( void ) const
{
	return (this->_channel_user_list);
}