#include "main.hpp"
#include <iostream>
#include "server/server.hpp"
constexpr int FIRST = 1;
constexpr int SECOND = 2;

using namespace std;

void launch_utunes();
void convert_string_to_vec();

int main(int argc, char const *argv[])
{
	launch_utunes( argv[FIRST] , argv[SECOND] );
	return 0;
}