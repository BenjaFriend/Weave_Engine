#pragma once 

#include <unordered_map>

/////////////////////////////////////////////////
// Forward Declarations
class Camera;
class Entity;

/// <summary>
/// the camera manager is in charge of making sure that there is always a 
/// camera available for use by either the editor or gameplay, and making sure
/// that the last one does not get deleted. 
/// </summary>
/// <author>Ben Hoffman</author>
class CameraManager
{

public:

    /// <summary>
    /// Get a reference to this camera manager static instance
    /// </summary>
    /// <returns>Pointer to the camera manager</returns>
    static CameraManager* GetInstance();

    /// <summary>
    /// Release this static instance of the camera manager
    /// </summary>
    static void ReleaseInstance();

    /// <summary>
    /// Add a camera to the scene that can be used for 
    /// </summary>
    /// <param name="aName">The name of this camera</param>
    /// <returns>Pointer to the added camera</returns>
    Camera* AddCamera( const std::string aName );
    
    /// <summary>
    /// Get the current camera being used to render the scene
    /// </summary>
    /// <returns>Pointer to the active camera</returns>
    FORCE_INLINE Camera* GetActiveCamera() const { return ActiveCamera; }

    FORCE_INLINE Camera* GetDebugCamera() const { return DebugCamera; }

private:

    CameraManager();

    ~CameraManager();

    static CameraManager* Instance;

    void CreateDebugCamera();

    Camera* DebugCamera = nullptr;
    Entity* DebugCameraEntity = nullptr;
    
    Camera* ActiveCamera = nullptr;
    Entity* CameraEntity = nullptr;

    std::unordered_map < std::string, Camera* > CurrentCameras;
};