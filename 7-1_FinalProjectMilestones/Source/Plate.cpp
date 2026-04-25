///////////////////////////////////////////////////////////////////////////////
// Plate.cpp
// =====
// manage the rendering of the plate object in the 3D scene
///////////////////////////////////////////////////////////////////////////////
#include "Plate.h"
#include "SceneManager.h"
#include <glm/glm.hpp>

namespace
{
    // Plate rim (rotated tapered cylinder)
    const glm::vec3 PLATE_RIM_SCALE(4.5f, 0.5f, 4.5f);
    const float PLATE_RIM_X_ROTATION = 180.0f;  
    const float PLATE_RIM_Y_ROTATION = 0.0f;
    const float PLATE_RIM_Z_ROTATION = 0.0f;
    const glm::vec3 PLATE_RIM_POSITION(-2.5f, 0.6f, 2.0f);

    // Plate base (flattened cylinder)
    const glm::vec3 PLATE_BASE_SCALE(3.5f, 0.0005f, 3.5f);
    const float PLATE_BASE_X_ROTATION = 0.0f;
    const float PLATE_BASE_Y_ROTATION = 0.0f;
    const float PLATE_BASE_Z_ROTATION = 0.0f;
    const glm::vec3 PLATE_BASE_POSITION(-2.5f, 0.5f, 2.0f);
    
}

// Render function
void Plate::Render(SceneManager* sceneManager)
{
    if (sceneManager == nullptr)
    {
        return;
    }

    // Set plate rim scale, rotation, and position
    sceneManager->SetTransformations(
        PLATE_RIM_SCALE,
        PLATE_RIM_X_ROTATION,
        PLATE_RIM_Y_ROTATION,
        PLATE_RIM_Z_ROTATION,
        PLATE_RIM_POSITION);

    // Draw plate rim
    // Set texture
    sceneManager->SetShaderTexture("plate_texture");
    sceneManager->SetShaderMaterial("plate");
    sceneManager->GetBasicMeshes()->DrawTaperedCylinderMesh(true, false, true);

    // Set plate base scale, rotation, and position
    sceneManager->SetTransformations(
        PLATE_BASE_SCALE,
        PLATE_BASE_X_ROTATION,
        PLATE_BASE_Y_ROTATION,
        PLATE_BASE_Z_ROTATION,
        PLATE_BASE_POSITION);

    // Draw plate base
    sceneManager->SetShaderTexture("plate_texture");
    sceneManager->SetShaderMaterial("plate");
    sceneManager->GetBasicMeshes()->DrawCylinderMesh();


}