#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

enum class States {bStart, bFinish, bRoad, bBlock, bDone, bPath};
struct node {int x; int y; int g; int h;};
const int neighbors[4][2]{{0,1}, {0,-1}, {1,0}, {-1,0}};

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
  trackNodes.push_back(node {start[0], start[1], 0, Heuristic(start, goal)});
  grid[start[0]][start[1]] = States::bDone;

  while(!trackNodes.empty()){
    sort(trackNodes.begin(), trackNodes.end(), Compare);
    node back = trackNodes.back();
    trackNodes.pop_back();

    grid[back.x][back.y] = States::bPath;
    if(back.x == goal[0] && back.y == goal[1])
      break;
    else
      for( int i = 0; i < 4; i++){

        int x2 = back.x + neighbors[i][0];
        int y2 = back.y + neighbors[i][1];
        int newStart[2]{x2,y2};

        if(x2<= grid.size() && y2 <= grid[0].size() &&
            grid[x2][y2]==States::bRoad){

          trackNodes.push_back(node{x2, y2, back.g + 1, Heuristic(newStart, goal)});
          grid[x2][y2] = States::bDone;
        }
      }
  }
}

void PrintBoard(vector<vector<States>> grid){

  if(grid.size() <= 0) cerr << "Board was empty" << endl;
  else{
    int row = grid.size();
    int col = grid[0].size();
    for(int i= 0; i < row; i++){
      for(int j = 0; j < col; j++){
        cout << ((grid[i][j] == States::bRoad)? " Road ": " Block ") << " ";
      }
      cout << endl;
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
  PrintBoard(grid);
  return 0;
}
