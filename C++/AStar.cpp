#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;

enum class States {bStart, bFinish, bRoad, bBlock, bDone, bPath};
struct node {int x; int y; int g; int h;};
const int neighbors[4][2]{{0,1}, {0,-1}, {1,0}, {-1,0}};
void PrintBoard(vector<vector<States>> grid){

  if(grid.size() <= 0) cerr << "Board was empty" << endl;
  else{
    int row = grid.size();
    int col = grid[0].size();
    for(int i= 0; i < row; i++){
      for(int j = 0; j < col; j++){
        //{bStart, bFinish, bRoad, bBlock, bDone, bPath}
        if(grid[i][j] == States::bRoad) cout << " █ ";
        else if(grid[i][j] == States::bBlock) cout << " ░ ";
        else if(grid[i][j] == States::bStart) cout << " 🚦";
        else if(grid[i][j] == States::bFinish) cout << " 🚩 ";
        else if(grid[i][j] == States::bPath) cout << " 🚶";
        else "Done  ";
        // cout << ((grid[i][j] == States::bRoad)? " Road ": " Block ") << " ";
      }
      cout << endl;
    }
  }
  cout <<endl;
}

vector<vector<States>> ConvertBoard(vector<vector<int>> board)
{
  vector<vector<States>> grid;
  if(board.size() <= 0) cerr << "Board was empty" << endl;
  else{
    int row = board.size();
    int col = board[0].size();
    for(int i= 0; i < row; i++){
      vector<States> r;
      for(int j = 0; j < col; j++){
        if(board[i][j] == 1) r.push_back(States::bBlock);
        else r.push_back(States::bRoad);
      }
      grid.push_back(r);
    }
  }
  return grid;
}

int Heuristic(int p1[2], int p2[2]){
  return abs(p1[0]-p2[0]) + abs(p1[1]-p2[1]);
}
bool Compare(node a , node b){
  return(a.g + a.h > b.g + b.h);
}
void SearchGrid(vector<vector<States>> &grid, int start[2], int goal[2]){

  vector<node> trackNodes;
  node startNode{start[0], start[1], 0, Heuristic(start, goal)};
  trackNodes.push_back(startNode);
  grid[start[0]][start[1]] = States::bDone;
  // PrintStates(grid[start[0]][start[1]]);

  while(trackNodes.size() > 0){
    sort(trackNodes.begin(), trackNodes.end(), Compare);
    node back = trackNodes.back();
    trackNodes.pop_back();

    grid[back.x][back.y] = States::bPath;
    if(back.x == goal[0] && back.y == goal[1]){
      grid[start[0]][start[1]] = States::bStart;
      grid[goal[0]][goal[1]] = States::bFinish;
      break;
    }
    else{
      for(int i = 0; i < 4; i++){

        int x2 = back.x + neighbors[i][0];
        int y2 = back.y + neighbors[i][1];
        int newStart[2]{x2,y2};

        if(x2 < grid.size() && y2 < grid[0].size() &&
            grid[x2][y2]==States::bRoad){
          node newNode{x2, y2, back.g + 1, Heuristic(newStart, goal)};
          trackNodes.push_back(newNode);
          grid[x2][y2] = States::bDone;
        }
      }
    }

  }
}




int main(){
  vector<vector<int>> board{{0,1,0,0,0,0},
                            {0,1,0,1,1,0},
                            {0,1,0,1,0,0},
                            {0,1,0,1,0,1},
                            {0,1,0,1,0,0},
                            {0,0,0,0,1,0}};




  vector<vector<States>> grid = ConvertBoard(board);
  int start[2]{0,0}, goal[2]{3,4};
  PrintBoard(grid);
  SearchGrid(grid, start, goal);
  // PrintStates(grid[start[0]][start[1]]);
  PrintBoard(grid);
  return 0;
}
