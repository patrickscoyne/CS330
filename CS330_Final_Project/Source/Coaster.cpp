///////////////////////////////////////////////////////////////////////////////
// Coaster.cpp
// =====
// manage the rendering of the coaster object in the 3D scene
///////////////////////////////////////////////////////////////////////////////
#include "Coaster.h"
#include "SceneManager.h"
#include <glm/glm.hpp>

namespace
{
    // Set scale, rotation, and position for the coaster
    const glm::vec3 COASTER_SCALE(1.85f, 0.12f, 1.85f);   
    const float COASTER_X_ROTATION = 0.0f;
    const float COASTER_Y_ROTATION = 0.0f;
    const float COASTER_Z_ROTATION = 0.0f;
    const glm::vec3 COASTER_POSITION(4.0f, 0.01f, -3.0f); 

    // Color constants
    // Side color (top of coaster has texture)
    const float COASTER_RED = 0.322f;
    const float COASTER_GREEN = 0.278f;
    const float COASTER_BLUE = 0.271f;
    const float COASTER_ALPHA = 1.0f;
}

void Coaster::Render(SceneManager* sceneManager)
{
    if (sceneManager == nullptr)
    {
        return;
    }

    sceneManager->SetTransformations(
        COASTER_SCALE,
        COASTER_X_ROTATION,
        COASTER_Y_ROTATION,
        COASTER_Z_ROTATION,
        COASTER_POSITION);

    // Set texture and material for coaster
    sceneManager->SetShaderTexture("coaster_texture");
    sceneManager->SetShaderMaterial("coaster");

    // Set UV scale for texture
    sceneManager->SetTextureUVScale(1.0f, 1.0f);

    // Draw coaster top
    sceneManager->GetBasicMeshes()->DrawCylinderMesh(true, false, false);  

    // Set coaster side color
    sceneManager->SetShaderColor(
        COASTER_RED,
        COASTER_GREEN,
        COASTER_BLUE,
        COASTER_ALPHA);

    // Draw coaster sides
    sceneManager->GetBasicMeshes()->DrawCylinderMesh(false, false, true); 
}