
FlyingCamera = {}

-----------------------------------------------
--- Called from C++ when we are initialized ---
-----------------------------------------------
function start()

  -- Create a table of data to be used for entity creation
  Entity1Data = {
	name = "Flying Camera Lua",
	mesh = Nil,
	material = Nil,
	pos = VEC3.new(10, 0, 0)
  }

  -- Load in an entity with this type of data
  FlyingCamera = CreateEntity( Entity1Data )

end
