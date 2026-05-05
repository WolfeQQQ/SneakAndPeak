#pragma once

class player
{
private:
    float x;
    float y;
    int speed;
    float stamina;
    bool isCaught;
    bool isSeeker;
    bool isConnected;
    float abilityCooldown;
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
    float setX(float newX);
    float setY(float newY);
    float setStamina(float newStamina);
    float setAbilityCooldown(float newAbilityCooldown);
    int setSpeed(int newSpeed);
    bool setIsCaught(bool newCaught);
    bool setIsSeeker(bool newSeeker);
    bool setIsConnected(bool newConnected);
};
