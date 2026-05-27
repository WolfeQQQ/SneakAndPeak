#pragma once 

#include <string>
#include "../shared/player.h"
#include "../shared/gameState.h"

/**
 * @class Network
 * @brief Handles all TCP socket communication between the client and the game server.
 */
class Network{
    private:
        int sock; // Socket file descriptor
        bool connected; // Connection status

    public:
        Network();  // Constructor, initializes the network object
        ~Network(); // Destructor, closes the socket if open

        /**
        * @brief Attempts to connect to the game server.
        * * @param ip The IP address of the server as a string.
        * @param port The port number to connect to.
        * @param[out] playerId Pointer to an integer where the assigned player ID will be stored.
        * @return true if the connection was successful, false otherwise.
        */
        bool ConnectToServer(const std::string& ip, int port, int *playerId);

        /**
         * @brief Closes the connection to the server and cleans up resources.
         */
        void Disconnect();

        /**
         * @brief Checks if the client is currently connected to the server.
         * @return true if connected, false otherwise.
         */
        bool IsConnected() const { return connected; }

        /**
         * @brief Sends the player's input to the server.
         * @param input The ClientInput structure containing the player's current input state.
         */
        void SendInput(player::ClientInput input);

        /**
         * @brief Receives the current game state from the server.
         * @param[out] packet Reference to a GameStatePacket structure where the received game state will be stored.
         * @return true if the game state was successfully received, false otherwise.
         */
        bool ReceiveState(GameStatePacket& packet);

};