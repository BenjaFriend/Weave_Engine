
woodEntity = {};

posY = 0;

-----------------------------------------------
--- Called from C++ when we are initialized ---
-----------------------------------------------
function start()

  -- Create a new material

  stoneMat = Material.new(
    device,                                       --ID3D11Device* aDevice,
    context,                                      --ID3D11DeviceContext* aContext
    "VertexShader.cso",                           --FileName vertexShader
    "PixelShader.cso",                            --FileName pixelShader
    "Assets/Textures/cobblestone_albedo.png",     --FileName albedoTexture
    "Assets/Textures/cobblestone_normals.png",    --FileName normalTexture
    "Assets/Textures/cobblestone_roughness.png",  --FileName RoughnessTexture
    "Assets/Textures/cobblestone_metal.png"       --FileName MetalTexture
  );

  bronzeMat = Material.new(
    device,
    context,
    "VertexShader.cso",
    "PixelShader.cso",
    "Assets/Textures/bronze_albedo.png",
    "Assets/Textures/bronze_normals.png",
    "Assets/Textures/bronze_roughness.png",
    "Assets/Textures/bronze_metal.png"
  );

  stoneEntiy = Entity.new(
    "Stone Entity",             -- Name
    "Assets/Models/sphere.obj", -- Mesh file
    stoneMat                    -- Material
  );

  bronzeEntity = Entity.new(
    "Bronze Entity",
    "Assets/Models/sphere.obj",
    bronzeMat
  );

  bronze2 = Entity.new(
    "Bronze Entity (2)",
    "Assets/Models/sphere.obj",
    bronzeMat
  );

  stoneEntiy:SetPos(0, 4, 0)
  bronzeEntity:SetPos(-2, 1 , 0)
  bronze2:SetPos(2, 1, 0)

end

-----------------------------------
--- Called from C++ every frame ---
-----------------------------------
function update( dt )

  --posY = posY + totalTime * math.sin(0.8)


  --woodEntity:SetPos(0, posY, 0)

end
