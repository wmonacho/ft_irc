#ifndef USERASPECTS_HPP
#define USERASPECTS_HPP


#include <vector>
#include "../user/User.hpp"
#include <algorithm>

class UserAspects
{
private :
    bool		_admin;

public :
    UserAspects();
    UserAspects( int right);
    UserAspects(const UserAspects& obj);
    UserAspects& operator=(const UserAspects& obj);
    ~UserAspects();

    bool	getAdmin( void ) const;
    void	setAdmin( int right );
};

#endif