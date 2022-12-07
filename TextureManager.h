#pragma once
//
// Created by Camila Morales on 7/23/22.
//

#ifndef MINESWEEPER_TEXTUREMANAGER_H
#define MINESWEEPER_TEXTUREMANAGER_H

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
using namespace sf;
using namespace std;
using std::unordered_map;
using std::string;


class TextureManager
{
    // only one of these in memory
    static unordered_map<string, sf::Texture> textures;
    static void LoadTexture(string fileName);
public:
    static Texture& GetTexture(string textureName);
    static void Clear(); // call this at the end of main
};


#endif //MINESWEEPER_TEXTUREMANAGER_H
