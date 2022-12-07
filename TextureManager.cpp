//
// Created by Camila Morales on 7/23/22.
//

#include "TextureManager.h"

// redeclare the variable so the file knows it exists
// do this for the static variables
std::unordered_map <string, Texture> TextureManager::textures;

void TextureManager::LoadTexture(string fileName)
{
    string path = "images/";
    path += fileName + ".png";
    textures[fileName].loadFromFile(path);
}

Texture& TextureManager::GetTexture(string textureName)
{
    if (textures.find(textureName) == textures.end()) // didnt find it
        LoadTexture(textureName);
    return textures[textureName];
}

void TextureManager::Clear()
{
    textures.clear(); //gets rid of all stored objects
}
