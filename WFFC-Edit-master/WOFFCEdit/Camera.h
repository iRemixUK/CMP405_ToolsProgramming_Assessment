#pragma once
#include "InputCommands.h"
#include "DisplayChunk.h"

namespace DX
{
	class StepTimer;
}


class Camera
{
public:

	Camera();
	~Camera();

	// Basic game loop
	void Tick(InputCommands* Input);
	void Update(DX::StepTimer const& timer);

	// Getters for view matrix, position, right and forward vectors
	DirectX::SimpleMath::Matrix GetViewMatrix();
	DirectX::SimpleMath::Vector3 GetCamPosition();
	void SetLookAt(DirectX::SimpleMath::Vector3 LookAt);
	DirectX::SimpleMath::Vector3 GetRightVector();
	DirectX::SimpleMath::Vector3 GetForwardVector();

	float GetCameraSpeed();
	float GetCamRotationRate();

	void SetCameraSpeed(float newCameraSpeed);
	void SetCamRotationRate(float newRotationRate);

private:

	void MoveCamera();
	void CameraMovementInput();
	void CreateLookDirection();

	InputCommands m_InputCommands;
	float cameraSpeed; // Camera speed
	float rotationRate; // Camera's rotation rate for when it is in free view

	// Camera variables
	DirectX::SimpleMath::Vector3 position; // position of the camera
	DirectX::SimpleMath::Vector3 orientation; // orientation of the camera
	DirectX::SimpleMath::Vector3 lookAt; // Point where camera is looking at
	DirectX::SimpleMath::Vector3 lookDirection; // The direction the camera is looking at
	DirectX::SimpleMath::Vector3 right; // Camera right vector (used for moving sideways)
	DirectX::SimpleMath::Vector3 up; // Camera up vector (for moving up relative to the camera's look direction


	DirectX::SimpleMath::Vector3 newLookAt;

	DirectX::SimpleMath::Matrix view; // Camera view matrix

	int mousePosXCurrent; // Current mouse x position
	int mousePosXLast; // Last frame's x position of mouse
	int mousePosYCurrent; // Current mouse y position
	int mousePosYLast; // Last frame's y position of mouse

};

