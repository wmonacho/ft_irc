/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmonacho <wmonacho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 13:57:00 by wmonacho          #+#    #+#             */
/*   Updated: 2023/05/23 14:34:27 by wmonacho         ###   ########.fr       */
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
        std::string _password;
		bool		_secret;
        bool        _inviteOnly;
        bool        _topicAdmin;
    	int			_userLimit;
		std::map<const User*, ChannelAspects> _channel_user_list;
	public :
		Channel();
		Channel( std::string name);
        Channel(const Channel& obj);
        Channel& operator=(const Channel& obj);
        ~Channel();
		
		void	setUserList(const User * new_user, ChannelAspects channel_aspects);
		void	setName( std::string new_string );
        void	setPassword(std::string pass);
		void	setTopic( std::string new_string );
        void	setInviteOnly(bool i);
        void	setTopicAdmin(bool i);
		void   	setUserLimit(int nb);

		
		bool				getUserAdmin(User *user);
		std::string			getTopic( void ) const;
		const User* 		getUser(const User *user);
		std::string			getName( void ) const;
        bool				getInviteOnly( void ) const;
		bool                        getTopicAdmin( void ) const;
		int                         getUserLimit( void ) const;
        std::string                 getPassword(void) const;
        const std::string			getUserUsername(User *user);
		const std::string			getUserRealname(User *user);
		const std::string			getUserPassword(User *user);
		const std::string			getUserNickname(User *user);
		std::map<const User*, ChannelAspects>	getUserList( void ) const;
		
		
		bool	channelIsSecret( void );
		bool	userInChannel(const User *user);
		void	kickUserFromChannel(const User *user);

};

#endif