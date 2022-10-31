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
#include <algorithm>
#include "maze.cpp"
using std::cout;
using std::ifstream;
using std::istringstream;
using std::string;
using std::vector;
using std::abs;
using std::sort;

/**
 * Compare the F values of two cells. f = h+ g
 */
// bool Compare(const vector<int> a, const vector<int> b) {
//   int f1 = a[2] + a[3]; // f1 = g1 + h1
//   int f2 = b[2] + b[3]; // f2 = g2 + h2
//   return f1 > f2;
// }
bool Compare(vector<int> a, vector<int> b){
  return (a[3] + a[4] > b[3] + b[4]);
}

/**
 * Sort the two-dimensional vector of ints in descending order.
 */
void CellSort(vector<vector<int>> *v) {
  sort(v->begin(), v->end(), Compare);
}

// TODO: Write the Heuristic function here.
int Heuristic(int x1, int y1, int x2, int y2){
  return abs(x2 - x1) + abs(y2 - y1);
}

// TODO: Write CheckValidCell here. Check that the
// cell is on the grid and not an obstacle (i.e. equals kEmpty).
bool CheckValidCell(int x, int y,vector<vector<State>>&grid){
 return ((x >= 0 && x < grid.size()) &&
         (y >= 0 && y < grid[0].size())&&
          grid[x][y] == State::kEmpty);
}

// TODO: Write the AddToOpen function here.
void AddToOpen(int x, int y, int g, int h,
              vector<vector<int>>& openNodes,
			         vector<vector<State>>& board){

  openNodes.push_back(vector<int>{x,y, g, h});
  board[x][y] = State::kClosed;
}
// directional deltas
const int delta[4][2]{{-1, 0}, {0, -1}, {1, 0}, {0, 1}};

// void ExpandNeighbors(vector<int>&node , int goal[2], vector<vector<int>>&open,
//                      vector<vector<State>>&grid){
//   int x = node[0], y=node[1], g = node[2]; // increament g
//   for(int i = 0; i < 4; i++){
//     if(CheckValidCell(x + delta[i][0],y + delta[i][1], grid)){
//       int h = Heuristic(x + delta[i][0],y + delta[i][1], goal[0], goal[1]); // calcualte h
//       AddToOpen(x + delta[i][0],y + delta[i][1], g + 1, h, open, grid);
//     }
//   }
// }

/**
 * Expand current nodes's neighbors and add them to the open list.
 */
void ExpandNeighbors(const vector<int> &current, int goal[2], vector<vector<int>> &openlist, vector<vector<State>> &grid) {
  // Get current node's data.
  int x = current[0];
  int y = current[1];
  int g = current[2];

  // Loop through current node's potential neighbors.
  for (int i = 0; i < 4; i++) {
    int x2 = x + delta[i][0];
    int y2 = y + delta[i][1];

    // Check that the potential neighbor's x2 and y2 values are on the grid and not closed.
    if (CheckValidCell(x2, y2, grid)) {
      // Increment g value and add neighbor to open list.
      int g2 = g + 1;
      int h2 = Heuristic(x2, y2, goal[0], goal[1]);
      AddToOpen(x2, y2, g2, h2, openlist, grid);
    }
  }
}


//Implementation of A* search algorithm
vector<vector<State>> Search(vector<vector<State>> grid,
                            int init[2], int goal[2]) {

// Create the vector of open nodes.
vector<vector<int>> open {};

// Initialize the starting node.
int x = init[0];
int y = init[1];
int g = 0;
int h = Heuristic(x, y, goal[0],goal[1]);
AddToOpen(x, y, g, h, open, grid);

while(!open.empty()){
  // TODO: Sort the open list using `CellSort`, and get the current node.
  CellSort(&open);
  auto node = open.back();
  open.pop_back();
  int x1 = node[0];
  int y1 = node[1];
  grid[x1][y1] = State::kPath;
  if(x1 == goal[0] && y1 == goal[1]) return grid;
  // else ExpandNeighbors();
}

// We've run out of new nodes to explore and haven't found a path.
cout << "No path found!" << "\n";
return std::vector<vector<State>>{};
}

#include "test.cpp"
int main() {
  int init[2]{0, 0};
  int goal[2]{4, 5};
  auto board = ReadBoardFile("1.board");
  auto solution = Search(board, init, goal);
  PrintBoard(solution);
  // Tests
  TestHeuristic();
  TestAddToOpen();
  TestCompare();
  TestSearch();
  TestCheckValidCell();
  TestExpandNeighbors();
}
