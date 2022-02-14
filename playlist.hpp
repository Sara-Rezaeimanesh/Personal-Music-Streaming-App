#ifndef PLAYLIST_HPP
#define PLAYLIST_HPP "PLAYLIST_HPP"
#include <vector>
#include <string>
#include <iostream>
#include <set>
#include "commonfuncs.hpp"

struct Mcompare{
	bool operator()( Music* const& music1 , Music* const& music2 )
	{
		return music1->id_is_smaller( music2 ); 
	}
};

class Music;

class Playlist{
public:
	Playlist( int id_ , std::string name_ , std::string privacy_ ) : id( id_ ) , name( name_ ) , privacy( privacy_ ){}
	void add_music( Music* music );
	void print_likes();
	void remove_song(Music* music );
	std::string print( std::string mode );
	std::string print_songs( std::string mode );
	bool name_matches( std::string name_  );
	bool id_matches( int pl_id ){ return id == pl_id; }
	bool id_is_smaller( Playlist* const& pl2 ){ return this->id < pl2->id; }
	bool has_music( Music* music );
	bool has_same_name( std::string name_ ){ return name == name_; }
	int get_similarity( Playlist* pl );

private:
	std::set <Music*,Mcompare> songs;
	int id;
	std::string name;
	std::string privacy;
};

#endif