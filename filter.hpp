#ifndef FILTER_PP
#define FILTER_HPP "FILTER_HPP"
#include <vector>
#include <set>
#include <string>
#include "playlist.hpp"

class Music;

class Filter{
public:
	Filter( std::string type_ ) : type( type_ ){}
	bool has_same_type( Filter* f ){ return type == f->type; }
	std::set <Music*,Mcompare> meetcriteria( std::set <Music*,Mcompare>& songs );
	virtual bool equals_ignore_case( Music* m ) = 0;

private:
	std::string type;
};

class ArtistFilter : public Filter{
public: 
	ArtistFilter( std::string type_ , std::string  artist_ ) : artist( artist_ ) , Filter( type_ ){}
	bool equals_ignore_case( Music* m );

private:
	std::string artist;
};

class LikeFilter : public Filter{
public:
	LikeFilter( std::string type_ , int minl , int maxl ) : min_like( minl ) , max_like( maxl ) , Filter( type_ ){}
	bool equals_ignore_case( Music* m );
private:
	int min_like , max_like;
	
};

class YearFilter : public Filter{
public:
	YearFilter( std::string type_ , int miny , int maxy ) : min_year( miny ) , max_year( maxy ) , Filter( type_ ){}
	bool equals_ignore_case( Music* m );
private:
	int min_year , max_year;
	
};

class AndFilter : public Filter{
public: 
	AndFilter( std::string type_ , std::vector <Filter*> filters_ ) : Filter( type_ ) , filters( filters_ ){}
	bool equals_ignore_case( Music* m );
private:
	std::vector <Filter*> filters;
};

typedef bool(*comparator)( Music* );
class FilterComparator{
public:
	FilterComparator( Filter* f_ ) : f( f_ ) {};
	bool operator()( Music* m ){ return f->equals_ignore_case( m ); }
private:
	Filter* f;
};

#endif