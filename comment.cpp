#include "comment.hpp"
#include "string"
#include "exceptions.hpp"
#include "user.hpp"
#include <iostream>
constexpr char DOUBLE_COLON[] = ": ";

bool Comment::operator<( const Comment& c ) const
{
	if( time < c.time )
		return true;
	else
		if( time == c.time )
			return user->username_is_smaller( c.user );
	return false;
}

void Comment::print()
{
	std::cout << time << SPACE;
	user->print();
	std::cout << DOUBLE_COLON << txt << std::endl;
}