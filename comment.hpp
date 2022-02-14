#ifndef COMMENT_HPP
#define COMMENT_HPP "COMMENT_HPP"
#include <string>
#include <vector>

class User;

class Comment{
public: 
	Comment( int time_ , std::string txt_ , User* user_ ) : time( time_ ) , txt( txt_ ) , user( user_ ){}
	bool operator<( const Comment& c ) const;
	void print();
private:
	int time;
	User* user;
	std::string txt;
};

#endif