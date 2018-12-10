anEntity = Entity.new()
anEntity.data = 72

objects = {}

objects[0] = anEntity
objects[1] = anEntity

--- Iterate through entity array and create them
for objCount = 0, #objects do
  CreateEntity (objects[objCount])
end

-- Called from C++ when we are initalized
function start()

  another = Entity.new()
  another.data = 56
  CreateEntity (another)

end


--------objs = {            -- create an objects table
--------
--------    entityName1 = {
--------        data = 15,
--------        file = "test.txt",
--------        aTabe = Entity.new()
--------    },
--------
--------    entity34 = {
--------        data = 30,
--------        file = "lolHelp"
--------    }
--------}   -- end objs table
--------