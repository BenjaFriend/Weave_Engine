why = {};

posY = 0;

-----------------------------------------------
--- Called from C++ when we are initialized ---
-----------------------------------------------
function start()

    why = Entity.new( "Lau Entity", "Assets/Models/sphere.obj" )
    why:SetPos(0, 2, 0)

    TEST = Entity.new()

end

-----------------------------------
--- Called from C++ every frame ---
-----------------------------------
function update(dt)

    posY = posY + dt * 3
    why:SetPos(0, posY, 0)

end
