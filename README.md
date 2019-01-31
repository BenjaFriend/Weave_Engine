# Weave Engine

This engine aims to accomplish the following:

* Server authoritative network architecture with a headless server
* LLVM/Clang compatibility
* Basic gameplay demo that showcases the networking features of the engine
* As cross platform as possible (input, graphics, filesystem, etc.)

## Getting Started

What do you need?

* Visual Studio 2017 with the C++ compiler options  
* [Clang](http://releases.llvm.org/download.html) 7.0.1 or newer __*__
* [Clang for Visual Studio](https://marketplace.visualstudio.com/items?itemName=LLVMExtensions.llvm-toolchain) __*__
* [Boost](https://www.boost.org/doc/libs/1_69_0/more/getting_started/windows.html)

__*__ _is an optional item that you can configure yourself_

Init the submodules

```
git submodule update --init --recursive;
```

In order to use Boost, make sure you have defined the following environment variables:

* `BOOST_ROOT`, the boost root path (Ex: `C:\Program Files\boost\boost_1_67_0`)
* `BOOST_LIBRARYDIR`, the library path for boost (Ex: `C:\Program Files\boost\boost_1_67_0\stage\lib`)

## Systems currently in place

* Entity Component System
* Editor Tools (ImGui)
* Lua binding for gameplay scripting

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
