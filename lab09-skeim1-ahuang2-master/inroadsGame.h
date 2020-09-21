/*
  Copyright (c) 2019
  Swarthmore College Computer Science Department, Swarthmore PA
  J. Brody, A. Danner, M. Gagne, L. Meeden, Z. Palmer, A. Soni, M. Wehar
  Distributed as course material for Fall 2019
  CPSC 035: Data Structures and Algorithms
*/

#pragma once
#include <vector>
#include "adts/graph.h"
#include "graphAlgorithms.h"

using std::vector;
using std::string;



/**
 * An instance of this class represents a single game of Inroads.  An object of
 * type InroadsGame does not represent the user interface, but does represent
 * the idea of a game of Inroads, including the state of the board and all of
 * the information pertaining to the player.  It also includes the logic for
 * making moves in the game and enforces the game's rules.
 */

class InroadsGame {
  public:

    InroadsGame();


    ~InroadsGame();

    /**
    * Gets vertices from getNextMove and puts them into the graph map private
    * variable. Also adds the corresponding edge to the graph map and sorts
    * the new vertices into appropriate vectors based on type.
    * @param src The source vertex.
    * @param dest The destination vertex.
    * @param label Whether or not the edge clicked was improved.
    * @param weight The weight of the edge clicked.
    * @param graph A pointer to the Graph object.
    */
    void getNewVertices(string src, string dest, bool label, int weight, Graph<string, bool, int>* graph);

    /**
    * Uses DFS to see if every residential has a path to the library in the
    * graph, adding points accordingly to the score (15 pt per residential).
    * @return int The total points scored from the library for that turn.
    */
    int calcLibraryScore();

    /**
    * Uses BFS to find paths of distance 2 or less between stores and
    * residentials, adding points accordingly (10 pt per residential).
    * @return int The total points scored from the stores for that turn.
    */
    int calcStoreScore();

    /**
    * Uses Dijkstra's algorithm to find the minimum weight paths from
    * medical locations to residentials, calculating the score using
    * 420/n, where n is the min weight path from the medical to the
    * residential.
    * @return int The total points scored from the medical locations
    * for that turn.
    */
    int calcMedicalScore();

    /**
    * Calls each of the score-calculating methods, adding their scores
    * to one string to send to main.
    * @return string The message for the text area under the graph.
    */
    string buildMessage();

    /**
    * Adds vertices to appropriate vectors for storing and easy
    * search in the score-calculating methods. Sorts into stores,
    * medical centers, and residentials.
    * @param src The vertex to add.
    */
    void addType(string src);

    /**
    * Returns the total score in the game so far, including all previous turns.
    * @return int The total score.
    */
    int getTotalScore();

  private:
    Graph<string, bool, int>* map;
    int libraryScore;
    int storeScore;
    int medicalScore;
    int score;
    int turn;

    vector<string> stores;
    vector<string> medicals;
    vector<string> residentials;

};
