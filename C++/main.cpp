/*
To Complete This Exercise:
        Write a std::vector<vector<State>> Search function stub which takes a board grid
        and two length 2 int arrays as arguments.
        The int arrays will represent the start and goal coordinates for the search.
        The function should print ""No path found!" and
        return an empty std::vector<vector<State>>.
        The function will later return the board with a path from the start to the goal.
        In main(), call Search with a start of {0, 0} and a goal of {4, 5}.
        Store the results in the variable solution.
        Pass solution to PrintBoard.
*/

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "maze.cpp"
using std::cout;
using std::ifstream;
using std::istringstream;
using std::string;
using std::vector;
using std::abs;


// TODO: Write the Heuristic function here.
int Heuristic(int x1, int y1, int x2, int y2){
  return abs(x2 - x1) + abs(y2 - y1);
}

// TODO: Write the AddToOpen function here.
void AddToOpen(int x, int y, int g, int h,
              vector<vector<int>>& openNodes,
			         vector<vector<State>>& board){

  openNodes.push_back(vector<int>{x,y, g, h});
  board[x][y] = State::kClosed;
}

//Implementation of A* search algorithm
vector<vector<State>> Search(vector<vector<State>> grid,
                            int init[2], int goal[2]) {
  cout << "No path found!" << "\n";
  return std::vector<vector<State>> {};
}





int main() {
  int init[2]{0, 0};
  int goal[2]{4, 5};
  auto board = ReadBoardFile("1.board");
  auto solution = Search(board, init, goal);
  PrintBoard(board);
  PrintBoard(solution);
}
