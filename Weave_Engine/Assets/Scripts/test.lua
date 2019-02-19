
posY = 0;
LoadedEnt = {}

-----------------------------------------------
--- Called from C++ when we are initialized ---
-----------------------------------------------
function start()

  -- Create a table of data to be used for entity creation
  Entity1Data = {
	name = "This REALLLLL",
	mesh = "Assets/Models/sphere.obj",
	material = LoadMaterial( "Assets/Materials/Cobblestone.wmat" ),
	pos = VEC3.new(2, 5, 0)
  }

  -- Load in an entity with this type of data
  LoadedEnt = CreateEntity( Entity1Data )

end
