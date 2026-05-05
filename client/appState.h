#pragma once

/*  
Enum class defining the different states of the client application,
used to manage the flow of the application and determine which screen should be displayed. 
*/

enum class AppState{
    MAIN_MENU, 
    CONNECTING,
    LOBBY,
    IN_GAME,
    DISCONNECTED,
    GAME_OVER,
    EXIT
};