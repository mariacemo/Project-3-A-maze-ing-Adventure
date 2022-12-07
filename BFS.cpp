//
// Created by Cami Morales on 11/29/22.
//
#pragma
#include "BFS.h"

BFS::BFS()
{
    //width = 5;
}

vector<unsigned int> BFS::getPath(map<int, int> c, int start, int target)
{
    // backtrace to get the actuakl path

    vector<unsigned int> path;
    path.push_back(target);


    int num = target;
    while (c[num] != -1)
    {
        path.push_back(c[num]);
        num = c[num];
    }

    reverse(path.begin(), path.end());

    return path;

}

vector<unsigned int> BFS::BFSAlgo(Graph& g, int target)
{



    map<int, int> tracing;

    // so it goes into the function
    queue<int> q;
    set<int> visited;
    // vector to store the traversal
    vector<unsigned int> path;


    visited.insert(g.getMazeNode(0, 0)->_nodeNum);
    q.push(g.getMazeNode(0, 0)->_nodeNum);

    tracing[g.getMazeNode(0, 0)->_nodeNum] = -1;
    // while pq isn't empty && target node hasn't been found

    while (!q.empty())
    {
        int curr = q.front();

        path.push_back(curr);
        q.pop();

        // pair <distance, node> = int, int
        vector<int> neighbors;

        unsigned int xCoor = curr % width;
        unsigned int yCoor = curr / width;

        if (curr == target)
            return getPath(tracing, 0, target);

        // need to look at (u)'s vector of adjacent nodes

        // populating the neighbors
        for (auto it = g.getMazeNode(xCoor, yCoor)->adjacentNodes.begin();
            it != g.getMazeNode(xCoor, yCoor)->adjacentNodes.end(); it++)
        {
            // add the adjacent nodes to the neighbors list
            if (it->first != nullptr)
            {
                int node = it->first->_nodeNum;
                //int weight = it->second;
                neighbors.push_back(node);

            }
        }

        // checking the neighbors
        for (int i = 0; i < neighbors.size(); i++)
        {
            if (visited.count(neighbors[i]) == 0)
            {
                tracing[neighbors[i]] = curr;
                visited.insert(neighbors[i]);
                q.push(neighbors[i]);
            }
        }

    }
    return path;
}