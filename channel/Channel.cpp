/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmonacho <wmonacho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 14:31:22 by wmonacho          #+#    #+#             */
/*   Updated: 2023/06/20 14:38:23 by wmonacho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel() 
{
    this->_name = "";
    this->_topic = "";
    this->_password = "";
    this ->_inviteOnly = false;
    this->_userLimit = -1;	
}

Channel::Channel( std::string name )
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
    *this = obj;
}

Channel::~Channel( void ) {}

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

const std::map<const User*, UserAspects>&	Channel::getUserList( void ) const
{
    return (this->_channel_user_list);
}

const User* Channel::getUser(const User *user)
{
	//std::cout << "DEBUG :" << this->getUserList().count(user) << std::endl;
	//if (this->getUserList().find(user) != this->getUserList().end())
	//{
	//	std::cout << "DEBUG" << std::endl;
	//	std::cout << "DEBUG user :" << this->getUserList().find(user)->first->getNickname() << std::endl;
	//}
	//else 
	//	std::cout << "NO USER" << std::endl;
	return this->getUserList().find(user)->first;
}

bool	Channel::getUserAdmin(const User *user) const
{
	if (this->_channel_user_list.count(user) == 0)
		return (false);
    if (this->getUserList().find(user)->second.getAdmin())
	 return true;
    return false;
}

const std::string		Channel::getUserUsername(User *user)
{
	if (this->_channel_user_list.count(user) == 0)
		return ("");
    return (this->getUserList().find(user)->first->getUsername());
}

const std::string		Channel::getUserNickname(User *user)
{
	if (this->_channel_user_list.count(user) == 0)
		return ("");
    return (this->getUserList().find(user)->first->getNickname());
}

const std::string		Channel::getUserPassword(User *user)
{
	if (this->_channel_user_list.count(user) == 0)
		return ("");
    return (this->getUserList().find(user)->first->getPassword());
}

const std::string		Channel::getUserRealname(User *user)
{
	if (this->_channel_user_list.count(user) == 0)
		return ("");
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

void	Channel::setUserList(const User  *new_user, UserAspects user_aspects)
{
	this->_channel_user_list.insert(std::make_pair<const User*, UserAspects>(new_user, user_aspects));	
	//this->_channel_user_list[new_user] = user_aspects;
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
	if (this->_channel_user_list.find(user) == this->_channel_user_list.end() )
		return (false);
    return true;
}

void	Channel::kickUserFromChannel(const User *user)
{
	this->_channel_user_list.erase(user);
}

void	Channel::changeUserAdmin(const User* user, bool i)
{
    this->_channel_user_list.find(user)->second.setAdmin(i);
}

void   	Channel::addGuestInviteList(std::string new_guest)
{
    this->_invite_list.push_back(new_guest);
}

bool   	Channel::userInInviteList(std::string guest)
{
    for(unsigned int i = 0; i < this->_invite_list.size(); i++)
    {
        if (this->_invite_list[i] == guest)
            return (true);
    }
    return (false);
}