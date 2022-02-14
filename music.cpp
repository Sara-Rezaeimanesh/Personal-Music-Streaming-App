#include "music.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include "comment.hpp"
#include "exceptions.hpp"
#include "user.hpp"
constexpr char LIST[] = "list";
constexpr char INDIV[] = "individual";
constexpr char LIKES_NUM[] = "#likes: ";
constexpr char COMMENTS_NUM[] = "#comments: ";
constexpr char PL_NUM[] = "#playlists: ";
constexpr int NONE = 0;

Music::Music( int id_ , std::string title_ , std::string artist_ , int release_year_ , std::string link_ )
{
	id = id_;
	release_year = release_year_;
	title = title_;
	artist = artist_;
	link = link_;
	num_of_likes = NONE;
	num_of_pls = NONE;
}

std::string Music::print_music( std::string mode , std::string form_func ) const
{
	std::ostringstream body;
	std::string line;
	if( mode == CSV )
		body << "<div class=\"grid-1-5\">" << "<h2>" << artist << "</h2>" 
		<< "<h3><span class=\"uppercase\">" << title << "</span></h3>" << "<p>" << release_year << "</p></div>";
	else
	{
		body << "<h2>" << artist << "</h2><p></p>" << "<h3><span class=\"uppercase\">" << title << "</span></h3>" << "<p>" << release_year << "</p>"
		<< "<audio controls><source src =\"" << link << "\"></audio>" << "<p></p>" << "<ul><li>" <<  LIKES_NUM << num_of_likes << "</li>" 
		<< "<li>" <<  COMMENTS_NUM << comments.size() << "</li>" << "<li>" <<  PL_NUM << num_of_pls << "</li></ul><form action=\""<< form_func << "?id=" << id << "\" method=\"post\"><button type=\"submit\">" << mode << "</button></form>";
	}
	return body.str();	
}

void Music::add_comment( std::string comment , int time , User* user )
{
	comments.insert( Comment( time , comment , user ) );
}

bool Music::id_matches( int id_ )
{
	if( id == id_ )
		return true;
	return false;
}

bool Music::likes_match( int min_like , int max_like )
{
	std::cout << num_of_likes <<" "<< max_like << " " <<  min_like << std::endl;
	return num_of_likes <= max_like && num_of_likes >= min_like;
}


bool Music::year_match( int min_year , int max_year )
{
	return release_year <= max_year && release_year >= min_year;
}

void Music::get_comments()
{
	if( comments.size() == NONE )
		throw EmptyEx();
	for( Comment c : comments )
		c.print();
}

std::string Music::make_table_block( std::string mode )
{
	std::ostringstream table_block;
	table_block << "<tr><td><a href=\"show_song_info?id=" << id << "\" target=\"_parent\" dideo-checked=\"true\"><span dir=\"rtl\" >" << title << "</span></a>" << "</td>" << std::endl << "<td>" << artist << "</td>"
			<< std::endl << "<td>" << release_year << "</td>";
	if( mode == "with remove" )
		table_block << "<td><a href=\"remove_song?id=" << id << "\" target=\"_parent\" dideo-checked=\"true\"><span dir=\"rtl\" >" << "remove" << "</span></a></td>";
	table_block << std::endl << "</tr>";
	return table_block.str();
}