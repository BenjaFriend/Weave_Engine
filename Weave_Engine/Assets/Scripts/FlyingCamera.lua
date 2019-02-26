
FlyingCamera = {}

-----------------------------------------------
--- Called from C++ when we are initialized ---
-----------------------------------------------
function start()
	Print("Start")
end

function update()
	if IsKeyDown(84) then
		MoveCamera(VEC3:new(0, .1, 0))
	end
end