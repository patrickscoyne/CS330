///////////////////////////////////////////////////////////////////////////////
// TableSurface.cpp
// =====
// manage the rendering of the table surface object in the 3D scene
///////////////////////////////////////////////////////////////////////////////

#include "TableSurface.h"
#include "SceneManager.h"
#include <glm/glm.hpp>

namespace
{
	// Table surface constants (scale, rotation, position, color)
	const glm::vec3 TABLE_SCALE(20.0f, 1.0f, 10.0f);
	const float TABLE_X_ROTATION = 0.0f;
	const float TABLE_Y_ROTATION = 0.0f;
	const float TABLE_Z_ROTATION = 0.0f;
	const glm::vec3 TABLE_POSITION(0.0f, 0.0f, 0.0f);

	const float TABLE_RED = 1.0f;
	const float TABLE_GREEN = 1.0f;
	const float TABLE_BLUE = 1.0f;
	const float TABLE_ALPHA = 1.0f;
}

// Render function
void TableSurface::Render(SceneManager* sceneManager)
{
	if (sceneManager == nullptr)
	{
		return;
	}

	sceneManager->SetTransformations(
		TABLE_SCALE,
		TABLE_X_ROTATION,
		TABLE_Y_ROTATION,
		TABLE_Z_ROTATION,
		TABLE_POSITION);

	// Set texture
	sceneManager->SetShaderTexture("wood");

	// Set Material
	sceneManager->SetShaderMaterial("table");
	
	// Set UV scale for texture
	sceneManager->SetTextureUVScale(2.7f, 1.0f);

	sceneManager->GetBasicMeshes()->DrawPlaneMesh();
}