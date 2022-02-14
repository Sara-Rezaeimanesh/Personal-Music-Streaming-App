#ifndef HANDLERS_HPP
#define HANDLERS_HPP "HANDLERS_HPP"
#include "server/server.hpp"

class Utunes;

class SignupHandler : public RequestHandler {
public:
	SignupHandler( Utunes* utunes_ ) : utunes( utunes_ ){};
	Response *callback(Request *);
private:
	Utunes* utunes;
};

class LoginHandler : public RequestHandler {
public:
	LoginHandler( Utunes* utunes_ ) : utunes( utunes_ ){};
	Response *callback(Request *);
private:
	Utunes* utunes;
};

class HomePageHandler : public RequestHandler {
public:
	HomePageHandler( Utunes* utunes_ ) : utunes( utunes_ ){};
	Response *callback(Request *);
private:
	Utunes* utunes;
};

class SongInfoHandler : public RequestHandler {
public:
	SongInfoHandler( Utunes* utunes_ ) : utunes( utunes_ ){};
	Response *callback(Request *);
private:
	Utunes* utunes;
	static bool song_liked;
};

class LikeHandler : public RequestHandler {
public:
	LikeHandler( Utunes* utunes_ ) : utunes( utunes_ ){};
	Response *callback(Request *);
private:
	Utunes* utunes;
	static bool song_liked;
};

class DislikeHandler : public RequestHandler {
public:
	DislikeHandler( Utunes* utunes_ ) : utunes( utunes_ ){};
	Response *callback(Request *);
private:
	Utunes* utunes;
	static bool song_liked;
};

class MyPlaylistHandler : public RequestHandler {
public:
	MyPlaylistHandler( Utunes* utunes_ ) : utunes( utunes_ ){};
	Response *callback(Request *);
private:
	Utunes* utunes;
	static bool song_liked;
};

class CreatePlaylistHandler : public RequestHandler {
public:
	CreatePlaylistHandler( Utunes* utunes_ ) : utunes( utunes_ ){};
	Response *callback(Request *);
private:
	Utunes* utunes;
};

class AddPlaylistHandler : public RequestHandler {
public:
	AddPlaylistHandler( Utunes* utunes_ ) : utunes( utunes_ ){};
	Response *callback(Request *);
private:
	Utunes* utunes;
};

class PlInfoHandler : public RequestHandler {
public:
	PlInfoHandler( Utunes* utunes_ ) : utunes( utunes_ ){};
	Response *callback(Request *);
private:
	Utunes* utunes;
};

class AddSongHandler : public RequestHandler {
public:
	AddSongHandler( Utunes* utunes_ ) : utunes( utunes_ ){};
	Response *callback(Request *);
private:
	Utunes* utunes;
};

class AddToPlHandler : public RequestHandler {
public:
	AddToPlHandler( Utunes* utunes_ ) : utunes( utunes_ ){};
	Response *callback(Request *);
private:
	Utunes* utunes;
};

class RemovalHandler : public RequestHandler {
public:
	RemovalHandler( Utunes* utunes_ ) : utunes( utunes_ ){};
	Response *callback(Request *);
private:
	Utunes* utunes;
};

class AllPlsHandler : public RequestHandler {
public:
	AllPlsHandler( Utunes* utunes_ ) : utunes( utunes_ ){};
	Response *callback(Request *);
private:
	Utunes* utunes;
};

class FilterHandler : public RequestHandler{
public:
	FilterHandler( Utunes* utunes_ ) : utunes( utunes_ ){};
	virtual std::string make_filter_string( Request * ) = 0;
	Response *callback(Request *);
protected:
	Utunes* utunes;	
};

class LikesFilterHandler : public FilterHandler {
public:
	LikesFilterHandler( Utunes* utunes_ ) : FilterHandler( utunes_ ){};
	std::string make_filter_string( Request * );
};

class  YearFilterHandler : public FilterHandler {
public:
	YearFilterHandler( Utunes* utunes_ ) : FilterHandler( utunes_ ){};
	std::string make_filter_string( Request * );
};

class  ArtistFilterHandler : public FilterHandler {
public:
	ArtistFilterHandler( Utunes* utunes_ ) : FilterHandler( utunes_ ){};
	std::string make_filter_string( Request * );
};

class  DeleteFiltersHandler : public RequestHandler {
public:
	DeleteFiltersHandler( Utunes* utunes_ ) : utunes( utunes_ ){};
	Response *callback(Request *);
private:
	Utunes* utunes;
};

#endif