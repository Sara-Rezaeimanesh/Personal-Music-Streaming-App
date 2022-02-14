#ifndef UTUNES_HPP
#define UTUNES_HPP "UTUNES_HPP"
#include "music.hpp"
#include "user.hpp"
#include "commonfuncs.hpp"
#include "server/server.hpp"
#include "playlist.hpp"
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <set>
constexpr int NONE = 0;

struct Ucompare{
	bool operator()( User* const& user1 , User* const& user2 )
	{
		return user1->username_is_smaller( user2 );
	}
};

struct Dcompare{
	bool operator()( double d1 , double d2 )
	{
		return d1 <= d2;
	}
};

class CommandProcr;
class Music;
class Playlist;

class Utunes{
public:
	Utunes(){ curr_user = nullptr; num_of_playlists = NONE; }
	~Utunes();
	void start( std::string songs_add , std::string likes_add );
	void print_musics();
	void run();
	void sign_up_user( std::string email , std::string username , std::string password );
	CommandProcr* get_command_procr( std::string command_type , 
											std::istream& command_stream );
	void add_likes( int id );
	void add_song_to_pl( std::vector <std::string> sorted_args );
	void logout_user();
	void login_user( std::vector <std::string> user_info );
	void print_all_songs();
	std::string print_song( int id );
	void add_playlist( std::vector <std::string> pl_info );
	void get_likes();
	void delete_likes( int id );
	std::string get_playlists();
	std::string get_pl_songs( int pl_id );
	void print_users();
	void delete_song_from_pl( std::vector <std::string> sorted_args );
	void add_comment( std::vector <std::string> sorted_args );
	void get_comments( int id );
	void check_login();
	void add_filters( std::string filter_args );
	void delete_filters();
	void get_similar_users( int count );
	std::string print_user(){ return curr_user->print();}
	bool curr_user_has_pl( int pid );
	std::string get_all_playlists();
	std::string get_recommended( int count );
	User* find_user_by_email( std::string email );
	std::string fill_table();
	Playlist* find_playlist( int pl_id );
private:
	std::string print_req_pl( User* user );
	std::vector <std::string> convert_file_to_vec( std::ifstream& likes_file );
	void add_users( std::vector <std::string> likes_info );
	std::string print_req_pl_songs( User* user , int pl_id );
	void erase_user();
	void validate_addition( std::string email , std::string username );
	void add_music( std::string song_info );
	void make_similarity_map();
	void add_user_to_similarity_map();
	void update_similarity_map();
	void print_similars( int count , const std::multimap <double,User*,Dcompare>& similarity_percs );
	void remove_headers( std::istream& songs_file , std::istream& likes_file );
	void add_songs( std::istream& songs_file , std::istream& likes_file );
	std::string print_recommended( std::multimap <double,Music*,Dcompare>& user_recoms_inv , int count );
	Music* make_music( std::vector <std::string> song_info );
	Music* get_song( int id );
	double calculate_confidence( Music* music , User* curr_user );
	int generate_id( std::string id_type );
	std::map <User*,double,Ucompare> make_user_similarity_map( User* user );
	std::map <Music*,double,Mcompare> make_confidence_map( int count );
	template <typename T , typename Comp>
	std::multimap <double,T,Dcompare> invert( std::map <T,double,Comp>& map_ );
	template <typename T>
	T* make_procr();
	std::vector <int> fill_likes( User* user );
	int num_of_playlists;
	User* curr_user;
	std::map <User*,std::map <User*,double,Ucompare>,Ucompare> users_similarity;
    std::set<Music*,Mcompare> songs;
    std::set<User*,Ucompare> users;
};

#endif