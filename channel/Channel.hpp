/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmonacho <wmonacho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 13:57:00 by wmonacho          #+#    #+#             */
/*   Updated: 2023/05/04 17:07:44 by wmonacho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP


#include <vector>
#include "../user/User.hpp"

class Channel
{
	private :
		std::string	_name;
		std::vector<User> _channel_user_list;
	public :
		Channel();
		Channel( std::string name);
        Channel(const Channel& obj);
        Channel& operator=(const Channel& obj);
        ~Channel();
		
		void				setName( std::string new_string );
		void				setUserList(const User & user);
		std::string			getName( void ) const;
		std::vector<User>	getUserList( void ) const;
};

#endif