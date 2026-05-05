#define PLAYER_WIDTH 25
#define PLAYER_LENGTH 30


//Input shared by clients to server (Client action keys)
//if key pressed - true, else - false
typedef struct {
    bool up;
    bool down;
    bool right;
    bool left;
    bool shift;
    //bool e_key;
} ClientInput;

//Current player in-game state
typedef struct
{
    float x;
    float y;
    int speed;
    float stamina;
    bool isCaught;
    bool isSeeker;
    float abilityCooldown;
} PlayerState;
