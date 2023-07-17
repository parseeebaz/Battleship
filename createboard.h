//
// Created by dante on 5/2/2022.
//

#include <string>
#include <vector>
#include <random>
#include <iostream>
#include <fstream>
#include <map>
#include <bits/stdc++.h>
#include <algorithm>

using namespace std;

#ifndef BATTLESHIP__CREATEBOARD_H_
#define BATTLESHIP__CREATEBOARD_H_
namespace BattleShip{
  class ships;
  class board;
  class Player;
}
using namespace BattleShip;
class Human;
class AIPlayer;
class CheatingAI;
class RandomAI;
class SearchAndDestroyAI;
vector<string> readFile(const string& filename);
void playGame(Player &player1, Player &player2, board &p1board, board &p2board, class ships &p1ships, class ships &p2ships);
int validateType(string const &input);
int promptType();
int promptAIType();
unique_ptr<Player> AITypeReturner(int AItype);
ships OOsetUp(Player &player, board &pboard, vector<string> &entireFile, int pNum);
void createBoards(int argc, char* argv[]);
#endif //BATTLESHIP__CREATEBOARD_H_