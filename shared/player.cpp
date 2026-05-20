#include "player.h"


//Constructor, Destructor
player::player() {
    this->x = 640.0f;      // Ustaw graczy na środku mapy
    this->y = 360.0f;
    this->speed = 5;       // Nadaj im jakąś początkową prędkość!
    this->stamina = 100.0f;
    this->isConnected = false;
    this->isCaught = false;
    this->isSeeker = false;
    this->isRunning = false;
    this->abilityCooldown = 0.0f;
    direction = player::Direction::DOWN;
    playerstate = player::PlayerState::IDLE;
    

    this->input.up = false;
    this->input.down = false;
    this->input.left = false;
    this->input.right = false;
    this->input.shift = false;
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
float player::getSpeed() const {
    return this->speed;
}
int player::getId() const {
    return this->id;
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
bool player::getIsRunning() const {
    return this->isRunning;
}
player::ClientInput player::getClientInput() const {
    return this->input;
}
player::Direction player::getDirection() const {
    return direction;
}
player::PlayerState player::getPlayerState() const {
    return playerstate;
}



//Setters
void player::setX(float newX) {
    this->x = newX;
}
void player::setY(float newY) {
    this->y = newY;
}
void player::setStamina(float newStamina) {
    this->stamina = newStamina;
}
void player::setAbilityCooldown(float newAbilityCooldown) {
    this->abilityCooldown = newAbilityCooldown;
}
void player::setSpeed(float newSpeed) {
    this->speed = newSpeed;
}
void player::setId(int newId){
    this->id = newId;
}
void player::setIsCaught(bool newCaught) {
    this->isCaught = newCaught;
}
void player::setIsSeeker(bool newSeeker) {
    this->isSeeker = newSeeker;
}
void player::setIsConnected(bool newConnected) {
    this->isConnected = newConnected;
}
void player::setIsRunning(bool newRunning) {
    this->isRunning = newRunning;
}
void player::setInput(ClientInput& newInput) {
    this->input = newInput;
}
void player::setDirection(player::Direction newDirection) {
    direction = newDirection;
}
void player::setPlayerState(player::PlayerState newPlayerState) {
    playerstate = newPlayerState;
}
