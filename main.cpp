/*
  Copyright (c) 2019
  Swarthmore College Computer Science Department, Swarthmore PA
  J. Brody, A. Danner, M. Gagne, L. Meeden, Z. Palmer, A. Soni, M. Wehar
  Distributed as course material for Fall 2019
  CPSC 035: Data Structures and Algorithms
*/

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>

#include "inroadsGUI.h"
#include "ioUtils.h"
#include "inroadsGame.h"
using std::to_string;

using namespace std;

int main(int argc, char** argv) {
    // Check command line arguments and give up if necessary.
    if (argc != 2) {
        cerr << "Expected one argument: name of map file." << endl;
        return 1;
    }

    // Initialize randomizer.  This should happen before any random numbers are
    // selected.
    srand(time(nullptr));

    // Get command-line argument.
    string mapFilename = string(argv[1]);

    // Read the Inroads map file.
    pair<Graph<string, bool, int>*, Dictionary<string, pair<int, int>>*>
        mapFileContents;
    try {
        mapFileContents = readInroadsMap(mapFilename);
    } catch (exception e) {
        cout << "Could not read Inroads map file " << mapFilename << ": "
             << e.what() << endl;
    }
    // The map file contained the graph representing the map.  It also contained
    // the vertex positions (so we know where each vertex goes on the map).
    Graph<string, bool, int>* graph = mapFileContents.first;
    Dictionary<string, pair<int, int>>* vertexPositions =
        mapFileContents.second;

    // Create the GUI object here.  This is statically allocated, so the GUI
    // will disappear the moment your program leaves the main function.
    // The GUI object should only be used by main, and not by any other function
    // or object; do NOT change the type of this variable.
    InroadsGUI gui(vertexPositions, mapFilename);

    // Tell the GUI about the graph we have.
    gui.updateInroadsMap(graph);
    gui.updateScoreText("Score: 0");
    gui.updateTurnText("Turn: 1");

    // TODO: Write code that runs the game.  You'll want to write most of your
    // game logic into the RailwayGame class and then use an object of that type
    // here to control the game and relay what it says to the GUI.  This
    // assignment is more open-ended than the previous assignments.
    InroadsGame myGame;
    int turn = 1;
    pair<string, string> nextMove = gui.getNextMove();
    string currMessage = "";

    // keep going until user clicks to exit window
    while ((nextMove.first != "") && (nextMove.second != "") && (turn < 21)) {

      // check if the road is already improved
      if (graph->getEdge(nextMove.first, nextMove.second).getLabel() == false) {
        // update edge's label
        int tempWeight = graph->getEdge(nextMove.first,
                                                  nextMove.second).getWeight();
        graph->removeEdge(nextMove.first, nextMove.second);
        graph->insertEdge(nextMove.first, nextMove.second, true, tempWeight);
        gui.updateInroadsMap(graph);

        // update our small graph with improved roads only

        myGame.getNewVertices(nextMove.first, nextMove.second, true,
                                                            tempWeight, graph);
        currMessage = myGame.buildMessage(); // calculates all scores
        // update message with scores for that this turn
        gui.updateMessage(currMessage);
        // increment turn count
        turn++;
        gui.updateScoreText("Score: " + to_string(myGame.getTotalScore()));
        if (turn == 21) {
          gui.updateTurnText("Game Over!"); // add game over after 20 turns
        } else {
          gui.updateTurnText("Turn: " + to_string(turn));
        }
      } else {
        // if user clicks on a road that's already been improved,
        // show them this message
        gui.updateMessage(currMessage + "\t \t \t" +
                                         "This road is already improved");
      }

      nextMove = gui.getNextMove();
    }

    // Finally, clean up and exit successfully.
    delete graph;
    delete vertexPositions;
    return 0;
}
