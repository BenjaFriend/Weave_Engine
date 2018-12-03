# Binding Lua

These are the steps that I took to bind Lua to this C++ engine

I did this based off of [this](https://eliasdaler.wordpress.com/2014/07/18/using-lua-with-cpp-luabridge/)
tutorial and [this one](https://eliasdaler.wordpress.com/2015/08/10/using-lua-and-cpp-in-practice/).

Add [LuaBridge](https://github.com/vinniefalco/LuaBridge) as a submodule
in the `additional` directory folder.

```
git submodule add git@github.com:vinniefalco/LuaBridge.git
```

[Download Lua](https://sourceforge.net/projects/luabinaries/files/5.2/Windows%20Libraries/Dynamic/)

Place that in your project somewhere and add the include folder
Add the `lib` folder in the Lua binaries to the project,
and add `lua52.lib` to the additional libraries in the linker.

Add a build step to copy the `lua52.dll` to your output directory as a post
build process:
```
xcopy /E /d "$(SolutionDir)additional\lua\lib\lua52.dll" "$(OutDir)"
```
