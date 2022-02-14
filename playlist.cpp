#include "playlist.hpp"
#include "music.hpp"
#include "exceptions.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
constexpr char LIST[] = "list";
constexpr char OTHER[] = "other";
constexpr char ARTIST[] = "artist";
constexpr char AND[] = "and";
constexpr char LIKES_STR[] = "likes";
constexpr char PRIVATE[] = "private";
constexpr char EMPTY_INT = 0;

void Playlist::add_music( Music* music )
{
	for( Music* m : songs )
		if( m == music )
			throw BadRequestEx();
	songs.insert( music );
}

void Playlist::print_likes()
{
	if( songs.size() == EMPTY_INT )
		throw EmptyEx();
}

std::string Playlist::print( std::string mode )
{
	std::ostringstream table_block;
	if( mode == OTHER && privacy == PRIVATE )
		return table_block.str();
	table_block << "<tr><td><a href=\"show_pl_info?id=" << id 
	<< "\" target=\"_parent\" dideo-checked=\"true\"><span dir=\"rtl\" >" 
	<< name << "</span></a>" << "</td>" << std::endl << "<td>" << privacy << "</td>"
	<< std::endl << "</tr>";
	return table_block.str();
}

void Playlist::remove_song( Music* music )
{
	for( auto itr = songs.begin(); itr != songs.end(); ++itr )
	{
		if( (*itr) == music )
		{
			songs.erase( itr );
			return;
		}
	}
	throw BadRequestEx();
}

std::string Playlist::print_songs( std::string mode )
{
	std::ostringstream body;
	body << "<h1><span class=\"blue\"><span class=\"yellow\">"<< name << "</pan></h1>"
	<< "<h2>" << privacy << "</h2>"
	<< "<thead><tr><th><h1>Title</h1></th><th><h1>Artist</h1></th><th><h1>Releas Year</h1></th>";
	if( !( mode == OTHER ) )
		body << "<th><h1>Options</h1></th>";
	body << "</tr></thead><tbody>";
	std::string music_mode = ( mode == OTHER ) ? "without remove" : "with remove";
	for( Music* music : songs ) 
		body << music->make_table_block( music_mode );
	return body.str();
}

int Playlist::get_similarity( Playlist* pl )
{
	int num_of_similars = EMPTY_INT;
  	for( Music* music1 : songs )
  		for( Music* music2 : pl->songs )
  			if( !music1->id_is_smaller( music2 ) && !music2->id_is_smaller( music1 ) )
  				num_of_similars++;
  	return num_of_similars;
}

bool Playlist::name_matches( std::string name_ )
{
	return name == name_;
}

bool Playlist::has_music( Music* music2 )
{
	for( Music* music : songs )
		if( music == music2 )
			return true;
	return false;
}