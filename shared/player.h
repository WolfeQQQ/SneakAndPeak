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
    int getSpeed() const;
    int getId() const;
    bool getIsCaught() const;
    bool getIsSeeker() const;
    bool getIsConnected() const;
    ClientInput getClientInput() const;

    //Setters
    void setX(float newX);
    void setY(float newY);
    void setStamina(float newStamina);
    void setAbilityCooldown(float newAbilityCooldown);
    void setSpeed(int newSpeed);
    void setIsCaught(bool changedCaught);
    void setIsSeeker(bool changedSeeker);
    void setIsConnected(bool changedConnectec);
    void setId(int newId);
    void setInput(ClientInput& newInput);


private:
    //Player Stats
    float x;
    float y;
    int id;
    int speed;
    float stamina;
    bool isCaught;
    bool isSeeker;
    bool isConnected;
    float abilityCooldown;

    //Client Input
    ClientInput input;

};
