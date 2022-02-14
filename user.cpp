#include "user.hpp"
#include "playlist.hpp"
#include "exceptions.hpp"
#include "filter.hpp"
#include "comment.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <functional>
#include <random>
#include <regex>
constexpr char NOTHING[] = "";
constexpr char ARTIST[] = "artist";
constexpr char LIKES[] = "min_like";
constexpr char YEAR[] = "min_year";
constexpr char AND[] = "and";
constexpr char LIKES_STR[] = "likes";
constexpr char PRIVATE[] = "private";
constexpr char RAND_STR[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz@$%&";
constexpr char LIKE_FORMAT[] = "(min_like )(.*)(max_like )(.*)";
constexpr char YEAR_FORMAT[] = "(min_year )(.*)(max_year )(.*)";
constexpr char ARTIST_FROMAT[] = "(artist )(.*)";
constexpr int TYPE = 0;
constexpr int MIN_FILTER_ARGS = 2;
constexpr int BOUNDED_FILTER_AGRS = 4;
constexpr int EMPTY_INT = 1;
constexpr int MIN_IDX = 1;
constexpr int FIRST = 1;
constexpr int MAX_IDX = 3;
constexpr int ID_LOWER_BOUND = 0;
constexpr int SALT_SIZE = 20;

enum PLattrib_idx{
	name_idx = 0,
	privacy_idx = 1
};

User::User( std::string email_ , std::string username_ , std::string password_ ) :
			email( email_ ) , username( username_ )
{
	logged_in = false;
	Playlist* pl = new Playlist( TYPE , LIKES_STR , PRIVATE );
	playlists.insert( pl );
	salt = generate_salt( SALT_SIZE );
	password = generate_hash( salt + password_ );
}

std::size_t User::generate_hash( std::string pass )
{
	std::hash<std::string> mystdhash; 
	return mystdhash(pass);
}

std::string User::generate_salt( std::size_t length )
{
	const std::string CHARACTERS = RAND_STR;
    std::random_device random_device;
    std::mt19937 generator( random_device() );
    std::uniform_int_distribution<> distribution( TYPE , CHARACTERS.size() - EMPTY_INT );
    std::string random_string;
    for ( std::size_t i = 0 ; i < length ; ++i )
        random_string += CHARACTERS[distribution( generator )];
    
    return random_string;
}

std::string User::print_pls( std::string mode )
{
	std::ostringstream body;
	for( Playlist* pl : playlists )
		body << pl->print( mode );
	return body.str();
}

std::string User::make_artist_str( std::string filter_args )
{
	std::string artist;
	std::istringstream artist_istream( filter_args );
	getline( artist_istream , artist , SPACE );
	getline( artist_istream , artist );
	return artist;
}

Filter* User::make_artist_filter( std::string filter_args , std::regex artist_format )
{
	std::string artist = make_artist_str( filter_args );
	ArtistFilter* filter = new ArtistFilter( ARTIST_FROMAT , artist );
	return filter;
}

void User::calculate_bounds( std::string filter_args , int &min , int &max )
{
	std::vector <std::string> args_vec = convert_string_to_vec( filter_args );
	if( args_vec.size() < BOUNDED_FILTER_AGRS )
		throw BadRequestEx();
	min = atoi( args_vec[MIN_IDX].c_str() );
	max = atoi( args_vec[MAX_IDX].c_str() );
	if( min > max )
		throw BadRequestEx();
}

Filter* User::make_bounded_filter( std::string filter_args , const std::regex like_format
									 , const std::regex year_format )
{
	int min , max;
	calculate_bounds( filter_args , min , max );
	if( regex_match( filter_args , like_format ) )
		return new LikeFilter( LIKE_FORMAT , min , max );
	if( regex_match( filter_args , year_format ) )
		return new YearFilter( YEAR_FORMAT , min , max );
	throw BadRequestEx();
}

Filter* User::find_filter( std::string filter_args )
{
	std::regex like_format( LIKE_FORMAT );
	std::regex year_format( YEAR_FORMAT );
	std::regex artist_format( ARTIST_FROMAT );
	if( regex_match( filter_args , artist_format ) )
		return make_artist_filter( filter_args , artist_format );
	if( regex_match( filter_args , year_format ) || regex_match( filter_args , like_format ) )
		return make_bounded_filter( filter_args , like_format , year_format );
	throw BadRequestEx();
}

std::set <Music*,Mcompare> User::filter_songs( std::set <Music*,Mcompare> songs )
{
	if( !filters.size() )
		return songs;
	AndFilter* and_f = new AndFilter( AND , filters );
	std::set <Music*,Mcompare> filtered_songs = and_f->meetcriteria( songs );
	delete and_f;
	return filtered_songs;
}

void User::logout()
{
	delete_filters();
	logged_in = false; 
}

void User::delete_filters()
{
	for( auto itr = filters.begin() ; itr != filters.end() ; ++itr )
		delete (*itr);
	filters.clear();
}

void User::add_filters( std::string filter_args )
{
	Filter* new_filter = find_filter( filter_args );
	for( auto itr = filters.begin() ; itr != filters.end() ; ++itr )
	{
		if( ( *itr )->has_same_type( new_filter ) )
		{
			filters.erase( itr );
			filters.push_back( new_filter );
			return;
		}
	}
	filters.push_back( new_filter );
}

bool User::identification_matches( std::string email_ , std::string password_ )
{
	return email == email_ && password == generate_hash( salt + password_ );
}

bool User::has_common_info( std::string email_ , std::string username_ )
{
	if( username_ == username || email_ == email )
		return true;
	return false;
}

void User::add_playlist( std::vector <std::string> pl_info , int id )
{
	for( Playlist* pl : playlists )
		if( pl->has_same_name( pl_info[name_idx] ) )
			throw BadRequestEx();
	Playlist* playlist = new Playlist( id , pl_info[name_idx] , pl_info[privacy_idx] );
	playlists.insert( playlist );
}

Playlist* User::find_playlist( int pl_id )
{
	for( Playlist* pl : playlists )
		if( pl->id_matches( pl_id ) )
			return pl;
	throw PermissionDeniedEx();
}

std::string User::print_pl( int pl_id , Playlist* pl , std::string mode )
{
	std::ostringstream body;
	if( !( pl_id == 0 ) || !( mode == "other" ) )
		body << pl->print_songs( mode );
	return body.str();
}

int User::get_similarity( User* curr_user )
{
	return ( *playlists.begin() )->get_similarity( *curr_user->playlists.begin() );
}

bool User::has_pl( int pl_id )
{
	for( Playlist* pl : playlists )
		if( pl->id_matches( pl_id ) )
			return true;
	return false;
}

bool User::likes_music( Music* music )
{
	return (*playlists.begin())->has_music( music );
}

void User::remove_song_from_playlist( Music* music )
{
	(*playlists.begin())->remove_song( music );
}

void User::add_song_to_playlist( Music* music , int pl_id )
{
	validate_id( pl_id );
	Playlist* pl = find_playlist( pl_id );
	pl->add_music( music );
}

void User::validate_id( int pl_id )
{
	if( pl_id < ID_LOWER_BOUND )
		throw BadRequestEx();
}

void User::delete_song_from_playlist( Music* music , int pl_id )
{
	validate_id( pl_id );
	Playlist* pl = find_playlist( pl_id );
	pl->remove_song( music );
}

void User::get_likes()
{
	(*playlists.begin())->print_likes();
}