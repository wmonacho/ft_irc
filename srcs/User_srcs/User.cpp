/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmonacho <wmonacho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 15:54:01 by wmonacho          #+#    #+#             */
/*   Updated: 2023/04/28 17:35:52 by wmonacho         ###   ########.fr       */
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

std::string getHost() const {
    return (this->m_host);
}

bool isAway() const {
    return (this->m_away);
}
void setAway(bool away) {
    this->m_away = away;
}
void addChannel(const std::string& channel) {
    m_channels.insert(channel);
}
void removeChannel(const std::string& channel) {
    m_channels.erase(channel);
}
bool isInChannel(const std::string& channel) const {
    return m_channels.find(channel) != m_channels.end();
}
void addMode(const std::string& channel, const std::string& mode) {
    m_modes[channel].insert(mode);
}
void removeMode(const std::string& channel, const std::string& mode) {
    m_modes[channel].erase(mode);
}
bool hasMode(const std::string& channel, const std::string& mode) const {
    auto it = m_modes.find(channel);
    if (it != m_modes.end()) {
        return it->second.find(mode) != it->second.end();
    }
    return false;
}
void setServer(const std::string& server) {
    m_server = server;
}
std::string getServer() const {
    return m_server;
}