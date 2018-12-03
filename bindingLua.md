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

Now that we have the build all set up, you should be able to compile and run.

To test to see if Lua bridge is actually working, make the following lua script:

```lua
testString = "hello lua!";
number = 42;
```

And somewhere in your C++, throw in this test code:

```C++
extern "C"
{
  #include "lua/lua.h"
  #include "lua/lauxlib.h"
  #include "lua/lualib.h"
}
#include "LuaBridge/LuaBridge.h"

// ...

int main() {
  using namespace luabridge;

  lua_State* L = luaL_newstate();
  luaL_openlibs( L );

  const char* luaScript = "Assets/Scripts/test.lua";

  if ( luaL_loadfile( L, filename.c_str() ) || lua_pcall( L, 0, 0, 0 ) )
  {
      LOG_ERROR( "Failed to load lua script: {}", filename );
  }
  else
  {
      LOG_TRACE( "Loaded lua script: {}", filename );

      LuaRef s = getGlobal( L, "testString" );
      LuaRef n = getGlobal( L, "number" );
      std::string luaString = s.cast<std::string>();
      int answer = n.cast<int>();

      LOG_TRACE( luaString );
      LOG_TRACE( "And here's our number: {}", answer );
  }
}
```
