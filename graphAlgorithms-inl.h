/*
  Copyright (c) 2019
  Swarthmore College Computer Science Department, Swarthmore PA
  J. Brody, A. Danner, M. Gagne, L. Meeden, Z. Palmer, A. Soni, M. Wehar
  Distributed as course material for Fall 2019
  CPSC 035: Data Structures and Algorithms
*/

#include <stdexcept>
#include <iostream>
#include <string>
#include <utility>

#include "adts/stack.h"
#include "adts/stlStack.h"
#include "adts/queue.h"
#include "adts/stlQueue.h"
#include "adts/stlHashTable.h"
#include "adts/graph.h"
#include "adjacencyListGraph.h"
#include "adts/stlMinPriorityQueue.h"
#include "adts/edge.h"

#include "graphAlgorithms.h"

template <typename V, typename E, typename W>
bool reachableDFS(V src, V dest, Graph<V, E, W>* g) {
    // create dictionary to keep track of which vertices have been visited
    STLHashTable<V, bool> visits;
    // add all vertices to hash table
    vector<V> allVertices = g->getVertices();
    for(int i = 0; i < allVertices.size(); i++) {
      visits.insert(allVertices[i], false);
    }

    /****************************************************/
    // use depth first search to find path through graph
    // create stack to hold vertices
    Stack<V> *vertices = new STLStack<V>();
    // add first vertex, source, to stack
    vertices->push(src);
    // mark vertex as visited
    visits.update(src, true);
    // repeat until stack is empty, adding adjacent vertices
    while(!vertices->isEmpty()) {
      // remove from top of stack
      V current = vertices->pop();
      // check if current is the destination vertex
      if(current == dest) {
        delete vertices;
        return true; // found a path
      } else {
        // add adjacent vertices to stack
        vector<V> neighbors = g->getNeighbors(current);
        // add all neighbors to stack
        for(int i = 0; i < neighbors.size(); i++) {
          if(visits.get(neighbors[i]) == false) {
            vertices->push(neighbors[i]);
            // mark vertex as visited
            visits.update(neighbors[i], true);
          }
        }
      }
    }
    delete vertices;
    return false; // no path found
}

template <typename V, typename E, typename W>
vector<V> shortestLengthPathBFS(V src, V dest, Graph<V, E, W>* g) {
    // create dictionary to keep track of which vertices have been visited
    STLHashTable<V, bool> visits;
    // add all vertices to hash table
    vector<V> allVertices = g->getVertices();
    for(int i = 0; i < allVertices.size(); i++) {
      visits.insert(allVertices[i], false);
    }

    // init vector to return path
    vector<V> shortestPath;
    // init hash table for previous vertices
    STLHashTable<V, V> previousSet;
    /****************************************************/
    // use breadth first search to find path through graph
    // create queue to hold vertices
    Queue<V> *vertices = new STLQueue<V>();
    // add first vertex, source, to queue
    vertices->enqueue(src);
    // mark vertex as visited
    visits.update(src, true);
    // repeat until queue is empty, adding adjacent vertices
    while(!vertices->isEmpty()) {
      // remove from first in queue
      V current = vertices->dequeue();
      // check if current is the destination vertex
      if(current == dest) {
        while(current != src) {
          shortestPath.push_back(current);
          current = previousSet.get(current);
        }
        // add start vertex
        shortestPath.push_back(current);
        // make new vector
        vector<V> toReturn;
        // switch order of path
        for(int i = shortestPath.size() - 1; i > -1; i--) {
          toReturn.push_back(shortestPath[i]);
        }
        delete vertices;
        return toReturn; // found a path, return vector of vertices
      } else {
        // add adjacent vertices to queue
        vector<V> neighbors = g->getNeighbors(current);
        // add all neighbors to queue
        for(int i = 0; i < neighbors.size(); i++) {
          if(visits.get(neighbors[i]) == false) {
            vertices->enqueue(neighbors[i]);
            // save neighbor's previous in hash table
            previousSet.insert(neighbors[i], current);
            // mark vertex as visited
            visits.update(neighbors[i], true);
          }
        }
      }
    }
    delete vertices;
    throw runtime_error("No path found."); // no path found
}

template <typename V, typename E, typename W>
Dictionary<V, W>* singleSourceShortestPath(V src, Graph<V, E, W>* g) {
    Dictionary<V, W> *vertices = new STLHashTable<V, W>();
    STLMinPriorityQueue<W, V> *edgeWeights = new STLMinPriorityQueue<W, V>();

    // insert first vertex
    edgeWeights->insert(0, src);
    // repeat until dictionary is full or until queue is empty
    while ((vertices->getSize() != g->getVertices().size()) &&
                                                  (!edgeWeights->isEmpty())){
      int priority = edgeWeights->peekPriority();
      V current = edgeWeights->remove(); // get lowest priority vertex
      // if the vertex isn't in the dictionary
      if (!vertices->contains(current)) {
        vector<Edge<V, E, W>> outgoing = g->getOutgoingEdges(current);
        // add vertex to dictionary
        vertices->insert(current, priority);
        // add all outgoing edges and weights to the priority queue
        for (int i = 0; i < outgoing.size(); i++) {
          edgeWeights->insert(vertices->get(current) + outgoing[i].getWeight(),
                                                outgoing[i].getDestination());
        }
      }
    }
    delete edgeWeights;
    return vertices;

}
