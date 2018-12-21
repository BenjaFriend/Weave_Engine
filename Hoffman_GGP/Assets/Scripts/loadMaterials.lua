-----------------------------------------------
--- Called from C++ when we are initialized ---
-----------------------------------------------
function start()

  -- Create a new material
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
  --
  bronze2 = Entity.new(
    "Lmoa new",
    "Assets/Models/sphere.obj",
    bronzeMat
  );
  
  bronze2:SetPos(-5, 1, 0)

end