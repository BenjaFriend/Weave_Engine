#pragma once

/////////////////////////////////////////////////
// Forward Declarations

class SimpleVertexShader;
class SimplePixelShader;

/// <summary>
///  Material class to define how the surface of a mesh looks when drawn
/// </summary>
/// <author>Ben Hoffman</author>
class Material
{
public:

    /// <summary>
    /// Constructor for this material
    /// </summary>
    /// <param name="aVertShader">Pointer to the vertex shader for this material</param>
    /// <param name="aPixShader">Pointer to the pixel shader for this material</param>
	Material(SimpleVertexShader* aVertShader, SimplePixelShader* aPixShader);

	/** Destructor of the material */
	~Material();


	////////////////////////////////////////////////////
	// Accessors
	////////////////////////////////////////////////////

	/** Returns the vertex shader of this material */
	SimpleVertexShader* GetVertexShader() const;
	
	/** Returns the pixel shader of this material */
	SimplePixelShader* GetPixelShader() const;

private: 

	/** Vertex shader of this material */
	SimpleVertexShader* VertexShader = nullptr;

	/** Pixel shader of this material */
	SimplePixelShader* PixelShader = nullptr;

};

