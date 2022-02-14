#include "filter.hpp"
#include "music.hpp"
#include "playlist.hpp"
#include <iostream>
#include <string>
#include <set>
#include <algorithm>
constexpr int NONE = 0;


std::set <Music*,Mcompare> Filter::meetcriteria( std::set <Music*,Mcompare>& songs )
{
	std::set <Music*,Mcompare> filtered_songs;
	for( Music* m : songs )
		if( equals_ignore_case( m ) )
			filtered_songs.insert( m );
	//filtered_songs.insert( *std::find_if( songs.begin() , songs.end() , FilterComparator( this ) ) );
	return filtered_songs;
}

bool ArtistFilter::equals_ignore_case( Music* m )
{
	return artist == m->get_artist();
}

bool LikeFilter::equals_ignore_case( Music* m )
{
	return m->likes_match( min_like , max_like );
}

bool AndFilter::equals_ignore_case( Music* m )
{
	int meets = NONE;
	int runs = 0;
	std::for_each( filters.begin() , filters.end() , 
					[&]( Filter* filter ){ if( filter->equals_ignore_case( m ) ) meets++; runs++; } );
	std::cout<<runs;
	return meets == filters.size();
}

bool YearFilter::equals_ignore_case( Music* m )
{
	return m->year_match( min_year , max_year );
}