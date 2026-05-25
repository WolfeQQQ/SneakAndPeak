#include "tilemap.h"
#include <fstream>
#include <sstream>
#include <iostream>

// Constructor, initializes empty structure
Tilemap::Tilemap() : tileSize(0) {
    tileSet = { 0 };
}

//Destructor, unloads resources
Tilemap::~Tilemap() {
    unload();
}

// Loads the tilemap from a CSV file and tileset structure
// Returns if it was successful or not
bool Tilemap::load(const std::string& mapPath, const std::string& texturePath, int tileSize) {
    this -> tileSize = tileSize;
    tileSet = LoadTexture(texturePath.c_str());

    // Check if the texture was loaded successfully
    // Throws an error if it fails
    if(tileSet.id == 0){
        std::cerr << "Failed to load tile set texture: " << texturePath << std::endl;
        return false;
    }

    // Open the CSV file containing the tilemap data
    // Throws an error if the file cannot be opened
    std::ifstream file(mapPath);
    if(!file.is_open()){
        std::cerr << "Failed to open map file: " << mapPath << std::endl;
        unload();
        return false;
    }

    // Read the CSV file line by line and parse IDs into the mapData Vector
    std::string line;
    while(std::getline(file,line)){
        std::vector<int> row;
        std::stringstream ss(line);
        std::string value;

        while(std::getline(ss, value ,',')){
            if(!value.empty()){
                row.push_back(std::stoi(value));
            }
        }
        if(!row.empty()){
                mapData.push_back(row);
        }
    }

    file.close();
    return true; // return true if the tilemap was loaded successfully
}

// Draws the tilemap to the screen using the loaded tileset texture
void Tilemap::draw() { 

    // Check if the tileSet texture is loaded and mapData is not empty before attempting to draw
    if(tileSet.id == 0 || mapData.empty()){
        return;
    }

    int perRow = tileSet.width / tileSize;  // Calculate how many tiles fit in one row of the texture

    for(int y = 0; y < mapData.size(); y++){
        for(int x = 0; x < mapData[y].size(); x++){
            int tileID = mapData[y][x];

            if(tileID < 0){
                continue;
            }

            int tileX = ((tileID) % perRow) * tileSize;
            int tileY = ((tileID) / perRow) * tileSize;

            Rectangle source = { (float)tileX, (float)tileY, (float)tileSize, (float)tileSize };    // Select the correct tile from the tileset texture

            // Calculate the on-screen position of the tile
            float destX = (float)(x * tileSize);
            float destY = (float)(y * tileSize);

            DrawTextureRec(tileSet, source, (Vector2){destX, destY}, WHITE); // Draw the tile to the screen
        }
    }
}

// Unloads the tilemap resources, including the tileset texture
void Tilemap::unload() {
    if(tileSet.id != 0){
        UnloadTexture(tileSet);
        tileSet = { 0 };
    }
    mapData.clear();
}

// Checks if a given coordinate is a solid tile
// Used for raycasting
bool Tilemap::isSolid(float x, float y){
    // Convert world coordinates to tile coordinates
    int tileX = (int)(x / tileSize); 
    int tileY = (int)(y / tileSize);

    // Check if the coordinates are within the bounds of the mapData
    if (tileY < 0 || tileY >= mapData.size()) return true;
    if (tileX < 0 || tileX >= mapData[tileY].size()) return true;

    return mapData[tileY][tileX] != -1; // For now, only 'id = -1' is considered a non-solid tile
}