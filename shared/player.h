#pragma once

// struct ClientInput {
//     bool up;
//     bool down;
//     bool right;
//     bool left;
//     bool shift;
//     //bool e_key;
//     };

class player
{   
public:
    //Constructor, Deconstructor
    player();
    ~player();

    enum class Direction
    {
        DOWN = 0,
        UP,
        LEFT,
        RIGHT
    };

    enum class PlayerState{
        IDLE,
        WALK,
        DEATH
    };

    //Client Input Definition
    struct ClientInput
    {
        bool up;
        bool down;
        bool right;
        bool left;
        bool shift;
        //bool eKey;
    };

    //Getters
    float getX() const;
    float getY() const;
    float getStamina() const;
    float getAbilityCooldown() const;
    float getSpeed() const;
    int getId() const;
    bool getIsCaught() const;
    bool getIsSeeker() const;
    bool getIsConnected() const;
    bool getIsRunning() const;
    ClientInput getClientInput() const;
    Direction getDirection() const;
    PlayerState getPlayerState() const;

    //Setters
    void setX(float newX);
    void setY(float newY);
    void setStamina(float newStamina);
    void setAbilityCooldown(float newAbilityCooldown);
    void setSpeed(float newSpeed);
    void setIsCaught(bool changedCaught);
    void setIsSeeker(bool changedSeeker);
    void setIsConnected(bool changedConnectec);
    void setIsRunning(bool changedRunning);
    void setId(int newId);
    void setInput(ClientInput& newInput);
    void setDirection(Direction newDirection);
    void setPlayerState(PlayerState newPlayerState);


private:
    //Player Stats
    float x;
    float y;
    int id;
    float speed;
    float stamina;
    bool isCaught;
    bool isSeeker;
    bool isConnected;
    bool isRunning;
    float abilityCooldown;

    //Client Input
    ClientInput input;
    //Direction
    Direction direction;
    //PlayerState
    PlayerState playerstate;
};
