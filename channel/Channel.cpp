/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmonacho <wmonacho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 14:31:22 by wmonacho          #+#    #+#             */
/*   Updated: 2023/05/11 14:43:09 by wmonacho         ###   ########.fr       */
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

bool	Channel::userInChannel(User *user)
{
	std::map<const User*, ChannelAspects>::iterator it_user = this->getUserList().find(user);

	if (it_user == this->getUserList().end())
		return (false);
	return true;
}

const User* Channel::getChannelUser(User *user) const
{
	std::map<const User*, ChannelAspects>::iterator it_user = this->getUserList().find(user);
	
	if (it_user == this->getUserList().end())
		return (it_user->first);
	return it_user->first;
}

bool	Channel::getUserAdmin(User *user)
{
	std::map<const User*, ChannelAspects>::iterator it_user = this->getUserList().find(user);
	
	if (it_user->second.getAdmin())
		return true;
	return false;
}

std::string		Channel::getUserUsername(User *user)
{
	
}

std::string		Channel::getUserNickname(User *user)
{
	
}

std::string		Channel::getUserPassword(User *user)
{
	
}

std::string		Channel::getUserRealname(User *user)
{
	
}