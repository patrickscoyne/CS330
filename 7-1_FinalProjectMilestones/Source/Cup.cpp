///////////////////////////////////////////////////////////////////////////////
// Cup.cpp
// =====
// manage the rendering of the cup object in the 3D scene
///////////////////////////////////////////////////////////////////////////////

#include "Cup.h"
#include "SceneManager.h"
#include <glm/glm.hpp>

namespace
{
	// Cup body (outside) constants (scale, rotation, position)
	const glm::vec3 CUP_BODY_SCALE(1.5f, 2.5f, 1.5f);
	const float CUP_BODY_X_ROTATION = 180.0f;
	const float CUP_BODY_Y_ROTATION = 0.0f;
	const float CUP_BODY_Z_ROTATION = 0.0f;
	const glm::vec3 CUP_BODY_POSITION(4.0f, 2.65f, -3.0f);

	// Cup body (inside) constants (scale)
	const glm::vec3 CUP_BODY_INSIDE_SCALE(1.45f, 2.5f, 1.45f);
	
	// Cup rim constants (scale, rotation, position)
	const glm::vec3 CUP_RIM_SCALE(1.47f, 0.075f, 1.47f);
	const float CUP_RIM_X_ROTATION = 0.0f;
	const float CUP_RIM_Y_ROTATION = 0.0f;
	const float CUP_RIM_Z_ROTATION = 0.0f;
	const glm::vec3 CUP_RIM_POSITION(4.0f, 2.65f, -3.0f);

	// Cup handle constants (scale, rotation, position)
	// Half Torus
	const glm::vec3 CUP_HANDLE_SCALE(0.5f, 0.6f, 1.0f);
	const float CUP_HANDLE_X_ROTATION = 0.0f;
	const float CUP_HANDLE_Y_ROTATION = 0.0f;
	const float CUP_HANDLE_Z_ROTATION = -70.0f;
	const glm::vec3 CUP_HANDLE_POSITION(5.5f, 1.62f, -3.0f);

	// Cylinder
	const glm::vec3 CUP_HANDLE1_SCALE(0.1f, 0.8f, 0.2f);
	const float CUP_HANDLE1_X_ROTATION = 0.0f;
	const float CUP_HANDLE1_Y_ROTATION = 0.0f;
	const float CUP_HANDLE1_Z_ROTATION = -70.0f;
	const glm::vec3 CUP_HANDLE1_POSITION(4.95f, 0.87f, -3.0f);

	// Color constants
	// Rim color
	const float CUP_RIM_RED = 0.388f;
	const float CUP_RIM_GREEN = 0.271f;
	const float CUP_RIM_BLUE = 0.212f;
	const float CUP_RIM_ALPHA = 1.0f;
}

// Render function
void Cup::Render(SceneManager* sceneManager)
{
	if (sceneManager == nullptr)
	{
		return;
	}

	// Draw cup handle (half torus and cylinder)
	// Half Torus
	sceneManager->SetTransformations(
		CUP_HANDLE_SCALE,
		CUP_HANDLE_X_ROTATION,
		CUP_HANDLE_Y_ROTATION,
		CUP_HANDLE_Z_ROTATION,
		CUP_HANDLE_POSITION);

	// Set texture
	sceneManager->SetShaderTexture("mug_smooth");
	sceneManager->SetShaderMaterial("cup");

	sceneManager->GetBasicMeshes()->DrawHalfTorusMesh();

	// Cylinder
	sceneManager->SetTransformations(
		CUP_HANDLE1_SCALE,
		CUP_HANDLE1_X_ROTATION,
		CUP_HANDLE1_Y_ROTATION,
		CUP_HANDLE1_Z_ROTATION,
		CUP_HANDLE1_POSITION);

	// Set texture
	sceneManager->SetShaderTexture("mug_smooth");
	sceneManager->SetShaderMaterial("cup");

	sceneManager->GetBasicMeshes()->DrawCylinderMesh();

	// Draw cup body (outside)
	sceneManager->SetTransformations(
		CUP_BODY_SCALE,
		CUP_BODY_X_ROTATION,
		CUP_BODY_Y_ROTATION,
		CUP_BODY_Z_ROTATION,
		CUP_BODY_POSITION);

	// Set texture
	sceneManager->SetShaderTexture("diamond_porcelain");
	sceneManager->SetShaderMaterial("cup");
	
	// Set UV scale for texture
	sceneManager->SetTextureUVScale(3.0f, 0.6f);

	// Bools set to draw sides only
	sceneManager->GetBasicMeshes()->DrawTaperedCylinderMesh(false, false, true);

	// Draw cup body (inside)
	sceneManager->SetTransformations(
		CUP_BODY_INSIDE_SCALE,
		CUP_BODY_X_ROTATION,
		CUP_BODY_Y_ROTATION,
		CUP_BODY_Z_ROTATION,
		CUP_BODY_POSITION);

	// Set Texture
	sceneManager->SetShaderTexture("mug_smooth");
	sceneManager->SetShaderMaterial("cup");
	
	// Bools set to draw cup sides and cup bottom
	sceneManager->GetBasicMeshes()->DrawTaperedCylinderMesh(true, false, true);

	// Draw cup rim 
	sceneManager->SetTransformations(
		CUP_RIM_SCALE,
		CUP_RIM_X_ROTATION,
		CUP_RIM_Y_ROTATION,
		CUP_RIM_Z_ROTATION,
		CUP_RIM_POSITION);

	// Set rim color
	sceneManager->SetShaderColor(
		CUP_RIM_RED,
		CUP_RIM_GREEN,
		CUP_RIM_BLUE,
		CUP_RIM_ALPHA);

	// Draw cup rim
	sceneManager->GetBasicMeshes()->DrawCylinderMesh(false, false, true);
}