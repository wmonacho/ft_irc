/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: will <will@student.42lyon.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 14:31:22 by wmonacho          #+#    #+#             */
/*   Updated: 2023/05/26 10:18:53 by will             ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel( std::string name)
{
	this->_name = name;
	this->_topic = "";
    this->_password = "";
    this ->_inviteOnly = false;
    this->_userLimit = -1;
}

Channel::Channel( Channel const & obj)
{
	this->_name = obj._name;
	this->_topic = obj._topic;
    this->_password = obj._password;
	this->_secret = obj._secret;
    this->_inviteOnly = obj._inviteOnly;
    this->_topicAdmin = obj._topicAdmin;
    this->_userLimit = obj._userLimit;
}

Channel::~Channel( void )
{

}

std::string Channel::getTopic( void ) const
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

int  Channel::getUserLimit( void ) const
{
    return (this->_userLimit);
}

bool    Channel::getInviteOnly() const
{
    return (this->_inviteOnly);
}

bool    Channel::getTopicAdmin() const
{
    return (this->_topicAdmin);
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

void    Channel::setInviteOnly(bool i)
{
    this->_inviteOnly = i;
}

void    Channel::setTopicAdmin(bool i)
{
    this->_topicAdmin = i;
}

void Channel::setUserLimit(int nb)
{
    this->_userLimit = nb;
}

void    Channel::setPassword(std::string pass)
{
    this->_password = pass;
}

void	Channel::setUserList(const User * new_user, ChannelAspects channel_aspects)
{
	this->_channel_user_list.insert(std::make_pair(new_user, channel_aspects));
	return ;
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

void	Channel::kickUserFromChannel(const User *user)
{
	this->_channel_user_list.erase(this->getUserList().find(user)->first);
}

void	Channel::changeUserAdmin(const User* user, bool i)
{
	this->_channel_user_list[user].setAdmin(i);
}