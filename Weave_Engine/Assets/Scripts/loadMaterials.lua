curY = 1

--- Material creation data
cobbleStoneInfo = {
	name = "Cobblestone Mat",
    VS = "VertexShader.cso",
	PS = "PixelShader.cso",
	albedo = "Assets/Textures/cobblestone_albedo.png",
	norm = "Assets/Textures/cobblestone_normals.png",
	roughness = "Assets/Textures/cobblestone_roughness.png",
	metal = "Assets/Textures/cobblestone_metal.png"
}

-----------------------------------------------
--- Called from C++ when we are initialized ---
-----------------------------------------------
function start()

    count = 0;
    i = 0

    while i < 10 do
        j = 0
        while j < 10 do
            EntityData = {
                name = "-----Lua Generated " .. count,
                mesh = "Assets/Models/sphere.obj",
                material = LoadMaterial( cobbleStoneInfo ),
                pos = VEC3:new( i, j, 0 )
            }

            CreateEntity( EntityData )

            j = j + 1
            count = count + 1
        end
        i = i + 1
    end

end
