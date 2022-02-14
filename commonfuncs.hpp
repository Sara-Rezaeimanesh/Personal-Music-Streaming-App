#ifndef COMMONFUNCS_HPP
#define COMMONFUNCS_HPP "COMMONFUNCS_HPP"
#include <vector>
#include <iostream>
#include "music.hpp"
#include <string>
#include <sstream>
constexpr char SEMI_COL = ',';
constexpr char NON_CSV[] = "non_csv";
constexpr char CSV[] = "csv";

class Music;

extern std::vector <std::string> convert_string_to_vec( std::string info , std::string info_type = NON_CSV );

#endif