
-----------------------------------------------
--- Called from C++ when we are initialized ---
-----------------------------------------------
function start()

    Print( "Welcome to the family lua" );

end

-----------------------------------------------
--- Called from C++ every update ---
-----------------------------------------------
function update()

end

function onClick()

    CurrentScene = GetActiveScene();
    CurrentScene:AddEntity( "This boi is from heck lua event" );

end
