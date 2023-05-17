/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: will <will@student.42lyon.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 14:31:22 by wmonacho          #+#    #+#             */
/*   Updated: 2023/05/15 14:56:41 by will             ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel( void )
{
	this->_name = "";
	this->_topic = "";
       this->_password = "";
       this ->_inviteOnly = false;
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

std::string	Channel::getTopic( void ) const
{
	return (this->_topic);
}

std::string Channel::getPassword() const
{
    return (this->_password);
}
std::string	Channel::getName( void ) const
{
    return (this->_name);
}

bool    Channel::getInviteOnly() const
{
    return (this->_inviteOnly);
}

void	Channel::setTopic( std::string new_string )
{
	this->_topic = new_string;
}

void	Channel::setName( std::string new_string )
{
    this->_name = new_string;
}

void    Channel::setInviteOnly(bool i)
{
    this->_inviteOnly = i;
}

void    Channel::setPassword(std::string pass)
{
    this->_password = pass;
}

void	Channel::setUserList(const User * new_user, ChannelAspects channel_aspects)
{
	this->_channel_user_list[new_user] = channel_aspects;
}

std::map<const User*, ChannelAspects>	Channel::getUserList( void ) const
{
	return (this->_channel_user_list);
}

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