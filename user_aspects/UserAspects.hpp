#ifndef USERASPECTS_HPP
#define USERASPECTS_HPP


#include <vector>

class UserAspects
{
private :
    bool		_admin;

public :
    UserAspects();
    UserAspects( bool right);
    UserAspects(const UserAspects& obj);
    UserAspects& operator=(const UserAspects& obj);
    ~UserAspects();

    bool	getAdmin( void ) const;
    void	setAdmin( bool new_right );
};

#endif