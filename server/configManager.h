#pragma once
#include <string>
#include "../shared/contstants.h"

class configManager {
private:
    int port;
    int tickDelayMs;

public:
configManager();

bool loadConfig(const std::string& filename);

int getPort() const {return port;}
int getTickDelay() const {return tickDelayMs;}
};