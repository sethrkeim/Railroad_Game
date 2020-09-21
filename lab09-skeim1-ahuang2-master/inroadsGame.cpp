/*
  Copyright (c) 2019
  Swarthmore College Computer Science Department, Swarthmore PA
  J. Brody, A. Danner, M. Gagne, L. Meeden, Z. Palmer, A. Soni, M. Wehar
  Distributed as course material for Fall 2019
  CPSC 035: Data Structures and Algorithms
*/

#include <string>

#include "inroadsGame.h"
#include "adts/graph.h"
#include "graphAlgorithms.h"
#include "adts/dictionary.h"
#include "adjacencyListUndirectedGraph.h"

#include <stdexcept>
using std::runtime_error;
using std::to_string;
using std::cout;
using std::endl;

InroadsGame::InroadsGame() {
  this->libraryScore = 0;
  this->storeScore = 0;
  this->medicalScore = 0;
  this->score = 0;
  this->turn = 1;
  this->map = new AdjacencyListUndirectedGraph<string, bool, int>();
}

InroadsGame::~InroadsGame() {
  delete this->map;
}

void InroadsGame::getNewVertices(string src, string dest,
                      bool label, int weight, Graph<string, bool, int>* graph) {
   this->turn++;
   this->libraryScore = 0;
   this->storeScore = 0;
   this->medicalScore = 0;
   // if vertex isn't already in our map of only improved edges, add it
   if (!this->map->containsVertex(src)) {
     this->map->insertVertex(src);
     addType(src); //making a mini vector of all vertices of each type
   }
   // if vertex isn't already in our map of only improved edges, add it
   if (!this->map->containsVertex(dest)) {
     this->map->insertVertex(dest);
     addType(dest); //making a mini vector of all vertices of each type
   }
   // insert the edge into our map
   this->map->insertEdge(src, dest, label, weight);

}

int InroadsGame::calcLibraryScore() {
  // if library is in our map
  if ((this->map->containsVertex("L")) && (this->residentials.size() != 0)) {
    // check each residential location in our map
    for (int i = 0; i < this->residentials.size(); i++) {
      string LibraryVertex = "L";
      // check to make sure the location is connected with the library
      if (reachableDFS(LibraryVertex, this->residentials[i], this->map)) {
        this->score += 15; // add to total score
        this->libraryScore += 15; //add to turn library score
      }
    }
  }
  return this->libraryScore;
}

int InroadsGame::calcStoreScore() {
  // if there are any stores and residential locations in our map
  if ((this->stores.size() != 0) && (this->residentials.size() != 0)) {
    // for each store
    for (int s = 0; s < this->stores.size(); s++) {
      // for each residential location
      for (int r = 0; r < this->residentials.size(); r++) {
        try {
          // see how far apart the store and residential are
          if (shortestLengthPathBFS(this->stores[s], this->residentials[r],
                                                      this->map).size() <= 3) {

            this->score += 10;
            this->storeScore += 10;
          }
        } catch (runtime_error e) {
          // if the store and residential location are not connected, do nothing
        }

        }
      }
    }
  return this->storeScore;
}


int InroadsGame::calcMedicalScore() {
  // if there are any medical and residential locations in our map
  if ((this->medicals.size() != 0) && (this->residentials.size() != 0)) {
    // for each medical
    for (int m = 0; m < this->medicals.size(); m++) {
      // get the dictionary of the min distance to each vertex
      Dictionary<string, int>* medicalPaths =
                        singleSourceShortestPath(this->medicals[m], this->map);
      // for each residential
      for (int r = 0; r < this->residentials.size(); r++) {
        // get the min weight to the medical center from the dictionary
        if (medicalPaths->contains(this->residentials[r])) {
          this->score += 420/(medicalPaths->get(this->residentials[r]));
          this->medicalScore += 420/(medicalPaths->get(this->residentials[r]));
        }
      }
      delete medicalPaths; // free memory
    }
  }

  return this->medicalScore;
}

string InroadsGame::buildMessage() {
    // calls each of the score calculation methods and adds to one string
    return "Your score this turn: \n \t +" + to_string(calcLibraryScore()) +
    " from the library \n \t +" + to_string(calcStoreScore()) + " from stores" +
    "\n \t +" + to_string(calcMedicalScore()) + " from medical access \n " +
    "Total points this turn: " + to_string(this->libraryScore + this->storeScore
                                                          + this->medicalScore);
}

void InroadsGame::addType(string src) {
  // sorts a given vertex into one of 3 vectors for easy use later
  if (src.at(0) == 'S') { // check the first letter of the string
    this->stores.push_back(src); // add to vector storing store vertices
  } else if (src.at(0) == 'M') {
    this->medicals.push_back(src); // add to vector storing medical vertices
  } else if (src.at(0) == 'R') {
    this->residentials.push_back(src); // add to vector storing residentials
  }
}

int InroadsGame::getTotalScore() {
  return this->score; // lets main access the total score 
}
