#include "Dijkstras.h"

Dijkstras::Dijkstras(Graph& m, unsigned int widthX, unsigned int heightY) {
    maze = m;
    width = widthX;
    height = heightY;
    parentVec = vector<int>(maze.getMazeSize(), -1); // initialize each parent node as -1
}

vector<unsigned int> Dijkstras::createPathVec(vector<unsigned int>& storePath, int target) {
    if (parentVec[target] == -1) { // only the first node/source == -1
        storePath.push_back(target);
    }
    else {
        createPathVec(storePath, parentVec[target]);
        storePath.push_back(target);
    }
    return storePath;
}

// returns a vector node's nodeNum
vector<unsigned int> Dijkstras::getShortestPath(int target) {
    vector<unsigned int> path;
    createPathVec(path, target);
    return path;
}

void Dijkstras::printIDPath(int target) {
    for (int i = 0; i < getShortestPath(target).size() - 1; i++) {
        cout << getShortestPath(target)[i] << ", ";
    }
    cout << getShortestPath(target).at(getShortestPath(target).size() - 1) << endl; // print last node / target
}

void Dijkstras::printEdgePath(int target) {
    for (int i = 0; i < getShortestPath(target).size() - 1; i++) {
        cout << "Edge: (" << getShortestPath(target)[i] << ", " << getShortestPath(target)[i + 1] << ")" << std::endl;
    }
}

void Dijkstras::dijkstrAlgorithm() {
    // pair <distance, node> = int, int
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    vector<int> distVec(maze.getMazeSize(), INT_MAX); // store distance from source node
    pq.push(make_pair(0, maze.getMazeNode(0, 0)->_nodeNum)); // Node 0's weight = 0

    distVec[maze.getMazeNode(0, 0)->_nodeNum] = 0; // at distVec[0] (node 0), weight = 0

    bool findingExit = true;
    // while pq isn't empty && target node hasn't been found
    while (!pq.empty() && findingExit) {
        pair<int, int> curr = pq.top();
        pq.pop();
        int u = curr.second; // node uniqueID

        unsigned int xCoor = u % width;
        unsigned int yCoor = u / width;

        // need to look at (u)'s vector of adjacent nodes
        for (auto it = maze.getMazeNode(xCoor, yCoor)->adjacentNodes.begin();
            it != maze.getMazeNode(xCoor, yCoor)->adjacentNodes.end(); it++) {

            // avoid nonexistent neighbors
            if (it->first != nullptr) {
                // compare nodeNum, check if neighbor = target node
                if (it->first->_nodeNum == maze.getMazeNode(width - 1, height - 1)->_nodeNum) {
                    distVec[it->first->_nodeNum] = distVec[u] + it->second; // final update to distVec
                    parentVec[it->first->_nodeNum] = u; // final update to parentVec

                    findingExit = false; // end while loop
                }
                else {
                    int v = it->first->_nodeNum; // neighbor
                    int w = it->second; // weight

                    if (distVec[v] > distVec[u] + w) {
                        distVec[v] = distVec[u] + w; // update distance
                        parentVec[v] = u; // update parent node
                        pq.push(make_pair(distVec[v], v)); // push new neighbors
                    }
                }
            }
        }
    }
}