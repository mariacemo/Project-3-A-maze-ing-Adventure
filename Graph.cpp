#include "Graph.h"

Graph::Graph() {}

Graph::Node::Node(std::string textureName) {
    sprite.setTexture(TextureManager::GetTexture(textureName));
}

void Graph::Node::drawNode(sf::RenderWindow& window, unsigned int width, unsigned int spriteSize) {
    this->sprite.setPosition((_nodeNum % width) * spriteSize, (_nodeNum / width) * spriteSize);
    this->attribute.setPosition((_nodeNum % width) * spriteSize + spriteSize/2, (_nodeNum / width) * spriteSize + spriteSize /2);
    window.draw(sprite);
}

Graph::Graph(unsigned int width, unsigned int height, bool debug)
{
    _width = width;
    _height = height;
    _debug = debug;
    _seed = 1;

    for (unsigned int i = 0; i < (_width * _height); ++i)
        mazeMap[i].setNodeNum(i);

}
void Graph::Node::setNodeNum(unsigned int n)
{
    _nodeNum = n;
}
Graph::Node::Node()
{
    _nodeNum = 0;
}

void Graph::initalizeGraph(std::unordered_map<unsigned int, Node>& nodeMap)
{
    for (unsigned int i = 0; i < (_width * _height); ++i)
    {
        nodeMap[i];
        nodeMap.at(i).setNodeNum(i);
    }
    setRand();
    connectVertices(nodeMap);
    // place textures
}
void Graph::setRand()
{
    if (!_debug)
        _seed = time(NULL);
    srand(_seed);
}
void Graph::connectVertices(std::unordered_map<unsigned int, Node>& nodeMap)
{
    for (unsigned int i = 0; i < (_width * _height); ++i)
    {
        if (i / _width != 0)
            nodeMap[i].adjacentNodes.push_back(std::make_pair(&nodeMap.at(i - _width), genWeight()));//genWeight grandomizes the maze by chaning the MST
        else
            nodeMap[i].adjacentNodes.push_back(std::make_pair(nullptr, genWeight()));//top edge
        if (i % _width != (_width - 1))
            nodeMap[i].adjacentNodes.push_back(std::make_pair(&nodeMap.at(i + 1), genWeight()));
        else
            nodeMap[i].adjacentNodes.push_back(std::make_pair(nullptr, genWeight()));//right edge
        if (i / _width != _height - 1)
            nodeMap[i].adjacentNodes.push_back(std::make_pair(&nodeMap.at(i + _width), genWeight()));
        else
            nodeMap[i].adjacentNodes.push_back(std::make_pair(nullptr, genWeight()));//bottom edge
        if (i % _width != 0)
            nodeMap[i].adjacentNodes.push_back(std::make_pair(&nodeMap.at(i - 1), genWeight()));
        else
            nodeMap[i].adjacentNodes.push_back(std::make_pair(nullptr, genWeight()));//left edge
    }
}
unsigned char Graph::genWeight()
{
    return rand() % 254;
}

Graph::Node* Graph::getNode(unsigned int x, unsigned int y, std::unordered_map<unsigned int, Node>& nodeMap)
{
    return &nodeMap.at(x + (y * _width));
}
void Graph::createMinTree(unsigned int x, unsigned int y)
{
    //outdated, still works fine can be used to testing
    std::unordered_map<unsigned int, Node> nodeMap;
    initalizeGraph(nodeMap);
    Node* sourceNode = getNode(x, y, nodeMap);
    //unoptimized works fine for smaller mazes
    std::set<Node*> visited;
    std::set<Node*> notVisited;
    for (unsigned int i = 0; i < nodeMap.size(); ++i)
        notVisited.insert(&nodeMap.at(i));
    notVisited.erase(sourceNode);
    visited.insert(sourceNode);
    unsigned int k = 0;
    while (notVisited.size() != 0)
    {
        unsigned int  min = UINT_MAX;
        unsigned int position = 4;
        Node* toNode = nullptr;
        Node* fromNode = nullptr;
        for (auto x : visited)
            for (unsigned int i = 0; i < 4; ++i)
            {
                unsigned int cost = x->adjacentNodes.at(i).second;
                if (x->adjacentNodes.at(i).first != nullptr && visited.count(x->adjacentNodes.at(i).first) == 0 && cost < min)
                {
                    min = cost;
                    position = i;
                    toNode = x->adjacentNodes.at(i).first;
                    fromNode = x;
                }
            }
        if (toNode != nullptr && fromNode != nullptr)
        {
            //zeroing out the extra nodes
            mazeMap[fromNode->_nodeNum].adjacentNodes.resize(4, std::make_pair(nullptr, 255));
            mazeMap[fromNode->_nodeNum].setNodeNum(fromNode->_nodeNum);
            mazeMap[toNode->_nodeNum].adjacentNodes.resize(4, std::make_pair(nullptr, 255));
            mazeMap[toNode->_nodeNum].setNodeNum(toNode->_nodeNum);

            mazeMap[fromNode->_nodeNum].adjacentNodes.at(position) = (std::make_pair(toNode, 1));
            position += 2;
            position = position % 4;
            mazeMap[toNode->_nodeNum].adjacentNodes.at(position) = std::make_pair(&mazeMap.at(fromNode->_nodeNum), 1);
            notVisited.erase(toNode);
            visited.insert(toNode);
            ++k;
            if (k % 100 == 0)
                std::cout << k << std::endl;;
        }
    }

}
void Graph::MST(unsigned int x, unsigned int y)
{
    //OPTIMIZED PRIM"S CAN BE BETTER
    std::unordered_map<unsigned int, Node> nodeMap;
    initalizeGraph(nodeMap);
    Node* sourceNode = getNode(x, y, nodeMap);

    std::priority_queue <std::pair<unsigned int, unsigned int>, std::vector<std::pair<unsigned int, unsigned int>>, std::greater<std::pair<unsigned int, unsigned int>>> pq;
    std::vector<unsigned int> key(nodeMap.size(), UINT_MAX);//used for cost values
    std::vector<bool> inMST(nodeMap.size(), false);//used for knowing which nodes have been visited
    std::vector<std::pair<Node*, char>> relativePos(nodeMap.size(), std::make_pair(nullptr, UINT_MAX));
    pq.push(std::make_pair(0, sourceNode->_nodeNum));//fist source node
    key[sourceNode->_nodeNum] = 0;

    while (!pq.empty())
    {
        unsigned int u = pq.top().second;
        pq.pop();

        if (inMST[u] == true)
            continue;

        inMST[u] = true;

        unsigned int nextNodeId = 0;
        for (char i = 0; i < 4; ++i)
        {
            //for edges of maze
            if (nodeMap.at(u).adjacentNodes.at(i).first == nullptr)
                continue;

            //variables used to get weights and vertices for each adjacent vertex
            unsigned int v = nodeMap.at(u).adjacentNodes.at(i).first->_nodeNum;
            unsigned int w = nodeMap.at(u).adjacentNodes.at(i).second;

            //finds smallest key over the 4 neighboring nodes
            if (inMST[v] == false && w < key[v])
            {
                key[v] = w;
                pq.push(std::make_pair(key[v], v));
                relativePos.at(v) = std::make_pair(&mazeMap.at(u), i);
            }
        }
    }

    for (unsigned int i = 0; i < nodeMap.size(); ++i)
    {
        //giving the adjacent arrays their sizes; sizes important for knowing which direction
        if (relativePos.at(i).first == nullptr)
            continue;
        mazeMap[relativePos.at(i).first->_nodeNum].adjacentNodes.resize(4, std::make_pair(nullptr, UINT_MAX));
        mazeMap[i].adjacentNodes.resize(4, std::make_pair(nullptr, UINT_MAX));

        //sets up the connections in the maze map.
        char reverseConnection = (relativePos[i].second + 2) % 4;
        mazeMap[relativePos.at(i).first->_nodeNum].adjacentNodes.at(relativePos[i].second) = std::make_pair(&mazeMap.at(i), 1);
        mazeMap[i].adjacentNodes.at(reverseConnection) = std::make_pair(relativePos.at(i).first, 1);
    }
}
//PUBLIC
//initalizations

//void Graph::setTexture(unordered_map<unsigned int, Node> &nodeMap, sf::RenderWindow& window)
void Graph::drawPath(sf::RenderWindow& window, std::vector<unsigned int>& path) {
    for (unsigned int i = 0; i < path.size(); i++) {
        if (_spriteSize == 3)
            mazeMap.at(path.at(i)).attribute.setTexture(TextureManager::GetTexture("visited"));
        else
            mazeMap.at(path.at(i)).attribute.setTexture(TextureManager::GetTexture("visitedL"));
        window.draw(mazeMap.at(path.at(i)).attribute);
    }
}
void Graph::setTexture()
{
    for (auto it = mazeMap.begin(); it != mazeMap.end(); it++)
    {
        // get individual node
        bool up = false;
        bool down = false;
        bool right = false;
        bool left = false;

        Node* parent = &(it->second);
        std::vector<Node*> neighbors = getAdjacent(parent);

        if (neighbors[0] != nullptr) {
            up = true;
        }
        if (neighbors[1] != nullptr) {
            right = true;
        }
        if (neighbors[2] != nullptr) {
            down = true;
        }
        if (neighbors[3] != nullptr) {
            left = true;
        }

        // ------------- ALL 4 DIRECTIONS = TRUE -------------
        if (up && right && down && left) {
            if (_spriteSize == 64)
                parent->sprite.setTexture(TextureManager::GetTexture("4 way pathL"));
            else
                parent->sprite.setTexture(TextureManager::GetTexture("4 way path"));

        }

        // ------------- ONLY 3 DIRECTIONS = TRUE -------------

// can NOT go up
        else if (!up && right && down && left) {
            if (_spriteSize == 64)
                parent->sprite.setTexture(TextureManager::GetTexture("3 way downwardL"));
            else
                parent->sprite.setTexture(TextureManager::GetTexture("3 way downward"));
        }
        // can not go right
        else if (up && !right && down && left) {
            if (_spriteSize == 64)
                parent->sprite.setTexture(TextureManager::GetTexture("3 way leftway pathL"));
            else
                parent->sprite.setTexture(TextureManager::GetTexture("3 way leftway path"));
        }
        // can not go down
        else if (up && right && !down && left) {
            if (_spriteSize == 64)
                parent->sprite.setTexture(TextureManager::GetTexture("3 way upwardL"));
            else
                parent->sprite.setTexture(TextureManager::GetTexture("3 way upward"));
        }
        // can not go left
        else if (up && right && down && !left) {
            if (_spriteSize == 64)
                parent->sprite.setTexture(TextureManager::GetTexture("3 way rightward pathL"));
            else
                parent->sprite.setTexture(TextureManager::GetTexture("3 way rightward path"));

        }

        // ------------- ONLY 2 DIRECTIONS = TRUE -------------

        else if (!up && right && !down && left) {
            // tile goes from left to right
            if (_spriteSize == 64)
                parent->sprite.setTexture(TextureManager::GetTexture("left to rightL"));
            else
                parent->sprite.setTexture(TextureManager::GetTexture("left to right"));
        }
        else if (up && !right && down && !left) {
            // tile goes from up and down
            if (_spriteSize == 64)
                parent->sprite.setTexture(TextureManager::GetTexture("PathL"));
            else
                parent->sprite.setTexture(TextureManager::GetTexture("Path"));
        }
        // corner pieces
        else if (!up && right && down && !left) {
            // |  ------
            // |  ------
            // | |
            // | |
            if (_spriteSize == 64)
                parent->sprite.setTexture(TextureManager::GetTexture("2 way rightwardL"));
            else
                parent->sprite.setTexture(TextureManager::GetTexture("2 way rightward"));
        }
        else if (!up && !right && down && left) {
            // ------  |
            // ------  |
            //       | |
            //       | |
            if (_spriteSize == 64)
                parent->sprite.setTexture(TextureManager::GetTexture("2 way downwardL"));
            else
                parent->sprite.setTexture(TextureManager::GetTexture("2 way downward"));
        }
        else if (up && !right && !down && left) {
            //      |
            //      |
            // ---- |
            // ---- |
            if (_spriteSize == 64)
                parent->sprite.setTexture(TextureManager::GetTexture("2 way up and leftL"));
            else
                parent->sprite.setTexture(TextureManager::GetTexture("2 way up and left")); 
        }
        else if (up && right && !down && !left) {
            // |
            // |
            // |  ----
            // |  -----
            if (_spriteSize == 64)
                parent->sprite.setTexture(TextureManager::GetTexture("2 way up and rightL"));
            else
                parent->sprite.setTexture(TextureManager::GetTexture("2 way up and right"));
        }

        // ------------- ONLY 1 DIRECTION = TRUE -------------

        else if (up && !right && !down && !left) {
            if (_spriteSize == 64)
                parent->sprite.setTexture(TextureManager::GetTexture("1 way upL"));
            else
                parent->sprite.setTexture(TextureManager::GetTexture("1 way up"));
        }
        else if (!up && right && !down && !left) {
            if (_spriteSize == 64)
                parent->sprite.setTexture(TextureManager::GetTexture("1 way rightL"));
            else
                parent->sprite.setTexture(TextureManager::GetTexture("1 way right"));


        }
        else if (!up && !right && down && !left) {
             if (_spriteSize == 64)
                 parent->sprite.setTexture(TextureManager::GetTexture("1 way downL"));
             else
                parent->sprite.setTexture(TextureManager::GetTexture("1 way down"));
        }
        else if (!up && !right && !down && left) {
             if (_spriteSize == 64)
                 parent->sprite.setTexture(TextureManager::GetTexture("1 way leftL"));
             else
                parent->sprite.setTexture(TextureManager::GetTexture("1 way left"));
           

        }
    }
}

void Graph::printMaze(sf::RenderWindow& window)
{
    for (auto it = mazeMap.begin(); it != mazeMap.end(); it++)
    {
        // print out all of the tiles
        it->second.drawNode(window, _width, _spriteSize);
    }
}
std::vector<Graph::Node*> Graph::getAdjacent(Graph::Node* node)
{
    std::vector<Graph::Node*> result;
    for (auto x : node->adjacentNodes)
        result.push_back(x.first);
    return result;
}
Graph::Node* Graph::getMazeNode(unsigned int x, unsigned int y)
{
    return &mazeMap.at(x + (y * _width));
}
void Graph::initalizeMaze()
{
    MST(0, 0);
    setTexture();
}

//debug tools
void Graph::debug()
{
    _debug = !_debug;
}
bool Graph::isDebug()
{
    if (_debug)
        return true;
    return false;
}
bool Graph::isCyclic()
{
    //cyclic detector editied from geeks for geeks
    //only used in tests cases to confirm graph
    unsigned int v = mazeMap.size();
    std::vector<bool> visited(v, false);
    for (unsigned int i = 0; i < v; ++i)
        if (!visited.at(i))
            if (isCyclic(i, visited, -1))
                return true;
    return false;
}
bool Graph::isCyclic(unsigned int v, std::vector<bool>& visited, unsigned int parent)
{
    visited.at(v) = true;

    for (char i = 0; i < 4; ++i)
    {
        if (mazeMap.at(v).adjacentNodes.at(i).first != nullptr && !visited[mazeMap.at(v).adjacentNodes.at(i).first->_nodeNum])
        {
            if (isCyclic(mazeMap.at(v).adjacentNodes.at(i).first->_nodeNum, visited, v))
                return true;
        }
        else if (mazeMap.at(v).adjacentNodes.at(i).first != nullptr && mazeMap.at(v).adjacentNodes.at(i).first->_nodeNum != parent)
            return true;
    }
    return false;
}
void Graph::setSeed(int s)
{
    _seed = s;
}
void Graph::printMazeEdges()
{
    for (auto x : mazeMap)
        for (auto y : x.second.adjacentNodes)
            if (y.first != nullptr)
                std::cout << "Edge: (" << x.first << ", " << y.first->_nodeNum << ")" << std::endl;

}
unsigned int Graph::getMazeSize()
{
    return mazeMap.size();
}
void Graph::setSpriteSize(unsigned int d) {
    _spriteSize = d;
}
unsigned int Graph::getSpriteSize() {
    return _spriteSize;
}