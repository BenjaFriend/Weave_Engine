
posY = 0;

cobbleStoneInfo = {
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

  cobbleStoneMat = LoadMaterial( cobbleStoneInfo )

  cobble1 = Entity.new(
    "Cobblestone Material Sphere",
    "Assets/Models/sphere.obj",
    cobbleStoneMat
  );

  cobble1:SetPos(2, 1, 0)

end

-----------------------------------
--- Called from C++ every frame ---
-----------------------------------
function update( dt )

  --posY = posY + totalTime * math.sin(0.8)


  --woodEntity:SetPos(0, posY, 0)

end
