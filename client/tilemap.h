#pragma once
#include <raylib.h>
#include <vector>
#include <string>

class Tilemap {
    private:
        Texture2D tileSet;
        int tileSize;
        std::vector<std::vector<int>> mapData;

    public:
        Tilemap();
        ~Tilemap();

        bool load(const std::string& mapPath, const std::string& texturePath, int tileSize);

        void draw();
        void unload();

};