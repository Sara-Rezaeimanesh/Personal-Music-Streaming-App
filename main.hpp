#ifndef MAIN_HPP
#define MAIN_HPP "MAIN_HPP"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "utunes.hpp"
#include "exceptions.hpp"
constexpr char DELIM = ' ';

void launch_utunes( std::string songs_add , std::string likes_add )
{
	std::string command;
	Utunes* utunes = new Utunes();
	utunes->start( songs_add , likes_add );
	utunes->run();
	delete utunes;
}

#endif