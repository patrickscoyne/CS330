///////////////////////////////////////////////////////////////////////////////
// Grapefruit.cpp
// =====
// manage the rendering of the grapefruit object in the 3D scene
///////////////////////////////////////////////////////////////////////////////
#include "Grapefruit.h"
#include "SceneManager.h"
#include <glm/glm.hpp>

namespace
{
    // Set grapefruit scale, rotation, and position
    const glm::vec3 GRAPEFRUIT_SCALE(1.2f, 1.2f, 1.2f);
    const float GRAPEFRUIT_X_ROTATION = 0.0f;
    const float GRAPEFRUIT_Y_ROTATION = 0.0f;
    const float GRAPEFRUIT_Z_ROTATION = 0.0f;
    const glm::vec3 GRAPEFRUIT_POSITION(-2.8f, 1.65f, 2.0f);   
}

// Render function
void Grapefruit::Render(SceneManager* sceneManager)
{
    if (sceneManager == nullptr)
    {
        return;
    }

    sceneManager->SetTransformations(
        GRAPEFRUIT_SCALE,
        GRAPEFRUIT_X_ROTATION,
        GRAPEFRUIT_Y_ROTATION,
        GRAPEFRUIT_Z_ROTATION,
        GRAPEFRUIT_POSITION);

    // Set texture, material, and UV scale
    sceneManager->SetShaderTexture("grapefruit");
    sceneManager->SetShaderMaterial("skin");
    sceneManager->SetTextureUVScale(5.0f, 3.0f);
    
    // Draw grapefruit
    sceneManager->GetBasicMeshes()->DrawSphereMesh();
}