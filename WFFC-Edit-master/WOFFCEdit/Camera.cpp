#include "Camera.h"
using namespace DirectX;
using namespace SimpleMath;

Camera::Camera()
{
	// Initialise variables for the camera
	cameraSpeed = 0.5f;
	rotationRate = 0.7f;

	mousePosXCurrent = 0;
	mousePosXLast = 0;
	mousePosYCurrent = 0;
	mousePosYLast = 0;

	lookDirection = DirectX::SimpleMath::Vector3(0, 0, 0);
	orientation = DirectX::SimpleMath::Vector3(0, 0, 0);
	position = DirectX::SimpleMath::Vector3(0, 10, -20);
	lookAt = DirectX::SimpleMath::Vector3(0, 0, 0);
	right = DirectX::SimpleMath::Vector3(0, 0, 0);
}

Camera::~Camera()
{

}

void Camera::Update(DX::StepTimer const& timer)
{
	// Update mouse positions 
	mousePosYLast = mousePosYCurrent;
	mousePosYCurrent = m_InputCommands.mouse_Y;

	mousePosXLast = mousePosXCurrent; 
	mousePosXCurrent = m_InputCommands.mouse_X; 

	MoveCamera(); // Moves the camera

	if (m_InputCommands.focusCamera == true)
	{
		// Focuses the camera on selected object and offsets it so it can be viewed
		position = newLookAt + (DirectX::XMFLOAT3(0, 5, -10));
		view = Matrix::CreateLookAt(position, newLookAt, Vector3::Up);
	}
}

void Camera::Tick(InputCommands* Input)
{
	m_InputCommands = *Input; // Update input
}

DirectX::SimpleMath::Matrix Camera::GetViewMatrix()
{
	return view;
}

void Camera::MoveCamera()
{
	if (m_InputCommands.mouse_RB_Down) // If the right mouse button is down
	{
		int xDelta = mousePosXLast - mousePosXCurrent; // Find the delta of the mouse position this frame
		orientation.y += xDelta * rotationRate; // Flip orientation so it matches

		int yDelta = mousePosYLast - mousePosYCurrent; // Find the delta of the mouse position this frame
		orientation.x += yDelta * rotationRate; // Flip orientation so it matches
	}

	CreateLookDirection();

	CameraMovementInput();

	// update lookat point
	lookAt = position + lookDirection;

	// apply camera vectors
	view = DirectX::SimpleMath::Matrix::CreateLookAt(position, lookAt, DirectX::SimpleMath::Vector3::UnitY);
}

void Camera::CameraMovementInput()
{
	// This function will move the camera based on the user's input

	if (m_InputCommands.up) // E key pressed
	{
		position += up * cameraSpeed;
	}

	if (m_InputCommands.down) // Q key pressed
	{
		position -= up * cameraSpeed;
	}

	if (m_InputCommands.forward) // W key pressed
	{
		position += lookDirection * cameraSpeed;
	}

	if (m_InputCommands.back) // S key pressed
	{
		position -= lookDirection * cameraSpeed;
	}

	if (m_InputCommands.left) // A key pressed
	{
		position -= right * cameraSpeed;
	}

	if (m_InputCommands.right) // D key pressed
	{
		position += right * cameraSpeed;
	}
}

void Camera::CreateLookDirection()
{
	// Create look direction from orientation
	lookDirection.x = sin((orientation.y) * 3.1415 / 180) * cos((orientation.x) * 3.1415 / 180);
	lookDirection.y = sin((orientation.x) * 3.1415 / 180);
	lookDirection.z = cos((orientation.y) * 3.1415 / 180) * cos((orientation.x) * 3.1415 / 180);
	lookDirection.Normalize(); // Normalise values


	// Create right and up vectors from look Direction
	lookDirection.Cross(DirectX::SimpleMath::Vector3::UnitY, right);
	lookDirection.Cross(right, up);
}


DirectX::SimpleMath::Vector3 Camera::GetCamPosition()
{
	return position;
}


void Camera::SetLookAt(DirectX::SimpleMath::Vector3 LookAt)
{
	newLookAt = LookAt;
}

DirectX::SimpleMath::Vector3 Camera::GetRightVector()
{
	return right;
}

DirectX::SimpleMath::Vector3 Camera::GetForwardVector()
{
	DirectX::SimpleMath::Vector3 ForwardVector;

	// Get the X and Z of the look direction to get forward vector
	ForwardVector.x = lookDirection.x;
	ForwardVector.z = lookDirection.z;
	ForwardVector.y = 0;

	ForwardVector.Normalize();
	return ForwardVector;
}

float Camera::GetCameraSpeed()
{
	return cameraSpeed;
}

float Camera::GetCamRotationRate()
{
	return rotationRate;
}

void Camera::SetCameraSpeed(float newCameraSpeed)
{
	cameraSpeed = newCameraSpeed;
}

void Camera::SetCamRotationRate(float newRotationRate)
{
	rotationRate = newRotationRate;
}
