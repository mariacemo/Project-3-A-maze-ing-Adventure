#pragma once
#include "Graph.h"
using namespace std;

class Dijkstras {
    Graph maze;

    // width/height of maze
    unsigned int width;
    unsigned int height;

    // parentVec, stores the "parent" of each node
    // if Edge(0,1), parentVec[1] = 0
    vector<int> parentVec;

    // recursive function returns vector with shortest path
    // vector[0] = source node, vector[vector.size() - 1] = target node
    vector<unsigned int> createPathVec(vector<unsigned int>& storePath, int target);

public:
    Dijkstras(Graph& m, unsigned int width, unsigned int height);

    // Dijkstra's algorithm uses min heap implemented with a priority queue to track shortest path
    // updates parentVec too
    void dijkstrAlgorithm();

    // target = last/exit node
    vector<unsigned int> getShortestPath(int target); // returns the vector returned in createPathVec

    // printing requires the vector returned by getShortestPath
    void printIDPath(int target); // prints shortest path using nodeNum
    void printEdgePath(int target); // prints shortest path using edges: Edge(x,y)
};