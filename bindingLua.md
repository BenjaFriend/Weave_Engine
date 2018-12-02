# Binding Lua

These are the steps that I took to bind Lua to this C++ engine

I did this based off of [this](https://eliasdaler.wordpress.com/2014/07/18/using-lua-with-cpp-luabridge/)
tutorial.

Add [LuaBridge](https://github.com/vinniefalco/LuaBridge) as a submodule
in the `additional` directory folder.

```
git submodule add git@github.com:vinniefalco/LuaBridge.git
```

[Download Lua](https://sourceforge.net/projects/luabinaries/files/5.2/Windows%20Libraries/Dynamic/)
