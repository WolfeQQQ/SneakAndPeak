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
