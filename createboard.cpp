//
// Created by dante on 5/2/2022.
//

#include <iostream>
#include "createboard.h"
#include <map>
#include <bits/stdc++.h>
#include <algorithm>
#include <random>

using namespace std;

class BattleShip::ships{
 public:
  int getStatus(char c){
    return allships.at(c);
  }
  void hit(char s) {
    allships.at(s)--;
  }
  bool keyExists(char s){
    if(allships.count(s) == 1){
      return true;
    } else{
      return false;
    }
  }
  void addShip(string const &s){
    allships.insert(pair<char, int>(s[0],((int)s[2]) - 48));
  }
 private:
  map<char,int> allships;
};

class BattleShip::board {
 public:
  board(){
    cols = 0;
    rows = 0;
  }
  void setSize(int row_comp, int col_comp) {
    rows = row_comp;
    cols = col_comp;
    for(int i = 0; i < rows; i++) {
      vector<char> tempvec;
      for(int j = 0; j<  cols; j++) {
        tempvec.push_back('*');
      }
      iboard.push_back(tempvec);
    }
  }
  int getCols() const{
    return  cols;
  }
  int getRows() const{
    return  rows;
  }
  void printFiring(){
    cout << "  ";
    for(int i = 0; i < cols; i++) {
      cout << i << " ";
    }
    cout << endl;
    for(int i = 0; i < rows; i++){
      cout << i << " ";
      for(int j = 0; j < cols; j++){
        if((iboard[i][j] != '*') && (iboard[i][j] != 'X') && (iboard[i][j] != 'O')){
          cout << "* ";
        } else {
          cout << iboard[i][j] << " ";
        }
      }
      cout << endl;
    }
  }
  void printPlacement(){
    cout << "  ";
    for(int i = 0; i < cols; i++) {
      cout << i << " ";
    }
    cout << endl;
    for(int i = 0; i < rows; i++){
      cout << i << " ";
      for(int j = 0; j < cols; j++){
        cout << iboard[i][j] << " ";
      }
      cout << endl;
    }
  }
  void add(int y, int x, char iter){
    iboard[y][x] = iter;
  }
  char currPos(int y, int x){
    return iboard[y][x];
  }
 private:
  int cols;
  int rows;
  vector<vector<char>> iboard;
};

class BattleShip::Player {
 public:
  Player(){
    Numships = 0;
  }
  void setName(string const &user){
    name = user;
  }
  void setShips(int total){
    Numships = total;
  }
  string getName(){
    return name;
  }
  int getShips() const{
    return Numships;
  }
  void shipSunk(){
    Numships--;
  }

  void static validateFire(board &board, ships &ships, int &y, int &x, string const &input){
    if(input.size() > 3){
      x = INT_MAX;
      y = INT_MAX;
    } else {
      y = input[0] - 48;
      x = input[2] - 48;
      if ((x > board.getCols()) || (board.currPos(y, x) == 'X') || (board.currPos(y, x) == 'O') || (y > board.getRows())) {
        x = INT_MAX;
        y = INT_MAX;
      }
    }
  }
  void gameStatus(Player &defenseplayer, board &board, ships &ships, int y, int x, bool &gameContinues, char c){
    if(board.currPos(y,x) == 'O'){
      cout << "Missed." << endl;
    } else {
      cout << getName() << " hit " << defenseplayer.getName() << "'s " << c << "!" << endl;
      if(ships.getStatus(c) == 0){
        cout << getName() << " destroyed " << defenseplayer.getName() << "'s " << c << "!" << endl;
        if(defenseplayer.getShips() == 0){
          cout << endl << getName() << " won the game!" << endl;
          gameContinues = false;
        }
      }
      board.add(y, x, 'X');
    }
  }
  void printBoardsAfterUpdate(board &primaryBoard, board &secondaryBoard){
    cout << getName() << "'s Firing Board" << endl;
    secondaryBoard.printFiring();
    cout << endl << endl;
    cout << getName() << "'s Placement Board" << endl;
    primaryBoard.printPlacement();
  }
  void static hitStatus(Player &defenseplayer, board &board, ships &ships, int y, int x){
    if(board.currPos(y,x) == '*'){
      board.add(y, x, 'O');
    } else {
      ships.hit(board.currPos(y,x));
      if(ships.getStatus(board.currPos(y,x)) == 0){
        defenseplayer.shipSunk();
      }
      board.add(y, x, 'X');
    }
  }
  void static placeShip(board &pboard, ships &pShips, char iter, int y, int x, int direction){
    if(direction == 0){ // horizontal
      for(int i = x; i < (x + pShips.getStatus(iter)); i++){
        pboard.add(y, i, iter);
      }
    }else{ // vertical
      for(int i = y; i < (y + pShips.getStatus(iter)); i++){
        pboard.add(i, x, iter);
      }
    }
  }
  bool static shipThere(board &pboard, ships &pShips, char iter, int y, int x, int direction){
    bool status = false;
    if(direction == 0){ // horizontal
      for(int i = x; i < (x + pShips.getStatus(iter)); i++){
        if(pboard.currPos(y, i) != '*'){
          status = true;
        }
      }
    }else{ // vertical
      for(int i = y; i < (y + pShips.getStatus(iter)); i++){
        if(pboard.currPos(i, x) != '*'){
          status = true;
        }
      }
    }
    return status;
  }
  void static validateCoords(int &y, int &x, string const &input, int direction, board &pboard, ships &pShips, char iter){
    if(input.size() > 3){
      x = INT_MAX;
      y = INT_MAX;
    } else {
      y = input[0] - 48;
      x = input[2] - 48;
      if (direction == 0) { //horizontal
        if (((x + pShips.getStatus(iter)) > pboard.getCols()) || (shipThere(pboard, pShips, iter, y, x, direction))) {
          y = INT_MAX;
          x = INT_MAX;
        }
      } else { //vertical
        if (((y + pShips.getStatus(iter)) > pboard.getRows()) || (shipThere(pboard, pShips, iter, y, x, direction))) {
          y = INT_MAX;
          x = INT_MAX;
        }
      }
    }
  }
  virtual void placeAllShips(board &pboard, ships &pShips, int i) = 0;
  virtual void getShot(Player &defenseplayer, board &primaryBoard, board &secondaryBoard, ships &ships, bool &gameContinues) = 0;
 private:
  string name;
  int Numships;
};

class Human : public Player {
 public:
  void placeAllShips(board &pboard, ships &pShips, int i) override {
    cout << "Player " << i << " please enter your name: ";
    getline(cin, name);
    setName(name);
    pboard.printPlacement();
    for(int k = 65; k < 91; k++){
      char iter = (char)k;
      int direction = INT_MAX;
      int x = INT_MAX;
      int y = INT_MAX;
      if(pShips.keyExists(iter)){
        hOrV(pboard, pShips, iter, direction);
        userCoords(pboard, pShips, iter, y, x, direction);
        pboard.printPlacement();
      }
    }
  }
  int static validateHorV(string input){
    transform(input.begin(), input.end(), input.begin(), ::tolower);
    if((input == "h") || (input == "h ")){
      return 0;
    } else if((input == "v") || (input == "v ")){
      return 1;
    } else {
      return INT_MAX;
    }
  }
  void hOrV(board &pboard, ships &pShips, char iter, int &direction){
    direction = INT_MAX;
    cout << getName() << ", do you want to place " << iter << " horizontally or vertically?" << endl;
    cout << "Enter h for horizontal or v for vertical" << endl << "Your choice: ";
    string input;
    getline(cin, input);
    direction = validateHorV(input);
    if(direction > 2){
      hOrV(pboard, pShips, iter, direction);
    }
  }
  void userCoords(board &pboard, ships &pShips, char iter, int &y, int &x, int direction){
    y = INT_MAX;
    x = INT_MAX;
    cout << getName() <<  ", enter the row and column you want to place " << iter << ", which is " << pShips.getStatus(iter)
         << " long, at with a space in between row and col: ";
    string input;
    getline(cin, input);
    validateCoords(y, x, input, direction, pboard, pShips, iter);
    if((y < pboard.getRows()) && (x < pboard.getCols())){
      placeShip(pboard, pShips, iter, y, x, direction);
    } else {
      hOrV(pboard, pShips, iter, direction);
      userCoords(pboard, pShips, iter, y, x, direction);
    }
  }
  void getShot(Player &defenseplayer, board &primaryBoard, board &secondaryBoard, ships &ships, bool &gameContinues) override{
    printBoardsAfterUpdate(primaryBoard, secondaryBoard);
    int x = INT_MAX;
    int y = INT_MAX;
    char c;
    cout << getName() << ", where would you like to fire?" << endl;
    cout << "Enter your attack coordinate in the form row col: ";
    string input;
    getline(cin, input);
    int tick = 0;
    int i;
    for(i = 0; i < input.size(); i++){
      if((input[i] == 45) || (input[i] == 32) || isdigit(input[i])){
        tick++;
      }
    }
    if(tick == i){
      validateFire(secondaryBoard, ships, y , x, input);
      if((x != INT_MAX) && (y != INT_MAX)){
        c = secondaryBoard.currPos(y,x);
        hitStatus(defenseplayer, secondaryBoard, ships, y, x);
        printBoardsAfterUpdate(primaryBoard, secondaryBoard);
        gameStatus(defenseplayer, secondaryBoard, ships, y, x, gameContinues, c);
      } else {
        printBoardsAfterUpdate(primaryBoard, secondaryBoard);
        getShot(defenseplayer, primaryBoard, secondaryBoard, ships, gameContinues);
      }
    } else {
      getShot(defenseplayer, primaryBoard, secondaryBoard, ships, gameContinues);
    }
  }
 private:
  string name;
};

class AIPlayer : public Player {
 public:
  static mt19937 generator;
  void placeAllShips(board &pboard, ships &pShips, int i) override{
    stringstream ss;
    ss << i;
    setName("AI " + ss.str());
    int row;
    int col;
    int direction;
    int Rimpact = 0;
    int Cimpact= 0;
    char iter;
    bool unplacedShipExists = true;
    for(int k = 65; k < 91; k++){
      iter = (char)k;
      if(pShips.keyExists(iter)){
        unplacedShipExists = true;
        while(unplacedShipExists){
          uniform_int_distribution<> hOrV(0, 1); // 2
          direction = hOrV(generator);
          //cout << "DIRECTION = " << direction << endl;
          if(direction == 0){
            Cimpact = pShips.getStatus(iter);
          } else {
            Rimpact = pShips.getStatus(iter);
          }
          uniform_int_distribution<> rowNum(0, ((pboard.getRows()-1) - Rimpact));
          row = rowNum(generator);
          //cout << "ROW = " << row << endl;
          uniform_int_distribution<> colNum(0, ((pboard.getCols()-1) - Cimpact));
          col = colNum(generator);
          //cout << "COL = " << col << endl;
          if(!shipThere(pboard, pShips, iter, row, col, direction)){
            placeShip(pboard, pShips, iter, row, col, direction);
            cout << "AI " << ss.str() << "'s" << " Board" << endl;
            pboard.printPlacement();
            unplacedShipExists = false;
          }
        }
      }
    }
  }
};

 class CheatingAI : public AIPlayer{
  public:
   void getShot(Player &defenseplayer, board &primaryBoard, board &secondaryBoard, ships &ships, bool &gameContinues) override{
     //secondaryboard == where we are attacking
     char c;
     bool done = false;
       for (int i = 0; i < secondaryBoard.getRows(); i++) {
         if(done){break;}
         for (int j = 0; j < secondaryBoard.getCols(); j++) {
           if(done){break;}
           if ((secondaryBoard.currPos(i, j) != '*') && (secondaryBoard.currPos(i, j) != 'O') && (secondaryBoard.currPos(i, j) != 'X')) {
             c = secondaryBoard.currPos(i, j);
             secondaryBoard.add(i, j, 'X');
             ships.hit(c);
             printBoardsAfterUpdate(primaryBoard, secondaryBoard);
             cout << getName() << " hit " << defenseplayer.getName() << "'s " << c << "!" << endl;
             if (ships.getStatus(c) == 0) {
               defenseplayer.shipSunk();
               cout << getName() << " destroyed " << defenseplayer.getName() << "'s " << c << "!" << endl;
               if (defenseplayer.getShips() == 0) {
                 cout << endl << getName() << " won the game!" << endl;
                 gameContinues = false;
               }
             }
             done = true;
           }
         }
       }
     }
};

class RandomAI : public AIPlayer{
 public:
  void getShot(Player &defenseplayer, board &primaryBoard, board &secondaryBoard, ships &ships, bool &gameContinues) override{
    if(possibleShots.empty()){
      for (int i = 0; i < secondaryBoard.getRows(); i++) {
        for (int j = 0; j < secondaryBoard.getCols(); j++) {
          possibleShots.emplace_back(i, j);
        }
      }
    }
    uniform_int_distribution<> vecShots(0, (int)(possibleShots.size()-1));
    int pickedShot = vecShots(generator);
    int y = possibleShots[pickedShot].first;
    int x = possibleShots[pickedShot].second;
    //cout << "picked shot  = " << pickedShot << ", (y,x) = (" << y << "," << x << ")" << endl;
    possibleShots.erase(possibleShots.begin() + pickedShot);
    char c = secondaryBoard.currPos(y,x);
    if(c == '*'){
      secondaryBoard.add(y, x, 'O');
      printBoardsAfterUpdate(primaryBoard, secondaryBoard);
      cout << "Missed." << endl;
    } else {
      ships.hit(c);
      if(ships.getStatus(c) == 0){
        defenseplayer.shipSunk();
      }
      secondaryBoard.add(y, x, 'X');
      printBoardsAfterUpdate(primaryBoard, secondaryBoard);
      cout << getName() << " hit " << defenseplayer.getName() << "'s " << c << "!" << endl;
      if(ships.getStatus(c) == 0){
        cout << getName() << " destroyed " << defenseplayer.getName() << "'s " << c << "!" << endl;
        if(defenseplayer.getShips() == 0){
          cout << endl << getName() << " won the game!" << endl;
          gameContinues = false;
        }
      }
    }
  }
 private:
  vector<pair<int, int>> possibleShots;
};

class SearchAndDestroyAI : public RandomAI {
 public:
  char huntLocation(int &y, int &x, board &secondaryBoard) {
    if (shotDir == 0) { // left
      shotDir++;
      if ((x - 1) > 0) {
        x--;
        return secondaryBoard.currPos(y, x);
      } else {
        return huntLocation(y, x, secondaryBoard);
      }
    } else if (shotDir == 1) { // top
      shotDir++;
      if ((y - 1) > 0) {
        y--;
        return secondaryBoard.currPos(y, x);
      } else {
        return huntLocation(y, x, secondaryBoard);
      }
    } else if (shotDir == 2) { // right
      shotDir++;
      if ((x + 1) < secondaryBoard.getCols()) {
        x++;
        return secondaryBoard.currPos(y, x);
      } else {
        return huntLocation(y, x, secondaryBoard);
      }
    } else { //down
      shotDir = 0;
      targets.erase(targets.begin());
      if ((y + 1) < secondaryBoard.getRows()) {
        y++;
        return secondaryBoard.currPos(y, x);
      } else {
        return huntLocation(y, x, secondaryBoard);
      }
    }
  }
  void getShot(Player &defenseplayer, board &primaryBoard, board &secondaryBoard, ships &ships, bool &gameContinues) override{
    int x;
    int y;
    int pickedShot;
    if(possibleShots.empty()){
      for (int i = 0; i < secondaryBoard.getRows(); i++) {
        for (int j = 0; j < secondaryBoard.getCols(); j++) {
          possibleShots.emplace_back(i, j);
        }
      }
    }
    char c = '?';
    if(isHunting){
      uniform_int_distribution<> vecShots(0, (int)(possibleShots.size()-1));
      pickedShot = vecShots(generator);
      y = possibleShots[pickedShot].first;
      x = possibleShots[pickedShot].second;
     // cout << "picked shot  = " << pickedShot << ", (y,x) = (" << y << "," << x << ")" << endl;
      possibleShots.erase(possibleShots.begin() + pickedShot);
      c = secondaryBoard.currPos(y,x);
      if((c == 'X') || (c == 'O')) {
        getShot(defenseplayer, primaryBoard, secondaryBoard, ships, gameContinues);
      } else if(c == '*'){ //doesn't check for blank or hit spots
        secondaryBoard.add(y, x, 'O');
        printBoardsAfterUpdate(primaryBoard, secondaryBoard);
        cout << "Missed." << endl;
      } else {
        targets.emplace_back(y, x);
        ships.hit(c);
        if(ships.getStatus(c) == 0){
          defenseplayer.shipSunk();
        }
        secondaryBoard.add(y, x, 'X');
        printBoardsAfterUpdate(primaryBoard, secondaryBoard);
        cout << getName() << " hit " << defenseplayer.getName() << "'s " << c << "!" << endl;
        if(ships.getStatus(c) == 0){
          cout << getName() << " destroyed " << defenseplayer.getName() << "'s " << c << "!" << endl;
          if(defenseplayer.getShips() == 0){
            cout << endl << getName() << " won the game!" << endl;
            gameContinues = false;
          }
        }
        isHunting = false;
      }
    } else {
      if (targets.empty()) {
        isHunting = true;
        getShot(defenseplayer, primaryBoard, secondaryBoard, ships, gameContinues);
      } else {
        y = targets[0].first;
        x = targets[0].second;
        c = huntLocation(y, x, secondaryBoard);
        if ((c == 'X') || (c == 'O')) {
          shotDir++;
          getShot(defenseplayer, primaryBoard, secondaryBoard, ships, gameContinues);
        } else if (c == '*') {
          secondaryBoard.add(y, x, 'O');
          printBoardsAfterUpdate(primaryBoard, secondaryBoard);
          cout << "Missed." << endl;
        } else {
          targets.emplace_back(y, x);
          ships.hit(c);
          if (ships.getStatus(c) == 0) {
            defenseplayer.shipSunk();
          }
          secondaryBoard.add(y, x, 'X');
          printBoardsAfterUpdate(primaryBoard, secondaryBoard);
          cout << getName() << " hit " << defenseplayer.getName() << "'s " << c << "!" << endl;
          if (ships.getStatus(c) == 0) {
            cout << getName() << " destroyed " << defenseplayer.getName() << "'s " << c << "!" << endl;
            if (defenseplayer.getShips() == 0) {
              cout << endl << getName() << " won the game!" << endl;
              gameContinues = false;
            }
          }
        }
      }
    }
  }
 private:
  vector<pair<int, int>> possibleShots;
  vector<pair<int, int>> targets;
  bool isHunting = true;
  int shotDir = 0; //0=left, 1=top, .... counter clockwise
};

vector<string> readFile(string const &filename){
  //read file into a vector of string
  ifstream myfile;
  myfile.open(filename);
  if(!myfile.is_open()){
    cout << "ERROR OPENING FILE" << endl;
  }
  vector<string> myLines;
  string line;
  while (std::getline(myfile, line))
  {
    myLines.push_back(line);
  }
  return myLines;
}

void playGame(Player &player1, Player &player2, board &p1board, board &p2board, ships &p1ships, ships &p2ships){
  bool gameContinues = true;
  while(gameContinues){
    player1.getShot(player2, p1board, p2board, p2ships, gameContinues);
    if(gameContinues) {
      player2.getShot(player1, p2board, p1board, p1ships, gameContinues);
    }
  }
}

int validateType(string const &input){
  if(input == "1"){
    return 1;
  } else if(input == "2"){
    return 2;
  } else if(input == "3"){
    return 3;
  } else {
    return INT_MAX;
  }
}

int promptType(){
  cout << "What type of game do you want to play?" << endl;
  cout << "1. Human vs Human" << endl << "2. Human vs AI" << endl << "3. AI vs AI" << endl;
  cout << "Your choice: ";
  string input;
  getline(cin, input);
  int type = validateType(input);
  if(type < 4){
    return type;
  } else {
    return promptType();
  }
}

int promptAIType(){
  cout << "What AI do you want?" << endl;
  cout << "1. Cheating AI" << endl << "2. Random AI" << endl << "3. Hunt Destroy AI" << endl;
  cout << "Your choice: ";
  string input;
  getline(cin, input);
  int type = validateType(input);
  if(type < 4){
    return type;
  } else {
    return promptAIType();
  }
}

unique_ptr<Player> AITypeReturner(int AItype){
  if(AItype == 1){
    unique_ptr<Player> robot = make_unique<CheatingAI>();
    return robot;
  } else if (AItype == 2){
    unique_ptr<Player> robot = make_unique<RandomAI>();
    return robot;
  } else {
    unique_ptr<Player> robot = make_unique<SearchAndDestroyAI>();
    return robot;
  }
}

ships OOsetUp(Player &player, board &pboard, vector<string> &entireFile, int pNum) {
  player.setShips(stoi(entireFile[2]));
  ships pShips;
  for(int i = 3; i < entireFile.size(); i++){
    pShips.addShip(entireFile[i]);
  }
  player.placeAllShips(pboard, pShips, pNum);
  return pShips;
}
mt19937 AIPlayer::generator;

void createBoards(int argc, char* argv[]){
  //seed
  if(argc < 2){
    AIPlayer::generator.seed(time(nullptr));
  } else {
    AIPlayer::generator.seed(stoi(argv[2]));
  }
  //move everything from file into classes
  string filename(argv[1]);
  vector<string> entireFile = readFile(filename);
  board p1board;
  board p2board;
  p1board.setSize(stoi(entireFile[0]), stoi(entireFile[1]));
  p2board.setSize(stoi(entireFile[0]), stoi(entireFile[1]));

  //prompt player for gametype
  //create instances of players depending on input
  int type = promptType();
  if(type == 1){
    //PvP
    unique_ptr<Player> player1 = make_unique<Human>();
    ships p1Ships = OOsetUp(*player1, p1board, entireFile, 1);
    unique_ptr<Player> player2 = make_unique<Human>();
    ships p2Ships = OOsetUp(*player2, p2board, entireFile, 2);
    playGame(*player1, *player2, p1board, p2board, p1Ships, p2Ships);
  } else if(type == 2){
    //PvA
    unique_ptr<Player> player1 = make_unique<Human>();
    ships p1Ships = OOsetUp(*player1, p1board, entireFile, 1);
    type = promptAIType();
    unique_ptr<Player> player2 = AITypeReturner(type);
    ships p2Ships = OOsetUp(*player2, p2board, entireFile, 2);
    playGame(*player1, *player2, p1board, p2board, p1Ships, p2Ships);

  } else {
    //AvA
    type = promptAIType();
    unique_ptr<Player> player1 = AITypeReturner(type);
    ships p1Ships = OOsetUp(*player1, p1board, entireFile, 1);
    int type2 = promptAIType();
    unique_ptr<Player> player2 = AITypeReturner(type2);
    ships p2Ships = OOsetUp(*player2, p2board, entireFile, 2);
    playGame(*player1, *player2, p1board, p2board, p1Ships, p2Ships);
  }
}