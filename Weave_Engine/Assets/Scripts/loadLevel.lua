
posY = 0;
LoadedEnt = {}

-----------------------------------------------
--- Called from C++ when we are initialized ---
-----------------------------------------------
function start()

  -- Create a table of data to be used for entity creation
  Entity1Data = {
	name = "Floor Boi",
	mesh = "Assets/Models/cube.obj",
	material = LoadMaterial( "Assets/Materials/Cobblestone.wmat" ),
	pos = VEC3.new( 0, -1, 0 )
  }

  -- Load in an entity with this type of data
  LoadedEnt = CreateEntity( Entity1Data )

  LoadedEnt:GetTransform():SetScaleX( 25 )
  LoadedEnt:GetTransform():SetScaleY( 0.1 )
  LoadedEnt:GetTransform():SetScaleZ( 25 )

end
