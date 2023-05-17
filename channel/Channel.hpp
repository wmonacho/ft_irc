/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmonacho <wmonacho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 13:57:00 by wmonacho          #+#    #+#             */
/*   Updated: 2023/05/17 15:54:54 by wmonacho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP


#include <vector>
#include "../user/User.hpp"
#include "../channel_aspects/ChannelAspects.hpp"

class User;
class ChannelApects;
class Channel
{
	private :
		std::string	_name;
		std::string	_topic;
		bool		_secret;
		std::map<const User*, ChannelAspects> _channel_user_list;
	public :
		Channel();
		Channel( std::string name);
        Channel(const Channel& obj);
        Channel& operator=(const Channel& obj);
        ~Channel();
		
		void						setUserList(const User * new_user, ChannelAspects channel_aspects);
		void						setName( std::string new_string );
		void						setTopic( std::string new_string );

		std::string					getName( void ) const;
		std::string					getTopic( void ) const;
		const User* 				getUser(const User *user);
		bool						getUserAdmin(User *user);
		const std::string			getUserUsername(User *user);
		const std::string			getUserRealname(User *user);
		const std::string			getUserPassword(User *user);
		const std::string			getUserNickname(User *user);
		std::map<const User*, ChannelAspects>	getUserList( void ) const;
		
		
		bool	channelIsSecret( void );
		bool	userInChannel(const User *user);
		void	kickUserFromChannel(User *user);

};

#endif