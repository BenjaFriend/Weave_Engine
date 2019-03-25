#include "stdafx.h"

#include "Camera.h"
#include "Entity/Transform.h"
#include "../Entity/Entity.h"
#include "CameraManager.h"

size_t Camera::CameraCount = 0;

COMPONENT_INIT( Camera )

#define POS_SAVE_KEY            "Pos"
#define FORWARD_SAVE_KEY        "Forward"
#define RIGHT_SAVE_KEY          "Right"
#define UP_SAVE_KEY             "Up"
#define YAW_ANGLE_SAVE_KEY      "Yaw"
#define PITCH_ANGLE_SAVE_KEY    "Pitch"

Camera::Camera()
{
    Pos = glm::vec3( 0.f, 0.f, -5.f );
    RelativeInput = glm::vec3( 0.f, 0.f, 0.f );
    Up = DEFAULT_UP;
    Forward = DEFAULT_FORWARD;
    Right = DEFAULT_RIGHT;

    PitchAngle = 0;
    YawAngle = 90;

    inputManager = Input::InputManager::GetInstance();

    CameraID = CameraCount++;
    CameraManager::GetInstance()->RegisterCamera( CameraID, this );
}

Camera::Camera( nlohmann::json const & aInitData )
    : Camera()
{
    Pos = {};
    Pos.x = aInitData [ POS_SAVE_KEY ] [ "X" ];
    Pos.y = aInitData [ POS_SAVE_KEY ] [ "Y" ];
    Pos.z = aInitData [ POS_SAVE_KEY ] [ "Z" ];

    RelativeInput = glm::vec3( 0.f, 0.f, 0.f );
}

Camera::~Camera()
{
    CameraManager::GetInstance()->UnregisterCamera( CameraID );
    --CameraCount;

    inputManager = nullptr;
}

void Camera::SetDoRotation( bool aDoRot )
{
    DoRotation = aDoRot;
}

const bool Camera::GetDoRotation() const
{
    return DoRotation;
}

const float Camera::GetMovementSpeed() const
{
    return MovementSpeed;
}

void Camera::SetMovementSpeed( float aNewVal )
{
    MovementSpeed = aNewVal;
}

void Camera::SaveComponentData( nlohmann::json & aCompData )
{
    aCompData [ POS_SAVE_KEY ] [ "X" ] = Pos.x;
    aCompData [ POS_SAVE_KEY ] [ "Y" ] = Pos.y;
    aCompData [ POS_SAVE_KEY ] [ "Z" ] = Pos.z;
}

void Camera::DrawEditorGUI()
{
    ImGui::InputFloat( "FOV", &FOV );
    ImGui::InputFloat( "NearZ", &NearZ );
    ImGui::InputFloat( "FarZ", &FarZ );

    ImGui::Text( "Camera ID: %ld", CameraID );
    if ( ImGui::Button( "Set as active Camera" ) )
    {
        SetAsActiveCamera();
    }

    if ( ImGui::Button( "Use Debug Camera" ) )
    {
        CameraManager::GetInstance()->SetDebugCameraActive();
    }
}

void Camera::UpdateProjectionMatrix( const float aWidth, const float aHeight )
{
    Pos = this->GetPosition();
    Entity* ent = OwningEntity->GetAsEntity();

    if ( ent != nullptr )
    {
        Transform* transform = ent->GetTransform();
        Pos = transform->GetPosition();
        Forward = transform->GetForward();
        Right = transform->GetRight();
        Up = transform->GetUp();
    }

    //calculate view
    View = glm::transpose( glm::lookAtLH( Pos, Pos + Forward, Up ) );

    //calculate proj
    Projection = glm::transpose( glm::perspectiveFovLH( FOV, aWidth, aHeight, NearZ, FarZ ) );
}

void Camera::UpdateMouseInput( const long aDeltaMouseX, const long aDeltaMouseY )
{
    if ( !DoRotation ) return;

    PitchAngle += static_cast< float >( aDeltaMouseY ) * SENSITIVITY * ( SouthPaw ? -1.f : 1.f );
    glm::clamp( PitchAngle, -MAX_PITCH, MAX_PITCH );
    YawAngle += static_cast< float >( aDeltaMouseX ) * SENSITIVITY * ( SouthPaw ? 1.f : -1.f );

    Entity* ent = OwningEntity->GetAsEntity();

    if ( ent != nullptr )
    {
        Transform* transform = ent->GetTransform();
        transform->SetRotation( { 0, YawAngle, PitchAngle } );
        Forward = transform->GetForward();
        Right = transform->GetRight();
        Up = transform->GetUp();
    }
    else
    {
        //rotate along x and y
        glm::mat4 rotation = glm::eulerAngleYX( YawAngle, PitchAngle );
        Forward = rotation * DEFAULT_FORWARD;
        Up = rotation * DEFAULT_UP;
        Right = glm::cross( Forward, Up );
    }
}

////////////////////////////////////////////////////
// Accessors
////////////////////////////////////////////////////

const glm::vec3 Camera::GetPosition() const
{
    if ( OwningEntity == nullptr ) return glm::vec3( 0.f, 0.f, 0.f );
    Entity* ent = OwningEntity->GetAsEntity();
    if ( ent != nullptr )
    {
        Transform* transform = ent->GetTransform();
        return transform->GetPosition();
    }
    else
    {
        return glm::vec3( 0.f );
    }
}

void Camera::SetPosition( glm::vec3 position )
{
    Pos = position;
}

const glm::mat4 Camera::GetViewMatrix() const
{
    return View;
}

const glm::mat4 Camera::GetProjectMatrix() const
{
    return Projection;
}

void Camera::SetAsActiveCamera()
{
    CameraManager::GetInstance()->SetActiveCamera( CameraID );
}