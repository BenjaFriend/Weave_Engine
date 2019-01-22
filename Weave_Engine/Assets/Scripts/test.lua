
posY = 0;
LoadedEnt = {}

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

  -- Create a table of data to be used for entity creation
  Entity1Data = {
	name = "This REALLLLL",
	mesh = "Assets/Models/sphere.obj",
	material = LoadMaterial( cobbleStoneInfo ),
	pos = VEC3.new(2, 5, 0)
  }

  -- Load in an entity with this type of data
  LoadedEnt = CreateEntity( Entity1Data )
	
end