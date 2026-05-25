# Sneak & Peak 

<div align="center">

![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![Raylib](https://img.shields.io/badge/raylib-000000?style=for-the-badge&logo=raylib&logoColor=white)
![Eigen](https://img.shields.io/badge/Eigen3-33B5E5?style=for-the-badge&logo=c&logoColor=white)
![TCP/IP](https://img.shields.io/badge/TCP%2FSockets-FF4136?style=for-the-badge&logo=transmission&logoColor=white)


placeholder for some gif 


> A fast-paced, 2D multiplayer stealth & hide-and-seek game built entirely from scratch.  

</div>

---

## Features 

* **Dynamic Raymarching Line of Sight:** Custom lighting engine with shadowcasting. You only see what's in front of you.

* **Custom TCP Multiplayer:** A fully functional, low-latency client-server architecture handling up to 4 players concurrently.

* **Stamina System:** Tactical resource management for sprinting and outsmarting the Seeker.

* **Pixel-Art Esthetics:** Smooth tilemap rendering with animated character sprites.




## Controls 

| Key | Action |
| :---: | :--- |
| `W` `A` `S` `D` | Move character | 
| `L-SHIFT` | Sprint (consumes stamina) | 
| `ENTER` | Select in menu | 
| `ESC` | Exit game / Disconnect | 



## Requirements | Dependecies

 To compile and run the game, you will need:

* **C++17** (GCC / Clang)
* **Raylib** (Graphics framework)
* **Eigen3** (Matrix & math library)
* **Make** (Build system)



## Build & Run 

 The project uses a unified `Makefile`. You can build both the server and the client simultaneously.


### 1. Compile
```bash
# Builds both server and client | Buduje serwer i klienta
make 

# Or build separately | Albo buduj osobno
make server
make client
```
### 2. Run the Server 
Start the server first. It will listen on port 5000 (defined in constants.h).

```bash
./bin/server
```

### 3. Run the Client
Open a new terminal and start the game client.
```
./bin/client
```

## Architecture

Sneak & Peak is divided into three main components:

1. `/server`: Headless authoritative game server. Handles collision detection (gameLogic), state distribution, and player connections.

2. `/client`: Raylib-based frontend. Responsible for rendering (including the GLSL shader + Raycasting combo), animations, and capturing input.

3. `/shared`: Common data structures, enums, constants, and network packet layouts to keep the client and server perfectly synchronized.
