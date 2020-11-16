
#ifndef tileproj
#define tileproj
#pragma once
#include<iostream>
#include<vector>
#include<string>


class tiles {


public:

	tiles(std::string x, std::vector<int> y);
	~tiles();

	std::vector<int> numbers;
	std::string name;

private:
	std::vector<int>* tile;

};


#endif