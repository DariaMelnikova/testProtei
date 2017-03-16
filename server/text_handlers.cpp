#include "text_handlers.hpp"
#include <regex>
#include <iostream>

using namespace std;

int get_sum (map<int, int> & d_map)
{
	int sum = 0;
	for (auto pair : d_map)
		sum += pair.first * pair.second;
	return sum;
}

void show_all (map<int, int> & d_map) 
{
	map<int, int>::reverse_iterator it;
	for (it = d_map.rbegin(); it != d_map.rend(); ++it)
		for (int i = 0; i < it->second; ++i)
			cout << it->first;
	cout << endl;
}

int show_min (map<int, int> & d_map) //Returns -1 if message contains no digit
{
	map<int, int>::iterator it;
	for (it = d_map.begin(); it != d_map.end(); ++it)
		if (it->second != 0)
			return it->first;
	return -1;
}

int show_max (map<int, int> & d_map) //Returns -1 if message contains no digit
{
        map<int, int>::reverse_iterator it;
        for (it = d_map.rbegin(); it != d_map.rend(); ++it)
                if (it->second != 0)
                        return it->first;
        return -1;
}

map<int, int> handler(string * buf)
{
	regex dig("[^(0-9)]+");
	regex_replace(*buf, dig, "");

	map<int, int> d_map;	

	for (char ch : *buf)
		if (ch >= '0' && ch <= '9')
			d_map[ch - '0']++;
	
	return d_map;
}

