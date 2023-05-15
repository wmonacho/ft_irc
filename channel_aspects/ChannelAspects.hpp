#ifndef CHANNELASPECTS_HPP
#define CHANNELASPECTS_HPP


#include <vector>

class ChannelAspects
{
	private :
		bool		_admin;
	public :
		ChannelAspects();
		ChannelAspects( bool right);
        ChannelAspects(const ChannelAspects& obj);
        ChannelAspects& operator=(const ChannelAspects& obj);
        ~ChannelAspects();

		bool	getAdmin( void );
		void	setAdmin( bool new_right );
};

#endif