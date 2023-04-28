/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmonacho <wmonacho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 15:54:00 by wmonacho          #+#    #+#             */
/*   Updated: 2023/04/28 17:42:53 by wmonacho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

#include <iostream>
#include <set>
#include <map>

class User
{
	private :
		std::string	_nickname;
		std::string	_username;
		std::string	_password;
		std::string	_full_name;
		std::string	_email_address;
		std::string	_host_address;
		std::string	_server_connect;
		std::string _m_host;
		bool 		_m_away;
		std::set<std::string> _m_channels;
		std::map<std::string, std::set<std::string>> _m_modes;
		std::string _m_server;
	public :
		User( void );
		~User( void );
		User( User const & obj);
		User		&operator=( User const & rhs);
		void		setEmailaddress( std::string new_string );
		void		setFullname( std::string new_string );
		void		setPassword( std::string new_string );
		void		setUsername( std::string new_string );
		void		setNickname( std::string new_string );
		std::string	getEmailaddress( void );
		std::string	getFullname( void );
		std::string	getPassword( void );
		std::string	getUsername( void );
		std::string	getNickname( void );
};


#endif