
# Starconquest
Starconquest is a realtime 4X strategy game. The game is inspired by titles like Master of Orion and Stellaris. The game is written in C++ and is using SDL2 for graphics and sounds.

# Libraries
The game needs the following Libraries to build:

* SDL2
* SDL2_image
* SDL2_gfx (optional)
* SDL2_mixer
* SDL2_ttf
* zlib
* pthread (Linux)

## Unittests
To run the unitests the library google_test will be required.

## Compiler
The game requires a compiler that supports C++17. To build the game the buildsystem CMake is needed.

# License
All source code is released under the GNU General Public License Version 3.

# Packages
The game is right now in a pre alpha stage. We will provide packages for Windows and Linux once the game reaches a state where testers will be able to test the game.

# Building the Game
## Linux/BSD
```bash
mkdir build/ && cd build/ && cmake ../
make
```
## Windows
It is adviced to generate a project for Visual Studio 2018 or newer with the CMake-GUI. All nessecary dependencies are included.

## macOS
As of now there is no support for this operating system.
