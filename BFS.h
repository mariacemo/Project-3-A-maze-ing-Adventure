
#pragma
//
// Created by Cami Morales on 11/29/22.
//
#include <queue>
#include <set>
#include "Graph.h"
#include <map>
#include <algorithm>
#include <vector>
using namespace std;

class BFS {

    // functions
    //Graph maze;
    int width;
    int height;
public:

    BFS();
    vector<unsigned int> getPath(map<int, int> c, int start, int target);
    vector<unsigned int> BFSAlgo(Graph& g, int target);
};
