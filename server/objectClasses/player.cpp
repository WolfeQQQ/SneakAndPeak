#include "player.h"


//Constructor, Destructor
player::player()
{
    this->isConnected = false;
}

player::~player()
{
}

//Getters
float player::getX() const {
    return this->x;
}
float player::getY() const {
    return this->y;
}
float player::getAbilityCooldown() const {
    return this->abilityCooldown;
}
float player::getStamina() const {
    return this->stamina;
}
int player::getSpeed() const {
    return this->speed;
}
bool player::getIsCaught() const {
    return this->isCaught;
}
bool player::getIsSeeker() const {
    return this->isSeeker;
}
bool player::getIsConnected() const {
    return this->isConnected;
}


//Setters
float player::setX(float newX) {
    this->x = newX;
}
float player::setY(float newY) {
    this->y = newY;
}
float player::setStamina(float newStamina) {
    this->stamina = newStamina;
}
float player::setAbilityCooldown(float newAbilityCooldown) {
    this->abilityCooldown = newAbilityCooldown;
}
int player::setSpeed(int newSpeed) {
    this->speed = newSpeed;
}
bool player::setIsCaught(bool newCaught) {
    this->isCaught = newCaught;
}
bool player::setIsSeeker(bool newSeeker) {
    this->isSeeker = newSeeker;
}
bool player::setIsConnected(bool newConnected) {
    this->isConnected = newConnected;
}
