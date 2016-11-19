#include "Graphics/Config.h"
#include "Platform/PlatformBackend.h"
#include "GUI/ImGuiBackend.h"
#include "Graphics/CommandList.h"
#include "Memory/PoolAllocator.h"
#include "Memory/StackAllocator.h"
#include "Global.h"
#include "IO/FileSystem.h"
#include "IO/FileWatcher.h"
#include "Input/Input.h"
#include "Input/InputContext.h"
#include "Core/Event/EventHandler.h"
#include "Resource/ShaderCache.h"
#include "Resource/MeshCache.h"
#include "Resource/AssimpMeshLoader.h"
#include "Resource/TSMLoader.h"
#include "Resource/TextLoader.h"
#include "Resource/StbImageLoader.h"
#include "Resource/TextureCache.h"
#include "Resource/AssetCommon.h"
#include <iostream>
#include "Graphics/RenderDevice.h"
#include "Math/MathUtility.h"
#include "Utility/MeshImporter.h"
#include "ECS/World.h"

class Test
{
public:
    Test()
    {
        EventCallback callback;
        callback.Bind<Test, &Test::OnStateInput>(this);
        Terminus::EventHandler::RegisterListener(InputStateEvent::sk_Type,  callback);
        
        callback.Bind<Test, &Test::OnActionInput>(this);
        Terminus::EventHandler::RegisterListener(InputActionEvent::sk_Type, callback);
        
        callback.Bind<Test, &Test::OnAxisInput>(this);
        Terminus::EventHandler::RegisterListener(InputAxisEvent::sk_Type, callback);
    }
    
    void OnStateInput(Event* _event)
    {
        InputStateEvent* event = (InputStateEvent*)_event;
        
        std::cout << event->GetName() << " "  <<  event->GetState() << std::endl;
    }
    
    void OnActionInput(Event* _event)
    {
        InputActionEvent* event = (InputActionEvent*)_event;
        
        std::string action;
        
        if(event->GetValue() == 0)
            action = "Released";
        else
            action = "Pressed";
        
        std::cout << event->GetName() << " "  << action << std::endl;

    }
    
    void OnAxisInput(Event* _event)
    {
        InputAxisEvent* event = (InputAxisEvent*)_event;
        std::cout << event->GetName() << " Axis : "  << event->GetAxis() << ", Value : " << event->GetValue() << " , Delta : " << event->GetDelta() << std::endl;
    }
};

// Global Data

Vertex* verticesList;
unsigned int* indicesList;

Matrix4 model;
Matrix4 view;
Matrix4 projection;

Mesh* testMesh;

Terminus::Graphics::VertexBuffer* vertexBuffer;
Terminus::Graphics::IndexBuffer* indexBuffer;
Terminus::Graphics::UniformBuffer* uniformBuffer;
Terminus::Graphics::VertexArray* vertexArray;
Terminus::Graphics::ShaderProgram* shaderProgram;
Terminus::Graphics::DepthStencilState* depthStencilState;
Terminus::Graphics::RasterizerState* rasterizerState;
Terminus::Graphics::Texture* texture;
Terminus::Graphics::SamplerState* samplerState;

Terminus::Resource::ShaderCache shaderCache;
Terminus::Resource::TextureCache textureCache;
Terminus::Resource::MeshCache meshCache;
Terminus::Graphics::RenderDevice render_device;
Terminus::Resource::MaterialCache materialCache;

Terminus::ECS::World entity_world;

// Init method declarations

void SetupCube();
void SetupMatrices();
void SetupGraphicsResources();
void CleanUpGraphicsResources();
void DrawScene();
void DrawMesh(Mesh* mesh);

int main(void)
{
    // Engine init
    
    Terminus::Global::Initialize();

    FileSystem::add_directory("Assets");
    FileWatcher::add_directory("Assets");
    
    if(!PlatformBackend::Initialize(800, 600))
        return -1;
    
    Input::Initialize();
    
	render_device.Initialize(nullptr, 0);
	Terminus::ImGuiBackend::initialize(render_device);
   
    SetupMatrices();
    SetupGraphicsResources();
	SetupCube();

	//Terminus::Utility::ImportMesh("Assets/Nanosuit/nanosuit.obj");

    InputContext* context = Input::CreateContext();
    context->m_ContextName = "Test";
    context->m_KeyboardStateMap[GLFW_KEY_E] = "HELLO WORLD";
    
    context->m_KeyboardActionMap[GLFW_KEY_Q] = "Fire";
    
    context->m_KeyboardAxisPositiveMap[GLFW_KEY_W] = "Forward";
    context->m_KeyboardAxisNegativeMap[GLFW_KEY_S] = "Forward";

    Input::SetActiveContext("Test");
    
    Test test;

    while(!PlatformBackend::IsShutdownRequested())
    {
        PlatformBackend::Update();
        FileWatcher::update();
        Terminus::EventHandler::Update();

		Terminus::ImGuiBackend::new_frame();

		bool open = true;

		ImGui::ShowTestWindow(&open);

        DrawScene();
		Terminus::ImGuiBackend::render();

		render_device.SwapBuffers();
    }
    
    CleanUpGraphicsResources();
    
	Terminus::ImGuiBackend::shutdown();
	render_device.Shutdown();

    PlatformBackend::Shutdown();
    
    Terminus::Global::Shutdown();
    
    return 0;
}

void SetupCube()
{
	materialCache.Initialize(&render_device, &textureCache);

	meshCache.Initialize(&render_device, &materialCache);
	meshCache.RegisterLoader<Terminus::Resource::AssimpMeshLoader>();
	meshCache.RegisterLoader<Terminus::Resource::TSMLoader>();

	//testMesh = meshCache.Load("cube.tsm");
	testMesh = meshCache.Load("Mesh/nanosuit.tsm");
}

void DrawMesh(Mesh* mesh)
{
	render_device.BindVertexArray(mesh->VertexArray);

	for (int i = 0; i < mesh->MeshCount; i++)
	{
		render_device.DrawIndexedBaseVertex(mesh->SubMeshes[i].m_IndexCount, mesh->SubMeshes[i].m_BaseIndex, mesh->SubMeshes[i].m_BaseVertex);
	}
}

void SetupMatrices()
{
    // Matrices
    float aspectRatio = static_cast<float>(PlatformBackend::GetWidth()) / static_cast<float>(PlatformBackend::GetHeight());
    
    model = Matrix4();
    
    view = glm::lookAtRH(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    projection = Terminus::Math::Perspective(45.0f , aspectRatio, 0.1f, 1000.0f);
}

void SetupGraphicsResources()
{
	shaderCache.Initialize(&render_device);
	textureCache.Initialize(&render_device);

    shaderCache.RegisterLoader<Terminus::Resource::TextLoader>();
    textureCache.RegisterLoader<Terminus::Resource::StbImageLoader>();
    
    // Setup graphics resources
    
    texture = textureCache.Load("brick.png");
    
    samplerState = render_device.CreateSamplerState(TextureFilteringMode::LINEAR_ALL, TextureFilteringMode::LINEAR_ALL, TextureWrapMode::REPEAT, TextureWrapMode::REPEAT, TextureWrapMode::REPEAT);
    
	uniformBuffer = render_device.CreateUniformBuffer(NULL, sizeof(Matrix4) * 3, BufferUsageType::DYNAMIC);

    depthStencilState = render_device.CreateDepthStencilState();
	rasterizerState = render_device.CreateRasterizerState();
    
	render_device.BindDepthStencilState(depthStencilState);
	render_device.BindRasterizerState(rasterizerState);
    
	shaderProgram = shaderCache.Load("Shaders/Basic_Vertex", "Shaders/Basic_Pixel");
    
    char* ptr = (char*)render_device.MapBuffer(uniformBuffer, BufferMapType::WRITE);
    
    memcpy(ptr, &model, sizeof(glm::mat4));
    memcpy(ptr + sizeof(glm::mat4), &view, sizeof(glm::mat4));
    memcpy(ptr + sizeof(glm::mat4) * 2, &projection, sizeof(glm::mat4));

	render_device.UnmapBuffer(uniformBuffer);
	render_device.SetPrimitiveType(DrawPrimitive::TRIANGLES);
}

void DrawScene()
{
	float seconds = glfwGetTime();

	model = glm::rotate(glm::radians(seconds * 25.0f), Vector3(0.0f, 1.0f, 1.0f));

	char* memPtr = (char*)render_device.MapBuffer(uniformBuffer, BufferMapType::WRITE);

	memcpy(memPtr, &model, sizeof(glm::mat4));
	memcpy(memPtr + sizeof(glm::mat4), &view, sizeof(glm::mat4));
	memcpy(memPtr + sizeof(glm::mat4) * 2, &projection, sizeof(glm::mat4));

	render_device.UnmapBuffer(uniformBuffer);

	render_device.BindFramebuffer(nullptr);
	render_device.SetViewport(PlatformBackend::GetWidth(), PlatformBackend::GetHeight(), 0, 0);
    
	render_device.ClearFramebuffer(FramebufferClearTarget::ALL, Vector4(0.3f, 0.3f, 0.3f, 1.0f));
    
    // Bind Shader Program
	render_device.BindShaderProgram(shaderProgram);
	render_device.BindSamplerState(samplerState, ShaderType::PIXEL ,0);
	render_device.BindTexture(texture, ShaderType::PIXEL, 0);
    // Bind Uniform Buffer
	render_device.BindUniformBuffer(uniformBuffer, ShaderType::VERTEX, 0);
    
	DrawMesh(testMesh);
}

void CleanUpGraphicsResources()
{
	delete[] verticesList;
	delete[] indicesList;

	meshCache.Unload(testMesh);

	render_device.DestroyTexture2D((Terminus::Graphics::Texture2D*)texture);
	render_device.DestroyDepthStencilState(depthStencilState);
	render_device.DestroySamplerState(samplerState);
	render_device.DestroyRasterizerState(rasterizerState);
    render_device.DestroyUniformBuffer(uniformBuffer);
}
