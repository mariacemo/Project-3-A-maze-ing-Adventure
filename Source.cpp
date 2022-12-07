#include <SFML/Graphics.hpp>
#include "TextureManager.h"
#include "Graph.h"
#include "Dijkstras.h"
#include "BFS.h"
#include "Character.h"
#include <iostream>
#include <time.h>

#define WIDTH_MAIN_MENU 256
#define HEIGHT_MAIN_MENU 256
#define BUTTON_WIDTH 100
#define BUTTON_HEIGHT 50
#define FBUTTON_POSX 88
#define FBUTTON_POSY 160
#define OBUTTON_POSX 87
#define OBUTTON_POSY 190

char MainMenu();
void OptimizedMaze();
void FancyMaze();
void ScoreBoard(float BFS, float dijk);

int main() {

    switch (MainMenu()) {
    case 0:
        FancyMaze();
        break;
    case 1:
        OptimizedMaze();
        break;
    }

    return 0;
}
char MainMenu() {
    sf::RenderWindow window(sf::VideoMode((unsigned int)WIDTH_MAIN_MENU, (unsigned int)HEIGHT_MAIN_MENU), "Choices, Choices");
    sf::Sprite background;
    background.setTexture(TextureManager::GetTexture("menu screen 1"));

    window.setSize(Vector2u(1024, 1024));

    sf::Sprite fancyButton;
    fancyButton.setTexture(TextureManager::GetTexture("game button"));
    fancyButton.setPosition(FBUTTON_POSX, FBUTTON_POSY);

    sf::Sprite optimizedButton;
    optimizedButton.setTexture(TextureManager::GetTexture("maze button"));
    optimizedButton.setPosition(OBUTTON_POSX, OBUTTON_POSY);

    sf::Mouse mouse;
    while (window.isOpen()) {

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (mouse.isButtonPressed(sf::Mouse::Left))

                std::cout << mouse.getPosition(window).x << ", " << mouse.getPosition(window).y << std::endl;

            if (mouse.isButtonPressed(sf::Mouse::Left) && mouse.getPosition(window).x < 645 && mouse.getPosition(window).x > 376)
                if (mouse.getPosition(window).y <  863 && mouse.getPosition(window).y > 773) {
                    window.close();
                    std::cout << "OPTIMIZED\n";
                    return 1;
                }
                else if (mouse.getPosition(window).y < 733 && mouse.getPosition(window).y > 650) {
                    window.close();
                    std::cout << "FANCY\n";
                    return 0;
                }
        }
        window.clear();
        window.draw(background);
        window.draw(fancyButton);
        window.draw(optimizedButton);
        
        window.display();
    }
    TextureManager::Clear();
    return 0;
}

void FancyMaze() {
    unsigned int w, h;
    w = 15;
    h = 15;
    Graph g1(w, h, false);
    g1.setSpriteSize(64);
    sf::RenderWindow window(sf::VideoMode(w * g1.getSpriteSize(), h * g1.getSpriteSize()), "Fancy!");

    sf::Sprite chest;
    chest.setTexture(TextureManager::GetTexture("treasure chest"));
    chest.setPosition(896, 896);
    clock_t mazet = clock();
    g1.initalizeMaze();
    mazet = clock() - mazet;
    std::cout << (double)mazet / CLOCKS_PER_SEC << "s\n";


    Dijkstras d(g1, w, h);
    clock_t dijkt = clock();
    d.dijkstrAlgorithm();
    std::vector<unsigned int> dijk = d.getShortestPath(g1.getMazeNode(w - 1, h - 1)->_nodeNum);
    dijkt = clock() - dijkt;
    std::cout << (double)dijkt / CLOCKS_PER_SEC << "s\n";

    BFS b;
    clock_t BFSt = clock();
    std::vector<unsigned int> BFS = b.BFSAlgo(g1, g1.getMazeNode(w - 1, h - 1)->_nodeNum);
    BFSt = clock() - BFSt;
    std::cout << (double)BFSt / CLOCKS_PER_SEC << "s\n";

    Character mc(0, 150, w, "protagonist");
    unsigned int nodeIndex = 0;


    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (!mc.atDest(BFS.at(nodeIndex)))
            mc.move(BFS.at(nodeIndex));
        else if (nodeIndex != BFS.size()-1)
            nodeIndex += 1;

        if (mc.atDest(BFS.at(BFS.size() - 1))) {
            mc.WIN();
        }


        window.clear();
        g1.printMaze(window);
        window.draw(chest);
        mc.drawCharacter(window);
        window.display();
    }
    TextureManager::Clear();
    ScoreBoard((float)BFSt / CLOCKS_PER_SEC, (float)dijkt / CLOCKS_PER_SEC);
}

void OptimizedMaze() {
    unsigned int w, h;
    w = 317;
    h = 317;
    Graph g1(w, h, false);
    g1.setSpriteSize(3);
    sf::RenderWindow window(sf::VideoMode(w * g1.getSpriteSize(), h * g1.getSpriteSize()), "WOW!");

    clock_t mazet = clock();
    g1.initalizeMaze();
    mazet = clock() - mazet;
    std::cout << (double)mazet / CLOCKS_PER_SEC << "s\n";


    Dijkstras d(g1, w, h);
    clock_t dijkt = clock();
    d.dijkstrAlgorithm();
    std::vector<unsigned int> dijk = d.getShortestPath(g1.getMazeNode(w - 1, h - 1)->_nodeNum);
    dijkt = clock() - dijkt;
    std::cout << (double)dijkt / CLOCKS_PER_SEC << "s\n";

    BFS b;
    clock_t BFSt = clock();
    std::vector<unsigned int> BFS = b.BFSAlgo(g1, g1.getMazeNode(w - 1, h - 1)->_nodeNum);
    BFSt = clock() - BFSt;
    std::cout << (double)BFSt / CLOCKS_PER_SEC << "s\n";

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();
        g1.printMaze(window);
        g1.drawPath(window, dijk);
        window.display();
    }
    TextureManager::Clear();

    ScoreBoard((float)BFSt / CLOCKS_PER_SEC, (float)dijkt / CLOCKS_PER_SEC);
}

void ScoreBoard(float BFS, float dijk) {
    sf::RenderWindow window(sf::VideoMode((unsigned int)512, (unsigned int)512), "High Score?!");

    std::string scoreBFS = "BFS: " + std::to_string(BFS).substr(0,5) + "s";
    std::string scoreDijk = "Dijk's: " + std::to_string(dijk).substr(0, 5) + "s";

    window.setSize(Vector2u(1024, 1024));
    
    sf::Sprite background;
    sf::Font scoreFont;

    sf::Sprite trophy;
    trophy.setTexture(TextureManager::GetTexture("trophy"));
    scoreFont.loadFromFile("images/ka1.ttf");
    
    background.setTexture(TextureManager::GetTexture("scoreboard"));

    trophy.setScale(.1, .1);

    if (BFS < dijk)
        trophy.setPosition(32, 1024 / 5 - 20);
    else
        trophy.setPosition(32, 1024 / 5 + 108);

    sf::Text score1 (scoreBFS, scoreFont);
    sf::Text score2(scoreDijk, scoreFont);

    score1.setPosition(128, 1024 / 5);
    score2.setPosition(128, 1024 / 5 + 128);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();

        window.draw(background);
        window.draw(score1);
        window.draw(score2);
        window.draw(trophy);
        window.display();
    }
    TextureManager::Clear();
}