/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmonacho <wmonacho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 15:54:00 by wmonacho          #+#    #+#             */
/*   Updated: 2023/05/17 15:58:00 by wmonacho         ###   ########.fr       */
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
		std::string	_realname;
	public :
		User( void );
		~User( void );
		User( User const & obj);
		User		&operator=( User const & rhs);
		void		setRealname( std::string new_string );
		void		setPassword( std::string new_string );
		void		setUsername( std::string new_string );
		void		setNickname( std::string new_string );
		void		setAdmin( int admin);
		
		bool	operator==(const User* other) const;
		
		bool				getAdmin( void ) const;
		std::string			getEmailaddress( void );
		std::string			getRealname( void ) const;
		std::string			getPassword( void ) const;
		std::string			getUsername( void ) const;
		std::string			getNickname( void ) const;
};


#endif