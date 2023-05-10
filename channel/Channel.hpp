/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmonacho <wmonacho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 13:57:00 by wmonacho          #+#    #+#             */
/*   Updated: 2023/05/10 17:32:41 by wmonacho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP


#include <vector>
#include "../../user/User.hpp"
#include "ChannelAspects/ChannelAspects.hpp"

class Channel
{
	private :
		std::string	_name;
		std::string	_topic;
		bool		_secret;
		std::map<const User&, ChannelAspects> _channel_user_list;
	public :
		Channel();
		Channel( std::string name);
        Channel(const Channel& obj);
        Channel& operator=(const Channel& obj);
        ~Channel();
		
		void	kickUserFromChannel(User user);
		void	setUserList(const User & user);
		void	setName( std::string new_string );
		void	setTopic( std::string new_string );
		
		bool	channelIsSecret( void );
		
		std::string			getName( void ) const;
		std::string			getTopic( void ) const;
		std::vector<User&>	getUserList( void ) const;
};

#endif