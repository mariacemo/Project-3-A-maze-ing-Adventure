#pragma once
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <queue>
#include <unordered_map>
#include <set>
#include <SFML/Graphics.hpp>
#include "TextureManager.h"

/* Project 3 MAZE - Nicholas Callahan
*
* If you have any questions about how to use the map I've included some test cases that show about how to use the graph, let me know if you guys need any help its probably best to text me
*
*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
* CREATING GRAPH OBJECT
*
*	- The constructor takes 3 parameters two unsigned ints for the width and height and a bool for "debug" mode
*		you can also toggle debug mode with the function debug()
*
*	- with debug set to TRUE the maze will use the seed to create the maze, "_seed" is 1 by default but can be changed with the setSeed(int) function
*		if you want to input a different seed other than 1 you have to input the seed prior to calling initalizeMaze()
*		If debug is false the maze will use TIME(NULL) (random) as the seed
*
*	- I suggest using debug mode to have a consistant maze that you can test with and only turing it off once you've completed functions
*
* CREATING THE MAZE
*
*	- Once everything is set up you can call initalizeMaze()
*
*		this function is a relativly expensive function compared to the rest of the program
*		on my desktop for 100k nodes it takes almost no time but running the my test  which in some cases traverse the whole graph can sometimes take about 2-3 seconds
*
*	- InitalizeMaze creates a grid graph then assigns random weights to each edge and then creates a minimum spanning tree out of the whole thing using prims with a priority queue
*
*		this also means there are NO CYCLES, however there are dead ends, there is only path to a specific node from a given node
*
*  USING THE MAZE
*
*	- getMazeNode takes in an x and y, (0,0) is the top left corner all the way to (w-1, h-1), the bottom right and returns a Node*.
*		each Node object contains its unique number (_nodeNum) and a vector of size 4 with Node* as elements.
*			its unique number is how it is stored in the unordered map, you can also use it to get its x and y (look at getMazeNode to see how)
*
*	- getAdjacent takes a Node* and gives a vector<Node*>
*		the index of the element in the vector tells you its relative position from the given Node
*			vector.at(0) is the node above the node object, 1 is to the right 2 is down and 3 is left.
*			if the node doesn't have an adjacent node in that direction it will be nullptr and UINT_MAX weight it will be important to CHECK FOR NULLPTR
*
* PRINT EDGES
*
*	- the function prints a list of the edges of the maze, I find it helpful to draw out the edges, all of the vertices have edges from A -> B and from B -> A this is intentionaly, it allows B to access A and A to access B
*		The order it prints is relatively random due to the use of an unordered_map however it will always print the edges in the clockwise order so it will print the edge that leads above it, to the right, below and to the left, however
*		if there is no node in the direction (std::pair(Node*, w) = std::pair(nullptr, UINT_MAX)) nothing will print
*
*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
*/


class Graph
{
    struct Node
    {
        sf::Sprite sprite; //revealed or hidden
        sf::Sprite attribute; // protagonist, footsteps
        Node(string textureName);


        unsigned int _nodeNum; //can be used to get the location of a node by floor of dividing it by the wideth to get the height/y
        // and taking the % to get the x you can see the reverse why that I do it in getNode and getMazeNode
        std::vector<std::pair<Node*, unsigned char>> adjacentNodes; //contains 4 values index 0 is up index 1 is right 2 down and 3 left short is weight
        //could be used in the future to store textures
        Node();
        void setNodeNum(unsigned int nodeID);//nodeID is the node num
        void drawNode(sf::RenderWindow& window, unsigned int width, unsigned int spriteSize);
    };


    //maze map
    std::unordered_map<unsigned int, Node> mazeMap;

    unsigned int _spriteSize;
    unsigned int _width;
    unsigned int _height;
    bool _debug;
    int _seed;

    //creates base graph
    void initalizeGraph(std::unordered_map<unsigned int, Node>& nodeMap);
    //used to set lock in seed
    void setRand();
    //used to connect grid graph NOT MAZE
    void connectVertices(std::unordered_map<unsigned int, Node>& nodeMap);
    //wieghts are randomly set to get a random maze every time (unless same seed), NOT USED FOR FUTURE TRAVSERAL, weights set to 1 after maze is created.
    unsigned char genWeight();

    //creates maze from graph
    //returns a node* at given x,y
    Node* getNode(unsigned int x, unsigned int y, std::unordered_map<unsigned int, Node>& nodeMap);
    //outdated implimentation of prims algorithm to connect the tree without priority queue, TOO SLOW FOR LARGER NUMBER
    void createMinTree(unsigned int x, unsigned int y);
    //creats the min span tree using prims with priority queue
    void MST(unsigned int x, unsigned int y);
    void placeTextures();
    void newBoard(int rows, int columns);




public:

    void drawPath(sf::RenderWindow& window, std::vector<unsigned int>& path);

    void setTexture();
    void printMaze(sf::RenderWindow& window);

    Graph();
    Graph(unsigned int width, unsigned int height, bool debug); //w and h abd debug
    //creates the maze
    void initalizeMaze();
    //returns the maze node* at given x,y
    Node* getMazeNode(unsigned int x, unsigned int y);
    //returns a vector of adjacent node pointers
    std::vector<Node*> getAdjacent(Node* parentNode);


    //used to flip debug on and off DEFAULT: false with debug the weights for the graph uses the _seed
    void debug();
    //used to test if debug on and off
    bool isDebug();
    //cycle detection uses BFS
    bool isCyclic();
    bool isCyclic(unsigned int v, std::vector<bool>& visited, unsigned int parent);
    //used to input seed, written test cases use the DEFAULT debuging seed; 1
    void setSeed(int seed);
    //prints edge list of maze graph
    void printMazeEdges();
    //returns the maze size
    unsigned int getMazeSize();
    void setSpriteSize(unsigned int d);
    unsigned int getSpriteSize();
};