/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmonacho <wmonacho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 15:54:00 by wmonacho          #+#    #+#             */
/*   Updated: 2023/04/27 12:49:48 by wmonacho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

#include <iostream>

class User
{
	private :
		std::string	_nickname;
		std::string	_username;
		std::string	_password;
		std::string	_full_name;
		std::string	_email_address;
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