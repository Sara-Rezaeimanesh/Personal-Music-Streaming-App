#ifndef USER_HPP
#define USER_HPP "USER_HPP"
#include "playlist.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <set>
#include <regex>

class Music;
class Filter;

struct Pcompare{
	bool operator()( Playlist* const& pl1 , Playlist* const& pl2 )
	{
		return pl1->id_is_smaller( pl2 );
	}
};

class User{
public:
	User( std::string email_ , std::string username_ , std::string password_ );
	void enter(){ logged_in = true; }
	void logout();
	void add_playlist( std::vector <std::string> pl_info , int id );
	std::string print() { return username; }
	void add_song_to_playlist( Music* music , int pl_id = 0 );
	void delete_song_from_playlist( Music* music , int pl_id );
	void get_likes();
	void remove_song_from_playlist( Music* music );
	void add_filters( std::string filter_args );
	std::string print_pls( std::string mode );
	std::string print_pl( int pl_id , Playlist* pl , std::string mode );
	void delete_filters();
	bool has_same_email( std::string email_ ){ return email_ == email; };
	std::string get_name(){ return username; }
	std::set <Music*,Mcompare> filter_songs( std::set <Music*,Mcompare> songs );
	bool username_matches( std::string name ){ return username == name; }
	bool identification_matches( std::string email_ , std::string password_ );
	bool has_common_info( std::string email_ , std::string username_ );
	bool username_is_smaller( User* const& user2 ){ return this->username < user2->username; }
	bool has_pl( int pl_id );
	bool likes_music( Music* music );
	int get_similarity( User* curr_user );
	Playlist* find_playlist( int pl_id );
	std::string generate_salt( std::size_t length );
	std::size_t generate_hash( std::string pass );
private:
	void replace_filter( Filter* filter );
	void validate_id( int pl_id );
	void calculate_bounds( std::string , int &min , int &max );
	Filter* find_filter( std::string filter_args );
	Filter* make_bounded_filter( std::string filter_args , 
								const std::regex like_format , const std::regex year_format );
	Filter* make_artist_filter( std::string filter_args , std::regex artist_format );
	std::string make_artist_str( std::string filter_args );
	std::string email , username , salt;
	std::size_t password;
	std::set<Playlist*,Pcompare> playlists;
	std::vector <Filter*> filters;
	bool logged_in;
};

#endif