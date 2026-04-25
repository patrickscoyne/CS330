///////////////////////////////////////////////////////////////////////////////
// shadermanager.cpp
// ============
// manage the loading and rendering of 3D scenes
//
//  AUTHOR: Brian Battersby - SNHU Instructor / Computer Science
//	Created for CS-330-Computational Graphics and Visualization, Nov. 1st, 2023
///////////////////////////////////////////////////////////////////////////////

#include "SceneManager.h"
#include "TableSurface.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif

#include <glm/gtx/transform.hpp>
#include <windows.h>

// declaration of global variables
namespace
{
	const char* g_ModelName = "model";
	const char* g_ColorValueName = "objectColor";
	const char* g_TextureValueName = "objectTexture";
	const char* g_UseTextureName = "bUseTexture";
	const char* g_UseLightingName = "bUseLighting";
}

/***********************************************************
 *  SceneManager()
 *
 *  The constructor for the class
 ***********************************************************/
SceneManager::SceneManager(ShaderManager *pShaderManager)
{
	m_pShaderManager = pShaderManager;
	m_basicMeshes = new ShapeMeshes();
}

/***********************************************************
 *  ~SceneManager()
 *
 *  The destructor for the class
 ***********************************************************/
SceneManager::~SceneManager()
{
	m_pShaderManager = NULL;
	delete m_basicMeshes;
	m_basicMeshes = NULL;
}

/***********************************************************
 *  CreateGLTexture()
 *
 *  This method is used for loading textures from image files,
 *  configuring the texture mapping parameters in OpenGL,
 *  generating the mipmaps, and loading the read texture into
 *  the next available texture slot in memory.
 ***********************************************************/
bool SceneManager::CreateGLTexture(const char* filename, std::string tag)
{
	int width = 0;
	int height = 0;
	int colorChannels = 0;
	GLuint textureID = 0;

	// indicate to always flip images vertically when loaded
	stbi_set_flip_vertically_on_load(true);

	// try to parse the image data from the specified image file
	unsigned char* image = stbi_load(
		filename,
		&width,
		&height,
		&colorChannels,
		0);

	// if the image was successfully read from the image file
	if (image)
	{
		std::cout << "Successfully loaded image:" << filename << ", width:" << width << ", height:" << height << ", channels:" << colorChannels << std::endl;

		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// if the loaded image is in RGB format
		if (colorChannels == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		// if the loaded image is in RGBA format - it supports transparency
		else if (colorChannels == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		else
		{
			std::cout << "Not implemented to handle image with " << colorChannels << " channels" << std::endl;
			return false;
		}

		// generate the texture mipmaps for mapping textures to lower resolutions
		glGenerateMipmap(GL_TEXTURE_2D);

		// free the image data from local memory
		stbi_image_free(image);
		glBindTexture(GL_TEXTURE_2D, 0); // Unbind the texture

		// register the loaded texture and associate it with the special tag string
		m_textureIDs[m_loadedTextures].ID = textureID;
		m_textureIDs[m_loadedTextures].tag = tag;
		m_loadedTextures++;

		return true;
	}

	std::cout << "Could not load image:" << filename << std::endl;

	// Error loading the image
	return false;
}

/***********************************************************
 *  BindGLTextures()
 *
 *  This method is used for binding the loaded textures to
 *  OpenGL texture memory slots.  There are up to 16 slots.
 ***********************************************************/
void SceneManager::BindGLTextures()
{
	for (int i = 0; i < m_loadedTextures; i++)
	{
		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, m_textureIDs[i].ID);
	}
}

/***********************************************************
 *  DestroyGLTextures()
 *
 *  This method is used for freeing the memory in all the
 *  used texture memory slots.
 ***********************************************************/
void SceneManager::DestroyGLTextures()
{
	for (int i = 0; i < m_loadedTextures; i++)
	{
		glGenTextures(1, &m_textureIDs[i].ID);
	}
}

/***********************************************************
 *  FindTextureID()
 *
 *  This method is used for getting an ID for the previously
 *  loaded texture bitmap associated with the passed in tag.
 ***********************************************************/
int SceneManager::FindTextureID(std::string tag)
{
	int textureID = -1;
	int index = 0;
	bool bFound = false;

	while ((index < m_loadedTextures) && (bFound == false))
	{
		if (m_textureIDs[index].tag.compare(tag) == 0)
		{
			textureID = m_textureIDs[index].ID;
			bFound = true;
		}
		else
			index++;
	}

	return(textureID);
}

/***********************************************************
 *  FindTextureSlot()
 *
 *  This method is used for getting a slot index for the previously
 *  loaded texture bitmap associated with the passed in tag.
 ***********************************************************/
int SceneManager::FindTextureSlot(std::string tag)
{
	int textureSlot = -1;
	int index = 0;
	bool bFound = false;

	while ((index < m_loadedTextures) && (bFound == false))
	{
		if (m_textureIDs[index].tag.compare(tag) == 0)
		{
			textureSlot = index;
			bFound = true;
		}
		else
			index++;
	}

	return(textureSlot);
}

/***********************************************************
 *  FindMaterial()
 *
 *  This method is used for getting a material from the previously
 *  defined materials list that is associated with the passed in tag.
 ***********************************************************/
bool SceneManager::FindMaterial(std::string tag, OBJECT_MATERIAL& material)
{
	if (m_objectMaterials.size() == 0)
	{
		return(false);
	}

	int index = 0;
	bool bFound = false;
	while ((index < m_objectMaterials.size()) && (bFound == false))
	{
		if (m_objectMaterials[index].tag.compare(tag) == 0)
		{
			bFound = true;
			material.ambientColor = m_objectMaterials[index].ambientColor;
			material.ambientStrength = m_objectMaterials[index].ambientStrength;
			material.diffuseColor = m_objectMaterials[index].diffuseColor;
			material.specularColor = m_objectMaterials[index].specularColor;
			material.shininess = m_objectMaterials[index].shininess;
		}
		else
		{
			index++;
		}
	}

	return(true);
}

/***********************************************************
 *  SetTransformations()
 *
 *  This method is used for setting the transform buffer
 *  using the passed in transformation values.
 ***********************************************************/
void SceneManager::SetTransformations(
	glm::vec3 scaleXYZ,
	float XrotationDegrees,
	float YrotationDegrees,
	float ZrotationDegrees,
	glm::vec3 positionXYZ)
{
	// variables for this method
	glm::mat4 modelView;
	glm::mat4 scale;
	glm::mat4 rotationX;
	glm::mat4 rotationY;
	glm::mat4 rotationZ;
	glm::mat4 translation;

	// set the scale value in the transform buffer
	scale = glm::scale(scaleXYZ);
	// set the rotation values in the transform buffer
	rotationX = glm::rotate(glm::radians(XrotationDegrees), glm::vec3(1.0f, 0.0f, 0.0f));
	rotationY = glm::rotate(glm::radians(YrotationDegrees), glm::vec3(0.0f, 1.0f, 0.0f));
	rotationZ = glm::rotate(glm::radians(ZrotationDegrees), glm::vec3(0.0f, 0.0f, 1.0f));
	// set the translation value in the transform buffer
	translation = glm::translate(positionXYZ);

	modelView = translation * rotationX * rotationY * rotationZ * scale;

	if (NULL != m_pShaderManager)
	{
		m_pShaderManager->setMat4Value(g_ModelName, modelView);
	}
}

/***********************************************************
 *  SetShaderColor()
 *
 *  This method is used for setting the passed in color
 *  into the shader for the next draw command
 ***********************************************************/
void SceneManager::SetShaderColor(
	float redColorValue,
	float greenColorValue,
	float blueColorValue,
	float alphaValue)
{
	// variables for this method
	glm::vec4 currentColor;

	currentColor.r = redColorValue;
	currentColor.g = greenColorValue;
	currentColor.b = blueColorValue;
	currentColor.a = alphaValue;

	if (NULL != m_pShaderManager)
	{
		m_pShaderManager->setIntValue(g_UseTextureName, false);
		m_pShaderManager->setVec4Value(g_ColorValueName, currentColor);
	}
}

/***********************************************************
 *  SetShaderTexture()
 *
 *  This method is used for setting the texture data
 *  associated with the passed in ID into the shader.
 ***********************************************************/
void SceneManager::SetShaderTexture(
	std::string textureTag)
{
	if (NULL != m_pShaderManager)
	{
		m_pShaderManager->setIntValue(g_UseTextureName, true);

		int textureID = -1;
		textureID = FindTextureSlot(textureTag);
		m_pShaderManager->setSampler2DValue(g_TextureValueName, textureID);
	}
}

/***********************************************************
 *  SetTextureUVScale()
 *
 *  This method is used for setting the texture UV scale
 *  values into the shader.
 ***********************************************************/
void SceneManager::SetTextureUVScale(float u, float v)
{
	if (NULL != m_pShaderManager)
	{
		m_pShaderManager->setVec2Value("UVscale", glm::vec2(u, v));
	}
}



/***********************************************************
 *  SetShaderMaterial()
 *
 *  This method is used for passing the material values
 *  into the shader.
 ***********************************************************/
void SceneManager::SetShaderMaterial(
	std::string materialTag)
{
	if (m_objectMaterials.size() > 0)
	{
		OBJECT_MATERIAL material;
		bool bReturn = false;

		bReturn = FindMaterial(materialTag, material);
		if (bReturn == true)
		{
			m_pShaderManager->setVec3Value("material.ambientColor", material.ambientColor);
			m_pShaderManager->setFloatValue("material.ambientStrength", material.ambientStrength);
			m_pShaderManager->setVec3Value("material.diffuseColor", material.diffuseColor);
			m_pShaderManager->setVec3Value("material.specularColor", material.specularColor);
			m_pShaderManager->setFloatValue("material.shininess", material.shininess);
		}
	}
}

/**************************************************************/
/*** STUDENTS CAN MODIFY the code in the methods BELOW for  ***/
/*** preparing and rendering their own 3D replicated scenes.***/
/*** Please refer to the code in the OpenGL sample project  ***/
/*** for assistance.                                        ***/
/**************************************************************/



/***********************************************************
 *  PrepareScene()
 *
 *  This method is used for preparing the 3D scene by loading
 *  the shapes, textures in memory to support the 3D scene 
 *  rendering
 ***********************************************************/
void SceneManager::PrepareScene()
{

	// define the materials for objects in the scene
	DefineObjectMaterials();
	// add and define the light sources for the scene
	SetupSceneLights();

	// Constants for texture names
	const char* const TAG_DIAMOND_PORCELAIN = "diamond_porcelain";
	const char* const TAG_WOOD = "wood";
	const char* const TAG_MUG_SMOOTH = "mug_smooth";
	const char* const TAG_GRAPEFRUIT = "grapefruit";
	const char* const TAG_PLATE = "plate_texture";
	const char* const TAG_COASTER = "coaster_texture";

	// bool for successful texture load
	bool texturesLoaded = true;

	// only one instance of a particular mesh needs to be
	// loaded in memory no matter how many times it is drawn
	// in the rendered 3D scene

	m_basicMeshes->LoadPlaneMesh();
	m_basicMeshes->LoadTaperedCylinderMesh();
	m_basicMeshes->LoadCylinderMesh();
	m_basicMeshes->LoadTorusMesh();
	m_basicMeshes->LoadSphereMesh();

	// Load textures and make sure each texture is valid
	// Texture for cup body (diamond pattern)
	if (!CreateGLTexture("textures/mug_texture.jpg", TAG_DIAMOND_PORCELAIN))
		texturesLoaded = false;

	// Texture for table 
	if (!CreateGLTexture("textures/table_texture.jpg", TAG_WOOD))
		texturesLoaded = false;

	// Texture for cup (smooth surfaces)
	if (!CreateGLTexture("textures/mug_smooth.jpg", TAG_MUG_SMOOTH))
		texturesLoaded = false;

	// Texture for grapefruit
	if (!CreateGLTexture("textures/grapefruit_texture.jpg", TAG_GRAPEFRUIT))
		texturesLoaded = false;
		
	// Texture for plate
	if (!CreateGLTexture("textures/plate_texture.jpg", TAG_PLATE))
		texturesLoaded = false;

	// Texture for coaster
	if (!CreateGLTexture("textures/coaster_texture.jpg", TAG_COASTER))
		texturesLoaded = false;

	// If some textures failed to load, ask user if they want to load scene still
	if (!texturesLoaded)
	{
		// Open a message box to notify user and give options to open/close
		int result = MessageBoxA(NULL,
			"Not all of the textures loaded successfully.\n"
			"Some objects may appear white or missing.\n"
			"Do you want to open the scene anyway?",
			"Texture Loading Error",
			MB_YESNO | MB_ICONWARNING);

		// If user clicks "no", exit the program
		if (result == IDNO)
		{
			std::cout << "Exiting\n";
			exit(0);
		}
	}

	BindGLTextures();
}

/***********************************************************
 *  RenderScene()
 *
 *  This method is used for rendering the 3D scene by 
 *  transforming and drawing the basic 3D shapes
 ***********************************************************/
void SceneManager::RenderScene()
{
	// Render the table surface
	m_tableSurface.Render(this);

	// Render cup
	m_cup.Render(this);

	// Render plate
	m_plate.Render(this);

	// Render the grapefruit
	m_grapefruit.Render(this);

	// Render the coaster
	m_coaster.Render(this);
}

/***********************************************************
 *  DefineObjectMaterials()
 *
 *  This method is used for configuring the various material
 *  settings for all of the objects within the 3D scene.
 ***********************************************************/
void SceneManager::DefineObjectMaterials()
{
	OBJECT_MATERIAL cupMaterial;

	cupMaterial.ambientColor = glm::vec3(0.1f, 0.1f, 0.1f);
	cupMaterial.ambientStrength = 0.5f;
	cupMaterial.diffuseColor = glm::vec3(0.5f, 0.5f, 0.5f);
	cupMaterial.specularColor = glm::vec3(0.1f, 0.1f, 0.1f);
	cupMaterial.shininess = 15.0f;
	cupMaterial.tag = "cup";
	m_objectMaterials.push_back(cupMaterial);

	OBJECT_MATERIAL woodMaterial;

	woodMaterial.ambientColor = glm::vec3(0.8f, 0.8f, 0.8f);
	woodMaterial.ambientStrength = 0.3f;
	woodMaterial.diffuseColor = glm::vec3(1.0f, 1.0f, 1.0f);
	woodMaterial.specularColor = glm::vec3(0.3f, 0.3f, 0.3f);
	woodMaterial.shininess = 1.0f;
	woodMaterial.tag = "table";
	m_objectMaterials.push_back(woodMaterial);

	OBJECT_MATERIAL fruitMaterial;

	fruitMaterial.ambientColor = glm::vec3(0.8f, 0.8f, 0.8f);
	fruitMaterial.ambientStrength = 0.3f;
	fruitMaterial.diffuseColor = glm::vec3(0.1f, 0.1f, 0.1f);
	fruitMaterial.specularColor = glm::vec3(0.1f, 0.1f, 0.1f);
	fruitMaterial.shininess = 10.0f;
	fruitMaterial.tag = "skin";
	m_objectMaterials.push_back(fruitMaterial);

	OBJECT_MATERIAL plateMaterial;

	plateMaterial.ambientColor = glm::vec3(0.95f, 0.95f, 0.95f);
	plateMaterial.ambientStrength = 0.10f;
	plateMaterial.diffuseColor = glm::vec3(0.55f, 0.55f, 0.55f);
	plateMaterial.specularColor = glm::vec3(0.4f, 0.4f, 0.4f);
	plateMaterial.shininess = 0.005f;
	plateMaterial.tag = "plate";
	m_objectMaterials.push_back(plateMaterial);

	OBJECT_MATERIAL coasterMaterial;

	coasterMaterial.ambientColor = glm::vec3(1.0f, 1.0f, 1.0f);
	coasterMaterial.ambientStrength = 0.01f;
	coasterMaterial.diffuseColor = glm::vec3(1.0f, 1.0f, 1.0f);
	coasterMaterial.specularColor = glm::vec3(0.0f, 0.0f, 0.0f);
	coasterMaterial.shininess = 0.01f;
	coasterMaterial.tag = "coaster";
	m_objectMaterials.push_back(coasterMaterial);
}

/***********************************************************
 *  SetupSceneLights()
 *
 *  This method is called to add and configure the light
 *  sources for the 3D scene.  There are up to 4 light sources.
 ***********************************************************/
void SceneManager::SetupSceneLights()
{
	m_pShaderManager->setBoolValue("bUseLighting", true);

	// Light properties structure
	struct LightProperties
	{
		glm::vec3 position;
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		float focalStrength;
		float specularIntensity;
	};

	// Set each lightsource's properties in an array
	const LightProperties lights[] = {
		// lightsource[0] (warm golden main light)
		{ glm::vec3(0.0f, 20.0f, 0.0f),
		  glm::vec3(0.22f, 0.17f, 0.11f),
		  glm::vec3(0.95f, 0.82f, 0.58f),
		  glm::vec3(1.0f, 0.88f, 0.70f), 
		  32.0f, 
		  1.0f 
		},

		// lightsource[1] (neutral rear light)
		{ glm::vec3(0.0f, 10.0f, -200.0f),
		  glm::vec3(0.01f, 0.01f, 0.01f),
		  glm::vec3(0.01f, 0.01f, 0.01f),
		  glm::vec3(0.3f, 0.3f, 0.3f), 
		  32.0f, 
		  10.0f 
		}
	};

	// Calculate how many light sources are in the array
	const size_t numLights = std::size(lights);

	// Loop and setup each light source in the array
	for (int i = 0; i < numLights; ++i)
	{
		std::string base = "lightSources[" + std::to_string(i) + "].";

		m_pShaderManager->setVec3Value(base + "position", lights[i].position);
		m_pShaderManager->setVec3Value(base + "ambientColor", lights[i].ambient);
		m_pShaderManager->setVec3Value(base + "diffuseColor", lights[i].diffuse);
		m_pShaderManager->setVec3Value(base + "specularColor", lights[i].specular);
		m_pShaderManager->setFloatValue(base + "focalStrength", lights[i].focalStrength);
		m_pShaderManager->setFloatValue(base + "specularIntensity", lights[i].specularIntensity);
	}
}
