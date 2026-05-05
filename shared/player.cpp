<<<<<<< HEAD
#include "player.h"


//Constructor, Destructor
player::player()
{
    this->isConnected = false;
    this->up = false;
    this->down = false;
    this->left = false;
    this->right = false;
    this->shift = false;
    //this->e_key = false;
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
void player::setSpeed(int newSpeed) {
    this->speed = newSpeed;
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
=======
#include "player.h"


//Constructor, Destructor
player::player()
{
    this->isConnected = false;
    this->input.up = false;
    this->input.down = false;
    this->input.left = false;
    this->input.right = false;
    this->input.shift = false;
    //this->input.eKey = false;
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
player::ClientInput player::getClientInput() const {
    return this->input;
}


//Setters
void player::setX(float newX) {
    this->x += newX;
}
void player::setY(float newY) {
    this->y += newY;
}
void player::setStamina(float newStamina) {
    this->stamina = newStamina;
}
void player::setAbilityCooldown(float newAbilityCooldown) {
    this->abilityCooldown = newAbilityCooldown;
}
void player::setSpeed(int newSpeed) {
    this->speed = newSpeed;
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
>>>>>>> b75cba87813cb24104d286244f27e2026e1c7c22
