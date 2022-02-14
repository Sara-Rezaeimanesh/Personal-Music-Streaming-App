#include "commonfuncs.hpp"
#include <vector>
#include <iostream>
#include "music.hpp"
#include <string>
#include <sstream>

std::vector <std::string> convert_string_to_vec( std::string info , std::string info_type )
{
	std::string temp;
	std::istringstream info_stream( info );
	std::vector <std::string> str_attributes;
	char delim;
	if( info_type == CSV )
		delim = SEMI_COL;
	else 
		delim = SPACE;
	while( getline( info_stream , temp , delim ) )
		str_attributes.push_back( temp );
	return str_attributes;
}