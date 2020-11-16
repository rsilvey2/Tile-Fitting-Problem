#include <iostream>
#include <vector>
#include <array>
#include"Header.h"
#include <vector>
#include<string>
#include <tuple>
#include <algorithm>



tiles::tiles(std::string x, std::vector<int> y) {
	name = x;
	numbers = y;
}

tiles::~tiles() {                                       
	delete[] tile;
}

bool doesfit(int a, int b) {
	if ((a == 1) && ((b == 4) || (b == 7))) {
		return true;
	}
	if ((a == 2) && ((b == 3) || (b == 8))) {
		return true;
	}
	if ((a == 3) && ((b == 1) || (b == 2))) {
		return true;
	}
	if ((a == 4) && ((b == 1) || (b == 2))) {
		return true;
	}
	if ((a == 5) && ((b == 3) || (b == 8))) {
		return true;
	}
	if ((a == 6) && ((b == 4) || (b == 7))) {
		return true;
	}
	if ((a == 7) && ((b == 5) || (b == 6))) {
		return true;
	}
	if ((a == 8) && ((b == 5) || (b == 6))) {
		return true;
	}
	return false;
}

int shift(tiles a, tiles b) {
	int shift = 0;
	int fits;
	while (shift < 8) {
		fits = 0;
		for (int i = 0; i < 8; i++) {
			if (doesfit(a.numbers[i], b.numbers[(i + shift)%8])) {
				fits++;
			}
		}
		if (fits == 8) {
			return shift;
		}
		else {
			shift++;
		}
	}
	return -1;
}

tiles createshifted(tiles a, int shift) {
	if (shift == -1) {
		shift = 0;
		a.name = a.name + " ERROR";
	}
	tiles shifted(a.name, {0,0,0,0,0,0,0,0});
	for (int i = 0; i < 8; i++) {
		shifted.numbers[i] = a.numbers[(i + shift) % 8];
	}
	return shifted;
}

void print(tiles a) {
	std::cout << a.name << " - ";
	for (int i = 0; i < 8; i++) {
		std::cout << a.numbers[i];
	}
	std::cout << std::endl;
}

std::tuple<tiles, int, std::vector<tiles>> scan(std::vector<tiles> newtiles) {
	tiles E("E", {0,0,0,0,0,0,0,0});
	int matches = 0;
	for (int i = 0; i < 8; i++) {
		std::vector<int> matched(1);
		for (int j = 0; j < 8; j++) {
			for (int k = j+1; k < 8; k++) {
				if ((newtiles[j].numbers[i] == newtiles[k].numbers[i]) && !(std::find(matched.begin(),matched.end(),k) != matched.end())){
					E.numbers[i] = E.numbers[i] + 1;
					matches++;
					matched.push_back(k);
				}
			}
		}
	}
	return {E, matches, newtiles};
}

std::vector<tiles> stackSequences(std::vector<tiles> tileorder) {
	tiles Z("Z", {0,0,0,0,0,0,0,0});
	std::vector<tiles> newtiles = {Z,Z,Z,Z,Z,Z,Z,Z};
	newtiles[0] = tileorder[0];
	for (int i = 1; i < 8; i++) {
		newtiles[i] = createshifted(tileorder[i], shift(newtiles[i - 1], tileorder[i]));
	}
	return newtiles;
}

void miniprintmatches(std::vector<int> matches) {
	for (int i = 0; i < matches.size() - 1; i++) {
		std::cout << matches[i] << ", ";
	}
	std::cout << matches[matches.size() -1];
}

void fullprintscan(std::vector<tiles> vectoroftiles) {
	for (int i = 0; i < 8; i++) {
		print(vectoroftiles[i]);
	}
	std::tuple<tiles, int, std::vector<tiles>> outs = scan(vectoroftiles);
	std::cout << "------------" << "\n";
	print(std::get<0>(outs));
	std::cout << "------------" << "\n";
	std::cout << std::get<1>(outs) << " Total Column Matches\n";
	std::cout << "-----------------------" << "\n";
}

void multifullprintscan(std::tuple<std::vector<std::vector<tiles>>, std::vector<std::vector<int>>> answer_matches) {
	std::cout << "=======================" << "\n";
	for (int j = 0; j < std::get<0>(answer_matches).size(); j++) {
		std::cout << "Answer " << j + 1 << ": Same as ";
		if (!std::get<1>(answer_matches)[j].empty()) {
			miniprintmatches(std::get<1>(answer_matches)[j]);
		}
		std::cout << "\n";
		std::cout << "------------" << "\n";
		for (int i = 0; i < 8; i++) {
			print(std::get<0>(answer_matches)[j][i]);
		}
		std::tuple<tiles, int, std::vector<tiles>> outs = scan(std::get<0>(answer_matches)[j]);
		std::cout << "------------" << "\n";
		print(std::get<0>(outs));
		std::cout << "------------" << "\n";
		std::cout << std::get<1>(outs) << " Total Column Matches\n";
		std::cout << "=======================" << "\n";
	}
}

std::vector<std::vector<tiles>> computeanswer(std::vector<tiles> input) {
	std::vector<std::vector<tiles>> answers;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				for (int l = 0; l < 4; l++) {
					for (int m = 0; m < 4; m++) {
						for (int n = 0; n < 4; n++) {
							for (int o = 0; o < 4; o++) {
								for (int p = 0; p < 4; p++) {
									std::vector<tiles> tileorder = { input[i],input[j],input[k],input[l],input[m],input[n],input[o],input[p] };
									std::tuple<tiles, int, std::vector<tiles>> tuple = scan(stackSequences(tileorder));
									if (std::get<1>(tuple) == 0) {
										answers.push_back(std::get<2>(tuple));
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return answers;
}

std::tuple<std::vector<std::vector<tiles>>,std::vector<std::vector<int>>> compareanswers(std::vector<std::vector<tiles>> answers) {
	int matches;
	std::vector<std::vector<int>> returnmatches(16);
	for (int i = 0; i < answers.size(); i++) {
		for (int j = 0; j < answers.size(); j++) {
			for (int k = 0; k < 8; k++) {
				matches = 0;
				for (int l = 0; l < 8; l++) {
					if (answers[i][l].name == answers[j][(l+k) % 8].name) {
						matches++;
					}
					if (matches == 8) {
						returnmatches[i].push_back(j+1);
					}
				}
			}
		}
	}
	return {answers, returnmatches};
}

int main() {

	tiles A("A",{2,8,6,4,3,5,7,1});
	tiles B("B",{2,1,3,5,7,8,6,4});
	tiles C("C",{2,8,6,5,7,1,3,4});
	tiles D("D",{2,8,7,1,3,5,6,4});

	std::vector<tiles> tileorder = {A,A,B,D,D,C,B,C};
	std::vector<tiles> input = { A,B,C,D };

	std::cout << "\n\n\n\n\n";

	multifullprintscan(compareanswers(computeanswer(input)));


	return 0;
}