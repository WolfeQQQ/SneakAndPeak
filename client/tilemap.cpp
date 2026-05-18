#include "tilemap.h"
#include <fstream>
#include <sstream>
#include <iostream>

Tilemap::Tilemap() : tileSize(0) {
    tileSet = { 0 };
}

Tilemap::~Tilemap() {
    unload();
}

bool Tilemap::load(const std::string& mapPath, const std::string& texturePath, int tileSize) {
    this -> tileSize = tileSize;
    tileSet = LoadTexture(texturePath.c_str());
    if(tileSet.id == 0){
        std::cerr << "Failed to load tile set texture: " << texturePath << std::endl;
        return false;
    }

    std::ifstream file(mapPath);
    if(!file.is_open()){
        std::cerr << "Failed to open map file: " << mapPath << std::endl;
        unload();
        return false;
    }

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
    return true;
}


void Tilemap::draw() { 
    if(tileSet.id == 0 || mapData.empty()){
        return;
    }

    int perRow = tileSet.width / tileSize;

    for(int y = 0; y < mapData.size(); y++){
        for(int x = 0; x < mapData[y].size(); x++){

            int tileID = mapData[y][x];

            if(tileID < 0){
                continue;
            }
            int tileX = ((tileID) % perRow) * tileSize;
            int tileY = ((tileID) / perRow) * tileSize;

            Rectangle source = { (float)tileX, (float)tileY, (float)tileSize, (float)tileSize };

            float destX = (float)(x * tileSize);
            float destY = (float)(y * tileSize);

            DrawTextureRec(tileSet, source, (Vector2){destX, destY}, WHITE);
        }
    }


}

void Tilemap::unload() {
    if(tileSet.id != 0){
        UnloadTexture(tileSet);
        tileSet = { 0 };
    }
    mapData.clear();
}