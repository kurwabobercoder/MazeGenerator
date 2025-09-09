#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <tuple>
#include <stdexcept>

struct Point {
    int row, col;
};

struct PathState {
    int row, col, traps;
};

bool isValid(int row, int col, int size) {
    return (row >= 0 && row < size && col >= 0 && col < size);
}

bool isOnBorder(int row, int col, int size) {
    return row == 0 || row == size - 1 || col == 0 || col == size - 1;
}

void printMaze(const std::vector<std::vector<char>>& maze, int size) {
    std::cout << "--- Invalid Maze Layout ---" << std::endl;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            std::cout << maze[i][j];
        }
        std::cout << std::endl;
    }
    std::cout << "---------------------------" << std::endl;
}

void findSpecialPoints(const std::vector<std::vector<char>>& maze, int size, std::vector<Point>& entrances, Point& treasure) {
    bool treasureFound = false;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (maze[i][j] == 'E') {
                entrances.push_back({i, j});
            } else if (maze[i][j] == 't') {
                treasure = {i, j};
                treasureFound = true;
            }
        }
    }
    if (!treasureFound) {
        treasure = {-1, -1};
    }
}

bool findPath(const std::vector<std::vector<char>>& maze, int size, Point start, Point end, bool checkTraps) {
    std::vector<std::vector<bool>> visited(size, std::vector<bool>(size, false));
    std::queue<PathState> q;

    q.push({start.row, start.col, 0});
    visited[start.row][start.col] = true;

    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};

    while (!q.empty()) {
        auto [row, col, traps] = q.front();
        q.pop();

        if (row == end.row && col == end.col) {
            return true;
        }

        for (int i = 0; i < 4; ++i) {
            int newRow = row + dr[i];
            int newCol = col + dc[i];

            if (isValid(newRow, newCol, size) && !visited[newRow][newCol]) {
                char cell = maze[newRow][newCol];
                if (cell != 'X') {
                    int newTraps = traps;
                    if (cell == 'T') {
                        newTraps++;
                    }

                    if (checkTraps && newTraps > 2) {
                        continue;
                    }

                    visited[newRow][newCol] = true;
                    q.push({newRow, newCol, newTraps});
                }
            }
        }
    }

    return false;
}

int main() {
    try {
        int size;
        if (!(std::cin >> size)) {
            std::cerr << "Test failed: Could not read size from input." << std::endl;
            return 1;
        }
        std::string line;
        std::getline(std::cin, line);

        std::vector<std::vector<char>> maze(size, std::vector<char>(size));
        for (int i = 0; i < size; ++i) {
            if (!(std::getline(std::cin, line) && line.size() >= size)) {
                std::cerr << "Test failed: Could not read row " << i << " or row is too short." << std::endl;
                return 1;
            }
            for (int j = 0; j < size; ++j) {
                maze[i][j] = line[j];
            }
        }

        std::vector<Point> entrances;
        Point treasure;
        findSpecialPoints(maze, size, entrances, treasure);

        if (entrances.size() != 2) {
            printMaze(maze, size);
            std::cout << "Test failed: Invalid number of entrances/exits (" << entrances.size() << " found)." << std::endl;
            return 0;
        }

        if (!isOnBorder(entrances[0].row, entrances[0].col, size) || !isOnBorder(entrances[1].row, entrances[1].col, size)) {
            printMaze(maze, size);
            std::cout << "Test failed: Entrance/Exit not on the border." << std::endl;
            return 0;
        }

        if (!findPath(maze, size, entrances[0], entrances[1], true)) {
            printMaze(maze, size);
            std::cout << "Test failed: No valid path from entrance to exit or path has too many traps." << std::endl;
            return 0;
        }

        if (treasure.row != -1 && !findPath(maze, size, entrances[0], treasure, false)) {
            printMaze(maze, size);
            std::cout << "Test failed: Treasure is unreachable." << std::endl;
            return 0;
        }

        std::cout << "Test passed!" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Test failed: An unexpected runtime error occurred: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
