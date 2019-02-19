curY = 1


-----------------------------------------------
--- Called from C++ when we are initialized ---
-----------------------------------------------
function start()

    count = 0;
    i = 0

    while i < 2 do
        j = 0
        while j < 2 do
            EntityData = {
                name = "-----Lua Generated " .. count,
                mesh = "Assets/Models/sphere.obj",
                material = LoadMaterial( "Assets/Materials/Bronze.wmat" ),
                pos = VEC3:new( i, j, 0 )
            }

            CreateEntity( EntityData )

            j = j + 1
            count = count + 1
        end
        i = i + 1
    end

end
