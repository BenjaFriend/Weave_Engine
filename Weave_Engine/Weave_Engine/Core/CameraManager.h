#pragma once 

#include "../Entity/Camera.h"

/// <summary>
/// the camera manager is in charge of making sure that there is always a 
/// camera available for use by either the editor or gameplay, and making sure
/// that the last one does not get deleted. 
/// </summary>
/// <author>Ben Hoffman</author>
class CameraManager
{

public:

    static CameraManager* GetInstance();

    static void ReleaseInstance();

    Camera* AddCamera();

    //const Camera* GetActiveCamera() const;

private:

    CameraManager();

    ~CameraManager();

    static CameraManager* Instance;



};