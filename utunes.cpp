#include "utunes.hpp"
#include <fstream>
#include "music.hpp"
#include <vector>
#include <iostream>
#include <string>
#include <iomanip>
#include <exception>
#include "exceptions.hpp"
#include "commonfuncs.hpp"
#include "server/server.hpp"
#include "user.hpp"
#include <cstdlib>
#include <ctime>
#include "handlers.hpp"
#include "playlist.hpp"
constexpr char LIST[] = "list";
constexpr char MUSIC[] = "music";
//constexpr char POST[] = "POST";
constexpr char OK[] = "OK\n";
//constexpr char GET[] = "GET";
constexpr char SELF[] = "self";
constexpr char OTHER[] = "other";
//constexpr char DELETE[] = "DELETE";
constexpr char INDIV[] = "individual";
constexpr char PERCENT = '%';
constexpr int STR_ATTRIBUTES_NUM = 5;
constexpr int NOTHING = 0;
constexpr int MID_IDX = 1;
constexpr int PID_IDX = 0;
constexpr int TIME_IDX = 2;
constexpr int COMMENT_IDX = 0;
constexpr int HUNDRED = 100;
constexpr int TWO_DECIMALS = 2;
constexpr int CURR_USER = 1;

enum MAttrib_idx{
	id_idx = 0,
	title_idx = 1,
	artist_idx = 2,
	year_idx = 3,
	link_idx = 4
};

enum Uattrib_idx{
	email_idx = 0,
	username_idx = 2,
	password_idx = 1
};

enum Lattrib_idx{
	username_l_idx = 0,
	email_l_idx = 1,
	password_l_idx = 2,
	id_l_idx = 3
};

Utunes::~Utunes()
{
	for( User* user : users )
		delete user;
	for( Music* music : songs )
		delete music;
}

void Utunes::add_filters( std::string filter_args )
{
	curr_user->add_filters( filter_args );
	std::cout << OK;
}

void Utunes::get_likes()
{
	curr_user->get_likes();
}

void Utunes::print_users()
{
	for( User* user : users )
	{
		if( user != curr_user )
		{
			user->print();
			std::cout << std::endl;
		}
	}
}

void Utunes::delete_song_from_pl( std::vector <std::string> sorted_args )
{
	int m_id = atoi( sorted_args[MID_IDX].c_str() );
	Music* music = get_song( m_id );
	curr_user->delete_song_from_playlist(  music , atoi( sorted_args[PID_IDX].c_str() ) );
	get_song( m_id )->decr_pl_num();
}

void Utunes::run()
{
	try
	{
		Server server(8080);
		server.get( "/" , new ShowPage( "signup_pg.html" ) );
		server.get( "/signup" , new SignupHandler( this ) );
		server.get( "/signup_success" , new ShowPage( "signup_succesful.html" ) );
		server.get( "/show_login" , new ShowPage( "login.html" ) );
		server.get( "/login" , new LoginHandler( this ) );
		server.get( "/homepage" , new HomePageHandler( this ) );
		server.get( "/show_song_info" , new SongInfoHandler( this ) );
		server.post( "/like" , new LikeHandler( this ) );
		server.post( "/dislike" , new DislikeHandler( this ) );
		server.get( "/manage_pls" , new MyPlaylistHandler( this ) ); 
		server.get( "/add_pl" , new CreatePlaylistHandler( this ) );
		server.get( "/add_playlist" , new AddPlaylistHandler( this ) );
		server.get( "/show_pl_info" , new PlInfoHandler( this ) ); 
		server.get( "/add_song" , new AddSongHandler( this ) );
		server.get( "/add_to_pl" , new AddToPlHandler( this ) );
		server.get( "/remove_song" , new RemovalHandler( this ) );
		server.get( "/all_pls" , new AllPlsHandler( this ) );
		server.get( "/add_filters" , new ShowPage( "filters.html" ) );
		server.get( "/likes_filter" , new LikesFilterHandler( this ) );
		server.get( "/year_filter" , new YearFilterHandler( this ) );
		server.get( "/artist_filter" , new ArtistFilterHandler( this ) );
		server.get( "/delete_filters" , new DeleteFiltersHandler( this ) );
		server.get( "/pass_forgot" , new ShowPage( "pass_forgot.html" ) );
		server.run();
	}
	catch( Server::Exception e )
	{ 
		std::cerr << e.getMessage() << std::endl;
	}
}

void Utunes::validate_addition( std::string email , std::string username )
{
	for( User* user : users )
		if( user->has_common_info( email , username ) )
			throw BadRequestEx();
}

std::string Utunes::print_req_pl( User* user )
{
	std::ostringstream body;
	body << user->print_pls( SELF );
	return body.str();
}

std::string Utunes::get_all_playlists()
{
	std::ostringstream body;
	for( User* user : users )
	{
		if( user == curr_user )
			body << user->print_pls( SELF );
		else
			body << user->print_pls( OTHER );
	}
	return body.str();
}

std::string Utunes::get_playlists()
{
	std::ostringstream body;
	body << print_req_pl( curr_user );
	return body.str();
}

bool Utunes::curr_user_has_pl( int pid )
{
	return curr_user->has_pl( pid );
}

void Utunes::erase_user()
{
	for( auto it = users.begin(); it != users.end(); ++it )
		if( (*it) == curr_user )
			users.erase( it );
			return;
}


Playlist* Utunes::find_playlist( int pl_id )
{
	if( pl_id == 0 )
		return curr_user->find_playlist( pl_id );
	for( User* user : users )
	{
		try
		{
			return user->find_playlist( pl_id );
		}
		catch(...){}	
	}
	throw NotFoundEx();
}

std::string Utunes::print_req_pl_songs( User* user , int pl_id )
{
	std::ostringstream body;
	Playlist* pl = find_playlist( pl_id );
	if( curr_user_has_pl( pl_id ) )
		body << user->print_pl( pl_id , pl , SELF );
	else
		body << user->print_pl( pl_id , pl , OTHER );
	return body.str();
}

std::string Utunes::get_pl_songs( int pl_id )
{
	std::ostringstream body;
	body << print_req_pl_songs( curr_user , pl_id );
	return body.str();
}

void Utunes::add_song_to_pl( std::vector <std::string> sorted_args )
{
	int m_id = atoi( sorted_args[MID_IDX].c_str() );
	Music* music = get_song( m_id );
	curr_user->add_song_to_playlist(  music , atoi( sorted_args[PID_IDX].c_str() ) );
	get_song( m_id )->add_pl_num();
	std::cout << OK;
}

void Utunes::update_similarity_map()
{
	users_similarity[curr_user] = make_user_similarity_map( curr_user );
	for( User* user : users )
		users_similarity[user][curr_user] = users_similarity[curr_user][user];
}

void Utunes::add_likes( int id )
{
	curr_user->add_song_to_playlist( get_song( id ) );
	update_similarity_map();
	get_song( id )->add_like();
	std::cout << OK;
}

void Utunes::get_comments( int id )
{
	for( Music* m : songs )
	{
		if( m->id_matches( id ) )
		{
			m->get_comments();
			return;
		}
	}
	throw NotFoundEx();
}

void Utunes::add_comment( std::vector <std::string> sorted_args )
{
	for( Music* m : songs )
	{
		if( m->id_matches( atoi( sorted_args[MID_IDX].c_str() ) ) )
		{
			m->add_comment( sorted_args[COMMENT_IDX] , atoi( sorted_args[TIME_IDX].c_str() ) , curr_user );
			std::cout << OK;
			return;
		}
	}
	throw NotFoundEx();
}

void Utunes::delete_likes( int id )
{
	curr_user->remove_song_from_playlist( get_song( id ) );
	update_similarity_map();
	get_song( id )->remove_like();
}

std::string Utunes::print_song( int id )
{
	Music* m = get_song( id );
	std::string mode = !curr_user->likes_music( m ) ? "like" : "unlike";
	std::string form_func = !curr_user->likes_music( m ) ? "/like" : "/unlike";
	return get_song( id )->print_music( mode , form_func );
}

Music* Utunes::get_song( int id )
{
	for( Music* music : songs )
		if( music->id_matches( id ) )
			return music;
	throw NotFoundEx();
}

void Utunes::logout_user()
{
	curr_user->logout();
	curr_user = nullptr;
	std::cout << OK;
}

void Utunes::delete_filters()
{
	curr_user->delete_filters();
}

void Utunes::login_user( std::vector <std::string> user_info )
{
	for( auto it = users.begin(); it != users.end(); ++it )
	{
		if( (*it)->identification_matches( user_info[email_idx] , user_info[password_idx] ) )
		{
			curr_user = (*it);
			(*it)->enter();
			return;
		}
	}
	throw BadRequestEx();
}

void Utunes::add_user_to_similarity_map()
{
	users_similarity.insert( std::pair<User*,std::map<User*,double,Ucompare> >
						( curr_user , make_user_similarity_map( curr_user ) ) );
	for( auto it = users_similarity.begin() ; it != users_similarity.end() ; ++it )
		it->second.insert( std::pair<User*,double>( curr_user , users_similarity[curr_user][it->first]) );

}

void Utunes::sign_up_user( std::string email , std::string username , std::string password )
{
	validate_addition( email , username );
	User* user = new User( email , username , password );
	user->enter();
	curr_user = user;
	add_user_to_similarity_map();
	users.insert( user );
}

User* Utunes::find_user_by_email( std::string email )
{
	return *std::find_if( users.begin() , users.end() , 
						[=]( User* user ){ return user->has_same_email( email ); } );
}

void Utunes::add_users( std::vector <std::string> likes_info )
{
	for( auto s : likes_info )
	{
		auto like_info = convert_string_to_vec( s , CSV );
		try
		{
			sign_up_user( like_info[email_l_idx] , like_info[username_l_idx] , like_info[password_l_idx] );
		}
		catch( std::exception& ex ){}
		Music* music = get_song( atoi( like_info[id_l_idx].c_str() ) );	
		music->add_like();	
		curr_user->add_song_to_playlist( music );
	}
	curr_user = nullptr;
}


std::vector <std::string> Utunes::convert_file_to_vec( std::ifstream& file )
{
	std::string line;
	std::vector <std::string> info_vec;
	while( getline( file , line ) )
		info_vec.push_back( line );
	return info_vec;
}

std::map <User*,double,Ucompare> Utunes::make_user_similarity_map( User* user )
{
	std::map <User*,double,Ucompare> similarity_percs;
	for( User* user2 : users )
		similarity_percs.insert( std::pair<User*,double>
								( user2 , (double)user->get_similarity( user2 ) / (double)songs.size() ) );
	return similarity_percs;
}

void Utunes::make_similarity_map()
{
	for( User* user : users )
		users_similarity[user] = make_user_similarity_map( user );
}

template <typename T , typename Comp>
std::multimap <double,T,Dcompare> Utunes::invert( std::map <T,double,Comp>& map_ )
{
	std::multimap <double,T,Dcompare> multi_map;
	for ( auto it = map_.begin(); it != map_.end() ; ++it )
		multi_map.insert( std::make_pair( it->second , it->first ) );
	return multi_map;
}

double Utunes::calculate_confidence( Music* music , User* curr_user )
{
	double sum = NOTHING;
	for( User* user : users )
		if( user->likes_music( music ) )
				sum += users_similarity[curr_user][user];
	return ( sum / (double)  ( users.size() - CURR_USER ) ) * HUNDRED;
}

std::map <Music*,double,Mcompare> Utunes::make_confidence_map( int count )
{
	std::map <Music*,double,Mcompare> user_recoms;
	for( Music* music : songs )
		if( !curr_user->likes_music( music ) )
			user_recoms.insert( std::pair<Music*,double>( music , calculate_confidence( music , curr_user ) ) );
	return user_recoms;
}

std::string Utunes::print_recommended( std::multimap <double,Music*,Dcompare>& user_recoms_inv , int count )
{
	std::ostringstream body;
	auto print_recom_song = [&]( std::pair<double,Music*> multi_pair )
	{
		if( count == 0 )
			return;
		body << multi_pair.second->make_table_block();
		count--;
	};
	std::for_each( user_recoms_inv.rbegin() , user_recoms_inv.rend() , print_recom_song );
	return body.str();
}

std::string Utunes::get_recommended( int count )
{
	std::map <Music*,double,Mcompare> user_recoms = make_confidence_map( count );
	std::multimap <double,Music*,Dcompare> user_recoms_inv = invert<Music*,Mcompare>( user_recoms );
	return print_recommended( user_recoms_inv , count );
}

void Utunes::print_similars( int count , const std::multimap <double,User*,Dcompare>& similarity_percs )
{
	int i = NOTHING;
	for( auto it = similarity_percs.rbegin() ; it != similarity_percs.rend() ; ++it )
	{
		if( i == count )
			break;
		if( it->second != curr_user )
		{
			std::cout << std::fixed << std::setprecision(TWO_DECIMALS) << it->first * HUNDRED << PERCENT << SPACE;
			it->second->print();
			std::cout << std::endl;
			i++;
		}
	}
}

void Utunes::get_similar_users( int count )
{
	if( count > users.size() )
		count = users.size();
	std::multimap <double,User*,Dcompare> similarity_percs = invert<User*,Ucompare>
															( users_similarity.find( curr_user )->second );
	print_similars( count , similarity_percs );
}

void Utunes::remove_headers( std::istream& songs_file , std::istream& likes_file )
{
	std::string header ;
	getline( songs_file , header );
	getline( likes_file , header );
}

void Utunes::add_songs( std::istream& songs_file , std::istream& likes_file )
{
	std::string song_info;
	while( getline( songs_file , song_info ) )
		add_music( song_info );
}

void Utunes::start( std::string songs_add , std::string likes_add )
{
	std::ifstream songs_file( songs_add.c_str() );
	std::ifstream likes_file( likes_add.c_str() );
	remove_headers( songs_file , likes_file );
	add_songs( songs_file , likes_file );
	std::vector <std::string> likes_info = convert_file_to_vec( likes_file );
	add_users( likes_info );
	make_similarity_map();
}

void Utunes::check_login()
{
	if( curr_user == nullptr )
		throw PermissionDeniedEx();
}

void Utunes::add_playlist( std::vector <std::string> pl_info )
{
	num_of_playlists++;
	curr_user->add_playlist( pl_info , num_of_playlists );
}

void Utunes::add_music( std::string song_info )
{
	std::vector <std::string> music_attributes = convert_string_to_vec( song_info , CSV );
	try
	{
		songs.insert( make_music( music_attributes ) );
	}
	catch( std::exception& ex ){}
}

int Utunes::generate_id( std::string id_type )
{
	if( id_type == MUSIC )
		return songs.size() + MID_IDX;
	return NOTHING;
}

Music* Utunes::make_music( std::vector <std::string> music_attributes )
{
	int id = atoi( music_attributes[id_idx].c_str() );
	if( music_attributes.size() < STR_ATTRIBUTES_NUM )
		throw InsufficientInfoEx();
	int release_year = atoi( music_attributes[year_idx].c_str() );
	Music* music = new Music( id , music_attributes[title_idx] , music_attributes[artist_idx] 
				, release_year , music_attributes[link_idx] );
	return music;
}

std::string Utunes::fill_table()
{
	std::set <Music*,Mcompare> filtered_songs = curr_user->filter_songs( songs );
	std::ostringstream body;
	if( filtered_songs.size() == NOTHING )
		return body.str();
	for( Music* music : filtered_songs )
		body << "<p></p>" << music->make_table_block();
	return body.str();
}

