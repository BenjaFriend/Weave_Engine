# Weave Engine

This engine aims to accomplish the following: 

* Server authoritative network architecture 
* LLVM/Clang compatibilty
* Cross platform with Unix machines (stretch goal, as this involves a lot of graphics)
* Basic gameplay demo that showcases the networking features of the engine 

## Getting Started

* Visual Studio 2017 with the C++ compiler options  

Init the submodules
```
git submodule update --init --recursive;
```

## Systems currently in place

* Entity Component System
* Editor Tools (ImGui)
* Lua binding for gameplay scripting
* 

---
### Starter Code

I was provided some starter code in `Found. of Game Graphics Programming (IGME 540), Fall 2018` at RIT with Professor Chris Cascioli. We were given a "core" DirectX 11 framework to build upon throughout the semester. 

The starter code included initial versions of: 
* `DXCore` class
* `Game` class
* `Main.cpp`
* `SimpleShader` class
* `PixelShader.hlsl` and `VertexShader.hlsl`

> 
> The goal of this course is to explore the use of an advanced graphics API to access hadrd-ware accelerated graphics in a real-time graphics engine context. 
>

_Chris Cascioli, Course Syllabus_ 


The reason that I am using this core framework for this project is because the focus is not on graphics, and it provided a good starting point to go off of.  

---
