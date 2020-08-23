[![discord](https://img.shields.io/discord/746366936937660527.svg?logo=discord "Discord")](https://discord.gg/ApTJcVN)
[![license](https://img.shields.io/github/license/mashape/apistatus.svg?style=flat-square "License")](https://github.com/swr06/Minecraft/blob/master/LICENSE)
![hunter](https://img.shields.io/badge/hunter-ATMC-blue.svg)

![Minecraft Logo](https://github.com/swr06/Minecraft/blob/master/Screenshots/logo.png)
<p align=center>
Created by Samuel Rasquinha using C++17 and OpenGL 3.3 :game_die:
</p>

# A Tiny Minecraft Clone
A Tiny Minecraft clone made with C++ and OpenGL. 

# Current Features
- 3D Perspective FPS style camera with smooth movement
- Terrain generation using simplex noise
- World structures such as trees and cactus
- Biome generation by noise quantization 
- Infinite Terrain
- High Definition texture pack
- Support for translucent blocks such as Glass (sort of)
- Fast chunk meshing 
- Face culling
- Frustum culling
- Complete day/night cycle
- Exponential fog
- 3D AABB Collision detection
- Efficient Flood Fill Voxel lighting algorithm using BFS queue
- Fast world saving and loading
- Complete main menu and GUI interface using ImGui
- Logger
- Cloud Skybox using cubemaps
- Accurate block editing using a voxel transversal algorithm
- Cave generation using 3D perlin noise
- Water generation 
- No OS Specific commands/dependencies. 

# Features to come (probably in the rewrite)
- Multiplayer
- Sun 
- Weather
- Multithreading

# Compiling 
This project compiles with Clang and MSVC. 

# Benchmarks? 
- Intel HD 4000 - 200~ FPS
- Vega 8 iGPU - 500~ FPS

# Contact me
- Email : samuelrasquinha@gmail.com
- Discord : swr#1899

# Credits
The block art, skybox art and fonts are not made by me. All credits go to their owners. 

# Special Thanks
1) Dan Bechard (Discord : @dandymcgee#2568)
2) Krazer (Discord : @Krazer7877)

# References 
1) https://www.seedofandromeda.com/blogs/29-fast-flood-fill-lighting-in-a-blocky-voxel-game-pt-1
2) https://www.seedofandromeda.com/blogs/30-fast-flood-fill-lighting-in-a-blocky-voxel-game-pt-2

# License
This project is under the MIT license. See LICENSE for more info

# Known issues
- Transparency issues with glass and models (such as flowers..)

# Screenshots

![Minecraft Menu](https://github.com/swr06/Minecraft/blob/master/Screenshots/menu.png)
<p align="center">
  <i>The Menu screen</i>
</p>

![Minecraft in-game](https://github.com/swr06/Minecraft/blob/master/Screenshots/in_game.png)
<p align="center">
  <i>An in-game screenshot</i>
</p>

![Minecraft in-game](https://github.com/swr06/Minecraft/blob/master/Screenshots/bfs_light.jpg)
<p align="center">
  <i>Lighting Demo with lamps</i>
</p>

# Supporting 
If you like this project, please consider starring it on github. Thankyou :)
