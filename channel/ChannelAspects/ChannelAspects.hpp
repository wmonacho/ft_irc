#ifndef CHANNELASPECTS_HPP
#define CHANNELASPECTS_HPP


#include <vector>
#include "../user/User.hpp"

class ChannelAspects
{
	private :
		bool		_admin;
	public :
		ChannelAspects();
        ChannelAspects(const ChannelAspects& obj);
        ChannelAspects& operator=(const ChannelAspects& obj);
        ~ChannelAspects();
};