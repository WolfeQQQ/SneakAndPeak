#include "configManager.h"
#include <fstream>
#include <iostream>
#include <sstream>

configManager::configManager(){
    port = 5000;
    tickDelayMs = 16;
}

bool configManager::loadConfig(const std::string& filename){

    std::ifstream inFile(filename);

    if(!inFile.is_open()){
        std::ofstream outFile(filename);
        if(outFile.is_open()){
            outFile << "port=5000\n";
            outFile << "tickDelayMs=16\n";
            outFile.close();
            std::cout << "Made new config file " << filename << "\n";
        }
        return true;
    }

    std::string line;
    while(std::getline(inFile,line)){
        if(line.empty() || line[0] == '#'){ continue;}
    
        std::stringstream info_in_line(line);
        std::string key;
        if (std::getline(info_in_line, key, '=')) {
            std::string valueStr;
            if (std::getline(info_in_line, valueStr)) {
                int value = std::stoi(valueStr);
                    
                if (key == "port") {port = value;}
                else if (key == "tickDelayMs") {tickDelayMs = value;}
            }
        }
    }
    inFile.close();
    return true;
}

