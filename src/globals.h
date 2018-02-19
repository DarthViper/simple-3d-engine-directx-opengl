#ifndef GE3D_GLOBALS_H
#define GE3D_GLOBALS_H

#define _CRT_SECURE_NO_WARNINGS
#define GLEW_STATIC
#define _UNICODE
#define UNICODE

// C++
#include <map>
#include <fstream>

// DIRECTX
#ifdef _WINDOWS
#include <d3d11.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <d3dcompiler.h>
#include <dxgi1_4.h>
#include <DirectXMath.h>
#endif

// OpenGL
#include <GL/glew.h>
#ifdef _WINDOWS
	#include <wglext.h>
#else
	#include <glxext.h>
#endif
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/transform.hpp>

// ASSIMP
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// JSON
#include <json11-master/json11.hpp>

// LZMA SDK
#include <LzmaLib.h>

// LIBNOISE
#include <noise.h>

// WXWIDGETS
#include <wx/app.h>
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/dcmemory.h>
#include <wx/dcgraph.h>
#include <wx/event.h>
#include <wx/filedlg.h>
#include <wx/frame.h>
#include <wx/gbsizer.h>
#include <wx/glcanvas.h>
#include <wx/image.h>
#include <wx/listbox.h>
#include <wx/menu.h>
#include <wx/msgdlg.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>
#include <wx/sizer.h>
#include <wx/statline.h>
#include <wx/stattext.h>
#include <wx/stopwatch.h>
#include <wx/webview.h>

class BoundingVolume;
class Buffer;
class Camera;
class Component;
class DXContext;
class HUD;
class InputManager;
class Mesh;
class Model;
class Noise;
class PhysicsEngine;
class RayCast;
class RenderEngine;
class SceneManager;
class ShaderManager;
class ShaderProgram;
class Skybox;
class Terrain;
class Texture;
class TimeManager;
class Utils;
class Water;
class WaterFBO;
class Window;
class WindowFrame;

struct AssImpMesh
{
	aiMesh*        Mesh;
	const aiScene* Scene;
	aiMatrix4x4    Transformation;
};

enum Attrib
{
	VERTEX_NORMAL,
	VERTEX_POSITION,
	VERTEX_TEXCOORDS,
	NR_OF_ATTRIBS
};

enum BoundingVolumeType
{
	BOUNDING_VOLUME_NONE,
	BOUNDING_VOLUME_BOX,
	BOUNDING_VOLUME_SPHERE,
	NR_OF_BOUNDING_VOLUMES
};

enum ComponentType
{
	COMPONENT_UNKNOWN = -1,
	COMPONENT_CAMERA,
	COMPONENT_HUD,
	COMPONENT_MESH,
	COMPONENT_MODEL,
	COMPONENT_SKYBOX,
	COMPONENT_TERRAIN,
	COMPONENT_WATER
};

enum DrawModes
{
	DRAW_MODE_UNKNOWN = -1,
	DRAW_MODE_FILLED,
	DRAW_MODE_WIREFRAME,
	NR_OF_DRAW_MODES
};

struct GLCanvas
{
	bool         Active      = false;
	float        AspectRatio = 0.0f;
	wxGLCanvas*  Canvas      = nullptr;
	DXContext*   DX          = nullptr;
	wxGLContext* GL          = nullptr;
	wxPoint      Position    = wxPoint(0, 0);
	wxSize       Size        = wxSize(0, 0);
	WindowFrame* Window      = nullptr;
};

struct GPUDescription
{
	wxString Renderer = wxT("");
	wxString Vendor   = wxT("");
	wxString Version  = wxT("");
};

enum GraphicsAPI
{
	GRAPHICS_API_UNKNOWN = -1,
	GRAPHICS_API_DIRECTX11,
	GRAPHICS_API_DIRECTX12,
	GRAPHICS_API_OPENGL,
	GRAPHICS_API_VULKAN,
	NR_OF_GRAPHICS_ENGINES
};

enum IconType
{
	ID_ICON_UNKNOWN,
	ID_ICON_BROWSE,
	ID_ICON_PLANE,
	ID_ICON_CUBE,
	ID_ICON_UV_SPHERE,
	ID_ICON_ICO_SPHERE,
	ID_ICON_CYLINDER,
	ID_ICON_CONE,
	ID_ICON_TORUS,
	ID_ICON_MONKEY_HEAD,
	ID_ICON_SKYBOX,
	ID_ICON_TERRAIN,
	ID_ICON_WATER,
	ID_ICON_HUD,
	ID_ICON_QUAD,
	ID_CANVAS,
	ID_ASPECT_RATIO,
	ID_FOV,
	ID_DRAW_MODE,
	ID_DRAW_BOUNDING,
	ID_GRAPHICS_API,
	ID_VSYNC,
	ID_COMPONENTS,
	ID_CHILDREN,
	ID_SCENE_DETAILS,
	ID_SCENE_CLEAR,
	ID_SCENE_LOAD,
	ID_SCENE_SAVE,
	ID_REMOVE_COMPONENT,
	ID_REMOVE_CHILD,
};

struct Icon
{
	wxString File  = "";
	IconType ID    = ID_ICON_UNKNOWN;
	wxString Title = "";
};

struct Light
{
	glm::vec4 Color      = glm::vec4(0.4f, 0.4f, 0.4f, 1.0f);
	glm::vec3 Position   = glm::vec3(10.0f, 50.0f, 100.0f);
	glm::vec3 Direction  = glm::vec3(-0.1f, -0.5f, -1.0f);
	float     Reflection = 0.6f;
	float     Shine      = 20.0f;
};

struct MouseState
{
	wxPoint Position = wxPoint(0, 0);
};

struct Resource
{
	wxString File   = "";
	wxString Name   = "";
	wxString Result = "";
};

struct Time
{
	long Hours= 0, Minutes = 0, Seconds = 0, MilliSeconds = 0, Total = 0;

	Time(long ms)
	{
		this->Hours        = (ms / 3600000);
		this->Minutes      = ((ms % 3600000) / 60000);
		this->Seconds      = (((ms % 3600000) % 60000) / 1000);
		this->MilliSeconds = (((ms % 3600000) % 60000) % 1000);
		this->Total        = ms;
	}
};

enum Uniform
{
	AMBIENT_LIGHT_INTENSITY,
	CAMERA_POSITION,
	CAMERA_NEAR,
	CAMERA_FAR,
	CLIP_MAX,
	CLIP_MIN,
	ENABLE_CLIPPING,
	IS_TEXTURED,
	IS_TRANSPARENT,
	MATERIAL_COLOR,
	MATRIX_MODEL,
	MATRIX_VIEW,
	MATRIX_PROJECTION,
	MATRIX_MVP,
	MOVE_FACTOR,
	SOLID_COLOR,
	SUNLIGHT_COLOR,
	SUNLIGHT_DIRECTION,
	SUNLIGHT_POSITION,
	SUNLIGHT_REFLECTION,
	SUNLIGHT_SHINE,
	TEXTURES0, TEXTURES1, TEXTURES2, TEXTURES3, TEXTURES4, TEXTURES5,
	TEXTURE_SCALES0, TEXTURE_SCALES1, TEXTURE_SCALES2, TEXTURE_SCALES3, TEXTURE_SCALES4, TEXTURE_SCALES5,
	WAVE_STRENGTH,
	NR_OF_UNIFORMS
};

static const unsigned int BUFFER_SIZE      = 1024;
static const unsigned int LZMA_OFFSET_ID   = 8;
static const unsigned int LZMA_OFFSET_SIZE = 8;
static const unsigned int MAX_TEXTURES     = 6;

enum ShaderID
{
	SHADER_ID_UNKNOWN = -1,
	SHADER_ID_DEFAULT,
	SHADER_ID_HUD,
	SHADER_ID_SKYBOX,
	SHADER_ID_SOLID,
	SHADER_ID_TERRAIN,
	SHADER_ID_WATER,
	NR_OF_SHADERS
};

#ifdef _WINDOWS

static const unsigned int BYTE_ALIGN_BUFFER_DATA = 65536;
static const unsigned int BYTE_ALIGN_BUFFER_VIEW = 256;

struct DXCameraBuffer
{
	DirectX::XMFLOAT3 Position;
	float             Near;
	float             Far;
	DirectX::XMFLOAT3 Padding1;
};

struct DXMatrixBuffer
{
	DirectX::XMMATRIX Model;
	DirectX::XMMATRIX View;
	DirectX::XMMATRIX Projection;
	DirectX::XMMATRIX MVP;
};

struct DXLightBuffer
{
	DirectX::XMFLOAT4 Color;
	DirectX::XMFLOAT3 Direction;
	float             Reflection;
	DirectX::XMFLOAT3 Position;
	float             Shine;
};

struct DXDefaultBuffer
{
	DXMatrixBuffer    Matrices;
	DXLightBuffer     SunLight;
	DirectX::XMFLOAT3 Ambient;
	int               EnableClipping;
	DirectX::XMFLOAT3 ClipMax;
	int               IsTextured;
	DirectX::XMFLOAT3 ClipMin;
	float             Padding1;
	DirectX::XMFLOAT4 MaterialColor;
	DirectX::XMFLOAT2 TextureScales[MAX_TEXTURES];	// tx = [ [x, y], [x, y], ... ];
};

struct DXHUDBuffer
{
	DXMatrixBuffer    Matrices;
	DirectX::XMFLOAT4 MaterialColor;
	int               IsTransparent;
	DirectX::XMFLOAT3 Padding1;
};

struct DXSkyboxBuffer
{
	DXMatrixBuffer    Matrices;
	DirectX::XMFLOAT2 TextureScales[MAX_TEXTURES];	// tx = [ [x, y], [x, y], ... ];
};

struct DXSolidBuffer
{
	DXMatrixBuffer    Matrices;
	DirectX::XMFLOAT4 SolidColor;
};

struct DXTerrainBuffer
{
	DXMatrixBuffer    Matrices;
	DXLightBuffer     SunLight;
	DirectX::XMFLOAT3 Ambient;
	int               EnableClipping;
	DirectX::XMFLOAT3 ClipMax;
	float             Padding1;
	DirectX::XMFLOAT3 ClipMin;
	float             Padding2;
	DirectX::XMFLOAT2 TextureScales[MAX_TEXTURES];	// tx = [ [x, y], [x, y], ... ];
};

struct DXWaterBuffer
{
	DXCameraBuffer    CameraMain;
	DXMatrixBuffer    Matrices;
	DXLightBuffer     SunLight;
	int               EnableClipping;
	DirectX::XMFLOAT3 ClipMax;
	DirectX::XMFLOAT3 ClipMin;
	float             MoveFactor;
	float             WaveStrength;
	DirectX::XMFLOAT3 Padding1;
	DirectX::XMFLOAT2 TextureScales[MAX_TEXTURES];	// tx = [ [x, y], [x, y], ... ];
};

#endif

#ifndef _DELETEP
	#define _DELETEP(x) if (x != nullptr) { delete x; x = nullptr; }
#endif

#ifndef _RELEASEP
	#define _RELEASEP(x) if (x != nullptr) { x->Release(); x = nullptr; }
#endif

#ifndef GE3D_UTILS_H
	#include "system/Utils.h"
#endif

#ifndef GE3D_NOISE_H
	#include "system/Noise.h"
#endif

#ifndef GE3D_INPUTMANAGER_H
	#include "input/InputManager.h"
#endif

#ifndef GE3D_RAYCAST_H
	#include "physics/RayCast.h"
#endif

#ifndef GE3D_PHYSICSENGINE_H
	#include "physics/PhysicsEngine.h"
#endif

#ifndef GE3D_FRAMEBUFFER_H
	#include "scene/FrameBuffer.h"
#endif

#ifndef GE3D_DXCONTEXT_H
	#include "render/DXContext.h"
#endif

#ifndef GE3D_RENDERENGINE_H
	#include "render/RenderEngine.h"
#endif

#ifndef GE3D_SHADERMANAGER_H
	#include "render/ShaderManager.h"
#endif

#ifndef GE3D_SHADERPROGRAM_H
	#include "render/ShaderProgram.h"
#endif

#ifndef GE3D_BUFFER_H
	#include "scene/Buffer.h"
#endif

#ifndef GE3D_COMPONENT_H
	#include "scene/Component.h"
#endif

#ifndef GE3D_MESH_H
	#include "scene/Mesh.h"
#endif

#ifndef GE3D_BOUNDINGVOLUME_H
	#include "scene/BoundingVolume.h"
#endif

#ifndef GE3D_CAMERA_H
	#include "scene/Camera.h"
#endif

#ifndef GE3D_HUD_H
	#include "scene/HUD.h"
#endif

#ifndef GE3D_MODEL_H
	#include "scene/Model.h"
#endif

#ifndef GE3D_SCENEMANAGER_H
	#include "scene/SceneManager.h"
#endif

#ifndef GE3D_SKYBOX_H
	#include "scene/Skybox.h"
#endif

#ifndef GE3D_TERRAIN_H
	#include "scene/Terrain.h"
#endif

#ifndef GE3D_TEXTURE_H
	#include "scene/Texture.h"
#endif

#ifndef GE3D_WATERFBO_H
	#include "scene/WaterFBO.h"
#endif

#ifndef GE3D_WATER_H
	#include "scene/Water.h"
#endif

#ifndef GE3D_TIMEMANAGER_H
	#include "time/TimeManager.h"
#endif

#ifndef GE3D_WINDOW_H
	#include "ui/Window.h"
#endif

#ifndef GE3D_WINDOWFRAME_H
	#include "ui/WindowFrame.h"
#endif

#endif
