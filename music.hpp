#ifndef MUSIC_HPP 
#define MUSIC_HPP "MUSIC_HPP"
#include "comment.hpp"
#include <string>
#include <iostream>
#include <vector>
#include <set>
constexpr char SPACE = ' ';

class User;

class Music{
public:
	Music( int id_ , std::string title_ , std::string artist_ , int release_year_ , std::string link_ );
	std::string print_music( std::string mode , std::string form_func ) const;
	void print_id(){ std::cout << id; }
	void print_rest(){ std::cout<< SPACE << title << SPACE << artist << SPACE << release_year << std::endl; }
	void add_comment( std::string comment , int time , User* user );
	void add_like(){ num_of_likes++; }
	void remove_like(){ num_of_likes--; }
	void decr_pl_num(){ num_of_pls--; }
	void add_pl_num(){ num_of_pls++; };
	void get_comments();
	std::string get_artist(){ return artist; } 
	std::string make_table_block(  std::string mode = "" );
	bool id_matches( int id_ );
	bool likes_match( int min_like , int max_like );
	bool year_match( int min_year , int max_year );
	bool id_is_smaller( Music* music2 ){ return id < music2->id; }

private:
	int id; 
	int num_of_likes , num_of_pls;
	int release_year;
	std::set <Comment> comments;
	std::string title;
	std::string artist;
	std::string link;
};

#endif