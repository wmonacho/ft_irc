/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmonacho <wmonacho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 14:31:22 by wmonacho          #+#    #+#             */
/*   Updated: 2023/05/17 15:54:35 by wmonacho         ###   ########.fr       */
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


// GETTER

std::string	Channel::getName( void ) const
{
	return (this->_name);
}

std::string	Channel::getTopic( void ) const
{
	return (this->_topic);
}

std::map<const User*, ChannelAspects>	Channel::getUserList( void ) const
{
	return (this->_channel_user_list);
}

const User* Channel::getUser(const User *user)
{
	if (this->getUserList().find(user) == this->getUserList().end())
		return (this->getUserList().find(user)->first);
	return this->getUserList().find(user)->first;
}

bool	Channel::getUserAdmin(User *user)
{
	if (this->getUserList().find(user)->second.getAdmin())
		return true;
	return false;
}

const std::string		Channel::getUserUsername(User *user)
{
	return (this->getUserList().find(user)->first->getUsername());
}

const std::string		Channel::getUserNickname(User *user)
{
	return (this->getUserList().find(user)->first->getNickname());
}

const std::string		Channel::getUserPassword(User *user)
{
	return (this->getUserList().find(user)->first->getPassword());
}

const std::string		Channel::getUserRealname(User *user)
{
	return (this->getUserList().find(user)->first->getRealname());
}

// SETTER

void	Channel::setName( std::string new_string )
{
	this->_name = new_string;
}

void	Channel::setTopic( std::string new_string )
{
	this->_topic = new_string;
}

void	Channel::setUserList(const User * new_user, ChannelAspects channel_aspects)
{
	this->_channel_user_list[new_user] = channel_aspects;
}


// CHANNEL FUNCTIONS

bool	Channel::channelIsSecret( void )
{
	if (this->_secret)
		return true;
	return false;
}

bool	Channel::userInChannel(const User *user)
{
	if (this->getUserList().find(user) == this->getUserList().end())
		return (false);
	return true;
}
