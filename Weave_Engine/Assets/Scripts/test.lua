ent = nil;


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

    if( ent == nil ) then
        CurrentScene = GetActiveScene();
        Print( CurrentScene:GetSceneName() );
        ent = CurrentScene:AddEntity( "Lua generated" );
    end



end
