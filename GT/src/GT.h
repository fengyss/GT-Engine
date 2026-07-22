#pragma once

// for use by GT applications
//#include <cstdio>
#include "GT/Core/Application.h"
#include "GT/Core/Log.h"
#include "GT/Core/Layer.h"
#include "GT/Core/LayerStack.h"
#include "GT/Core/Timestep.h"
#include "GT/Core/Input.h"
#include "GT/Core/Buffer.h"
#include "GT/Core/KeyCodes.h"
#include "GT/Core/MouseButtoncodes.h"
#include "GT/Core/TextureSheet.h"


#include "GT/Core/Asset/AssetImporter.h"
#include "GT/Core/Asset/AssetManager.h"

#include "GT/ImGui/ImGuiLayer.h"


// -------Renderer------------

#include "GT/Renderer/Mesh.h"
#include "GT/Renderer/Model.h"

#include "GT/Renderer/Renderer.h"
#include "GT/Renderer/Renderer2D.h"
#include "GT/Renderer/Renderer3D.h"
#include "GT/Renderer/RenderCommand.h"

#include "GT/Renderer/VertexArray.h"
#include "GT/Renderer/Buffer.h"
#include "GT/Renderer/Shader.h"
#include "GT/Renderer/Texture.h"
#include "GT/Renderer/Framebuffer.h"


#include "GT/Renderer/ShadowMap.h"

#include "GT/Renderer/Camera.h"
#include "GT/Renderer/EditorCamera.h"
#include "GT/Renderer/OrthographicCamera.h"
#include "GT/OrthographicCameraController.h"

// ----------Scene----------------

#include "GT/Scene/Scene.h"
#include "GT/Scene/SceneCamera.h"
#include "GT/Scene/Components.h"
#include "GT/Scene/Entity.h"
#include "GT/Scene/ScriptableEntity.h"
