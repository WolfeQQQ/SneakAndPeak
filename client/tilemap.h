#pragma once
#include <raylib.h>
#include <vector>
#include <string>

/**
 * @class Tilemap
 * @brief Manages loading, rendering and raycasting for the tilemap
 */
class Tilemap {
    private:
        Texture2D tileSet;  // Texture containing all tile sprites
        int tileSize;       // Size of each tile in pixels
        std::vector<std::vector<int>> mapData;  // 2D vector storing the tile IDs, loaded from the CSV file

    public:
        Tilemap(); // Constructor, initializes empty state
        ~Tilemap(); // Destructor, unloads resources

        /**
         * @brief Loads the tilemap from a CSV file and the tileset texture.
         * @param mapPath Path to the CSV file containing the tilemap data.
         * @param texturePath Path to the texture file containing all tile sprites.
         * @param tileSize Size of each tile in pixels.
         * @return true if the tilemap was loaded successfully, false otherwise.
         */
        bool load(const std::string& mapPath, const std::string& texturePath, int tileSize); 

        /**
         * @brief Draws the tilemap to the screen using the loaded tileset texture.
         */
        void draw();

        /**
         * @brief Unloads the tilemap resources, including the tileset texture.
         */
        void unload();

        /**
         * @brief Checks if a given world coordinate is solid (i.e., collides with a wall tile).
         * @param x The x-coordinate in world space to check.
         * @param y The y-coordinate in world space to check.
         * @return true if the coordinate is solid, false otherwise.
         */
        bool isSolid(float x, float y);

        bool isWall(int tileId);

};