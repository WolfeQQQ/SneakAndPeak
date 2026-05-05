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
    float setX(float);
    float setY(float);
    float setStamina(float);
    float setAbilityCooldown(float);
    int setSpeed(int);
    bool setIsCaught(bool);
    bool setIsSeeker(bool);
    bool setIsConnected(bool);

    //methods
    bool checkCollision(player, player);
};
