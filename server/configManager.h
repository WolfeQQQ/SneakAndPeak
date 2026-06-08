#pragma once
#include <string>
#include "../shared/contstants.h"

class configManager {
private:
    int port;
    int tickDelayMs;

public:
/**
     * @brief Constructs a new config Manager object with default values.
*/
configManager();

/**
     * @brief Loads server configuration from a specified text file.
     * @param filename The path to the configuration file.
     * @return true if the configuration was loaded successfully.
     * @return false if an error occurred while loading or parsing the file.
*/
bool loadConfig(const std::string& filename);

/**
     * @brief Gets the network port configured for the server.
     * @return int The server port.
*/
int getPort() const {return port;}

/**
     * @brief Gets the configured tick delay for the server loop.
     * @return int The delay in milliseconds.
*/
int getTickDelay() const {return tickDelayMs;}
};