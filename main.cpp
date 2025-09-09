#include <iostream>
#include <random>
#include <vector>

int main() {
	int size = 0;
	std::cin >> size;

	if (size < 2) {
		std::cout << "THE MAZE IS IMPOSSIBLE TO GENERATE" << std::endl;
		return 1;
	}

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distrib(1, 2);
	std::uniform_int_distribution<> distribTraps(1, 1000);
	std::uniform_int_distribution<> distribExits(1, 4);
	std::uniform_int_distribution<> distribExitIndex(0, size - 1);

	int possibleTraps = 5;
	int possibleTrapsOnPath = 2;
	int possibleTreasure = 1;


	int firstExitType = distribExits(gen);
	int firstExitIndex = distribExitIndex(gen);
	int secondExitType, secondExitIndex;

	do {
		secondExitType = distribExits(gen);
		secondExitIndex = distribExitIndex(gen);
	} while (firstExitType == secondExitType && firstExitIndex == secondExitIndex);

	std::cout << size << std::endl;


	std::vector<char> maze(size);
	std::vector<int> setNumbers(size, 0);
	std::vector<int> setSizes(size, 0);

	for (int i = 0; i < size; ++i) {
		if (i > 0) {
			int setNumber = 0;
			int setSize = 0;
			for (int j = 0; j < size; ++j) {
				if (j == 0 || maze[j] != maze[j - 1]) {
					setSizes[setNumber] = setSize;
					setNumber++;
					setSize = 1;
				} else {
					setSize++;
				}
				setNumbers[j] = setNumber;
			}
			setSizes[setNumber] = setSize;

			for (int j = 0; j < size; ++j) {
				if (setSizes[setNumbers[j]] == 1) {
					maze[j] = 'P';
				} else {
					int randomNumber = distrib(gen);
					if ((randomNumber & 1) && maze[j] != 'X') {
						setSizes[setNumbers[j]]--;
						maze[j] = 'X';
					} else {
						maze[j] = 'P';
					}
				}
			}
		} else {
			for (int j = 0; j < size; ++j) {
				int randomNumber = distrib(gen);
				maze[j] = (randomNumber & 1) ? 'X' : 'P';
			}
		}


		if (i == size - 1) {
			for (int j = 0; j < size; ++j) {
				if (maze[j] == 'X') {
					maze[j] = 'P';
				}
			}
		}


		for (int j = 0; j < size; ++j) {
			char original_char = maze[j];
			bool is_exit = false;

			if (i == 0 && (firstExitType == 1 && j == firstExitIndex || secondExitType == 1 && j == secondExitIndex)) {
				maze[j] = 'E';
				is_exit = true;
			}
			if (j == 0 && (firstExitType == 2 && i == firstExitIndex || secondExitType == 2 && i == secondExitIndex)) {
				maze[j] = 'E';
				is_exit = true;
			}
			if (j == size - 1 && (firstExitType == 3 && i == firstExitIndex || secondExitType == 3 && i ==
								secondExitIndex)) {
				maze[j] = 'E';
				is_exit = true;
			}
			if (i == size - 1 && (firstExitType == 4 && j == firstExitIndex || secondExitType == 4 && j ==
								secondExitIndex)) {
				maze[j] = 'E';
				is_exit = true;
			}

			if (!is_exit) {
				int randomNumber = distribTraps(gen);
				if (original_char == 'X' && randomNumber <= 2 && possibleTraps > 0) {
					maze[j] = 'T';
					possibleTraps--;
				} else if (original_char == 'P' && randomNumber == 1 && possibleTrapsOnPath > 0) {
					maze[j] = 'T';
					possibleTrapsOnPath--;
					possibleTraps--;
				} else if (original_char == 'P' && randomNumber == 2 && possibleTreasure > 0) {
					maze[j] = 't';
					possibleTreasure--;
				}
			}
			std::cout << maze[j];
		}
		std::cout << std::endl;
	}
	return 0;
}
