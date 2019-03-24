
FlyingCamera = {}
cameraSpeed = 5.5;
rotSpeed = 5.5;
prevMousePos = VEC2:new( 0, 0 );

-----------------------------------------------
--- Called from C++ when we are initialized ---
-----------------------------------------------
function start()
	Print("Start");
end

function update( deltaTime )

	movement = VEC3:new(0, 0, 0)

	if IsCKeyDown('W') then
		movement.z = movement.z + 1
	end
	if IsCKeyDown('S') then
		movement.z = movement.z - 1
	end

	if IsCKeyDown('D') then
		movement.x = movement.x - 1;
	end
	if IsCKeyDown('A') then
		movement.x = movement.x + 1;
	end

	if IsCKeyDown(' ') then
		movement.y = movement.y + 1;
	end
	if IsCKeyDown('X') then
		movement.y = movement.y - 1;
	end


	rot = VEC2:new( 0, 0 );

	curMousePos = GetMousePosition();	-- returns a vector 2 of mouse pos

	-- If right click is happening
	if IsIKeyDown( 2 ) then
		rot.x = ( curMousePos.x - prevMousePos.x ) * deltaTime * rotSpeed;
		rot.y = -1 * ( curMousePos.y - prevMousePos.y ) * deltaTime * rotSpeed;
	end
	movement.x = movement.x * deltaTime * cameraSpeed;
	movement.y = movement.y * deltaTime * cameraSpeed;
	movement.z = movement.z * deltaTime * cameraSpeed;

	MoveCamera( movement, rot );
	prevMousePos = curMousePos;

end
