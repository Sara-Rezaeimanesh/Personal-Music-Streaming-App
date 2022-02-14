#include "handlers.hpp"
#include "utunes.hpp"
#include "exceptions.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <exception>

std::string read_file( std::string addr )
{
	std::string line;
	std::ostringstream body;
	std::ifstream input_html( addr );
	while( input_html >> line )
		body << line << std::endl;
	return body.str();
}

Response *SignupHandler::callback( Request *req ) 
{
	Response *res = new Response;
	res->setHeader( "Content-Type" , "text/html" );
	std::ostringstream body;
	try
	{
		utunes->sign_up_user( req->getQueryParam( "Email" ) , req->getQueryParam( "Username" ) , 
								req->getQueryParam( "Password" ) );
		res = Response::redirect("/signup_success");
	}
	catch( std::exception& e )
	{ 
		body << "<!DOCTYPE html>" << "<html>" 
		<< "<head>An error has ocuured while trying to sign you up!</head>"
		<<"<br/>"<<"<p>Error message: "<< e.what() <<"</p>" << "<a href = \"/\">try again</a></html>";
		res->setBody( body.str() );
	}
	return res;
}

Response *LoginHandler::callback( Request *req ) 
{
	Response *res = new Response;
	res->setHeader( "Content-Type" , "text/html" );
	try
	{
		std::vector <std::string> args{ req->getQueryParam("Email") , req->getQueryParam("Password") };
		utunes->login_user( args );
  		res = Response::redirect("/homepage");
	}
	catch( std::exception& e )
	{
		std::ostringstream error_body;
		error_body << "<!DOCTYPE html>" << "<html>" 
		<< "<head>An error has ocuured while trying to login!</head>"
		<<"<br/>"<<"<p>Error message: "<< e.what() <<"</p>" << "</html>";
		res->setBody( error_body.str() );
	}
  	return res;
}

Response *HomePageHandler::callback( Request *req ) 
{
	Response *res = new Response;
	res->setHeader( "Content-Type" , "text/html" );
	std::string line;
	std::ostringstream body;
	std::ifstream input_html( "homepage.txt" );
	while( input_html >> line )
		body << line << std::endl;
	std::string mssg = req->getQueryParam( "mssg" );
	if( mssg == "Filter applied successfully!" || mssg == "Couldn't apply filter. try again" )
		body << "<div style = \"position:relative; left:0px;\"><h2>" << mssg << "</h2></div>";
	body << utunes->fill_table() << read_file( "homepage_rest.txt" );
	res->setBody( body.str() );
  	return res;
}

Response *SongInfoHandler::callback( Request *req ) 
{
	Response *res = new Response;
	res->setHeader( "Content-Type" , "text/html" );
	std::ostringstream body;
	body << read_file( "song.txt" );
	int id = atoi( req->getQueryParam("id").c_str() );
	body << utunes->print_song( id ) << "</div></div>" 
	<< "<div style = \"position:absolute; left:140px; top:550px;\"><h1>Recommended For You</h1></div>" 
	<< std::endl << "<table class=\"container\"><thead><tr>"
	<< std::endl << "<th><h1>Title</h1></th><th><h1>Artist</h1></th>"
	<< std::endl << "<th><h1>Release Year</h1></th></tr></thead><tbody>"
	<< std::endl << utunes->get_recommended(5) << read_file( "songinfo_beta.txt" );
	res->setBody( body.str() );
  	return res;
}

Response *LikeHandler::callback( Request *req ) 
{
	Response *res = new Response;
	res->setHeader( "Content-Type" , "text/html" );
	std::ostringstream body;
	int id = atoi ( req->getQueryParam( "id" ).c_str() );
	utunes->add_likes( id );
	body << "/show_song_info?id="<<id;
	res = Response::redirect( body.str() );
  	return res;
}

Response *DislikeHandler::callback( Request *req ) 
{
	Response *res = new Response;
	res->setHeader( "Content-Type" , "text/html" );
	std::ostringstream body;
	int id = atoi ( req->getQueryParam( "id" ).c_str() );
	utunes->delete_likes( id );
	body << "/show_song_info?id=" <<id;
	res = Response::redirect( body.str() );
  	return res;
}

Response *MyPlaylistHandler::callback( Request *req ) 
{
	Response *res = new Response;
	res->setHeader( "Content-Type" , "text/html" );
	std::ostringstream body;
	std::ifstream input_html( "playlist.txt" );
	std::string line;
	while( input_html >> line )
		body << line << std::endl;
	body << utunes->get_playlists() << read_file( "plinfo_rest.txt" );
	res->setBody( body.str() );
	return res;
}

Response *AddPlaylistHandler::callback( Request *req ) 
{
	Response *res = new Response;
	res->setHeader( "Content-Type" , "text/html" );
	try
	{
		std::vector <std::string> pl_info{ req->getQueryParam( "Name" )  ,  req->getQueryParam( "Privacy" ) };
		if( req->getQueryParam( "Privacy" ) != "public" && req->getQueryParam( "Privacy" ) != "private" )
			throw BadRequestEx();
		utunes->add_playlist( pl_info );
		res = Response::redirect( "/manage_pls" );
	}
	catch( std::exception& ex )
	{
		std::ostringstream error_body;
		error_body << "<!DOCTYPE html>" << "<html>" 
		<< "<head>Sorry! Couldn't add playlist. try again</head>"
		<<"<br/>"<<"<p>Error message: "<< ex.what() <<"</p>" << "</html>";
		res->setBody( error_body.str() );		
	}
	return res;
}

Response *CreatePlaylistHandler::callback( Request *req ) 
{
	Response *res = new Response;
	res->setHeader( "Content-Type" , "text/html" );
	std::ostringstream body;
	std::ifstream input_html( "add_pl.txt" );
	std::string line;
	while( input_html >> line )
		body << line << std::endl;
	res->setBody( body.str() );
	return res;
}

Response *PlInfoHandler::callback( Request *req ) 
{
	Response *res = new Response;
	res->setHeader( "Content-Type" , "text/html" );
	std::ostringstream body;
	std::ifstream input_html( "show_pl.txt" );
	std::string line;
	while( input_html >> line )
		body << line << std::endl;
	int id = atoi( req->getQueryParam( "id" ).c_str() );
	body << utunes->get_pl_songs( id ) << "</tbody></table>"
		<< std::endl;
	if( utunes->curr_user_has_pl( id ) )
		body << "</div><div style = \"position:relative; left:130px; top:100px\" >"
		<< "<a href=\"/add_song?id="
		<< req->getQueryParam( "id" ) << "\"><p><span class=\"bg\"></span><span class=\"base\">"
		<< "</span><span class=\"text\">Add Song</span></p></a></div></div></div>";
	body << "</div>" << "<form action=\"/homepage\" method=\"get\">"
	<< "<div style = \"position:absolute; left:70px; top:800px;\">"
	<< "<button type=\"submit\">Home</button><br/></div></form></html>";
	res->setBody( body.str() );
	return res;
}

Response *AddSongHandler::callback( Request *req ) 
{
	Response *res = new Response;
	res->setHeader( "Content-Type" , "text/html" );
	std::ostringstream body;
	body << read_file( "add_song.txt" );
	res->setSessionId( req->getQueryParam( "id" ) );
	res->setBody( body.str() );
	return res;
}

Response *AddToPlHandler::callback( Request *req ) 
{
	Response *res = new Response;
	res->setHeader( "Content-Type" , "text/html" );
	std::string link = "/show_pl_info?id=" + req->getSessionId();
	std::ostringstream body;
	std::vector <std::string> args{ req->getSessionId() , req->getQueryParam( "sid" ) };
	utunes->add_song_to_pl( args );
	res = Response::redirect( link );
	return res;
}

Response *RemovalHandler::callback( Request *req ) 
{
	Response *res = new Response;
	res->setHeader( "Content-Type" , "text/html" );
	std::string link = "/show_pl_info?id=" + req->getSessionId();
	std::ostringstream body;
	std::vector <std::string> args{ req->getSessionId() , req->getQueryParam( "id" ) };
	utunes->delete_song_from_pl( args );
	res = Response::redirect( link );
	return res;
}

Response *AllPlsHandler::callback( Request *req ) 
{
	Response *res = new Response;
	res->setHeader( "Content-Type" , "text/html" );
	std::ostringstream body;
	body << read_file( "playlist.txt" );
	body << utunes->get_all_playlists() << read_file( "allpl_rest.txt" );
	res->setBody( body.str() );
	return res;
}

Response *FilterHandler::callback( Request *req )
{
	Response *res = new Response;
	res->setHeader( "Content-Type" , "text/html" );
	try
	{
		std::ostringstream filter_string;
		filter_string << make_filter_string( req );
		utunes->add_filters( filter_string.str() ); 
		res = Response::redirect( "/homepage?mssg=Filter applied successfully!" );
	}
	catch( std::exception& e )
	{
		res = Response::redirect( "/homepage?mssg=Couldn't apply filters :( try again" );
	}
	return res;
}

std::string LikesFilterHandler::make_filter_string( Request *req ) 
{
	std::ostringstream filter_string;
	filter_string << "min_like " << req->getQueryParam( "min_like" ) << " max_like "
	<< req->getQueryParam( "max_like" );
	return filter_string.str();
}
 
std::string YearFilterHandler::make_filter_string( Request *req ) 
{
	std::ostringstream filter_string;
	filter_string << "min_year " << req->getQueryParam( "min_year" ) << " max_year "
	<< req->getQueryParam( "max_year" );
	return filter_string.str();

}

std::string ArtistFilterHandler::make_filter_string( Request *req ) 
{
	std::ostringstream filter_string;
	filter_string << "artist " << req->getQueryParam( "artist" );
	return filter_string.str();
}

Response *DeleteFiltersHandler::callback( Request *req ) 
{
	Response *res = new Response;
	res->setHeader( "Content-Type" , "text/html" );
	utunes->delete_filters(); 
	res = Response::redirect( "/homepage" );
	return res;
}


