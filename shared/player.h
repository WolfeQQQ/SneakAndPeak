<<<<<<< HEAD
#pragma once

class player
{
private:
    //Player Stats
    float x;
    float y;
    int speed;
    float stamina;
    bool isCaught;
    bool isSeeker;
    bool isConnected;
    float abilityCooldown;

    //Client Input
    bool up;
    bool down;
    bool right;
    bool left;
    bool shift;
    //bool e_key;
    
public:
    //Constructor, Deconstructor
    player();
    ~player();

    //Getters
    float getX() const;
    float getY() const;
    float getStamina() const;
    float getAbilityCooldown() const;
    int getSpeed() const;
    bool getIsCaught() const;
    bool getIsSeeker() const;
    bool getIsConnected() const;

    //Setters
    void setX(float);
    void setY(float);
    void setStamina(float);
    void setAbilityCooldown(float);
    void setSpeed(int);
    void setIsCaught(bool);
    void setIsSeeker(bool);
    void setIsConnected(bool);

    //methods
    bool checkCollision(player, player);
};
=======
#pragma once

struct ClientInput {
    bool up;
    bool down;
    bool right;
    bool left;
    bool shift;
    //bool e_key;
    };

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


private:
    //Player Stats
    float x;
    float y;
    int speed;
    float stamina;
    bool isCaught;
    bool isSeeker;
    bool isConnected;
    float abilityCooldown;

    //Client Input
    ClientInput input;

};
>>>>>>> b75cba87813cb24104d286244f27e2026e1c7c22
