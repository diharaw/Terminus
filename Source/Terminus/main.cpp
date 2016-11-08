#include "Graphics\Config.h"
#include "Platform/PlatformBackend.h"
#include "GUI/ImGuiBackend.h"
#include "Graphics/RenderConfigUI.h"
#include "Graphics/CommandList.h"
#include "Graphics/RenderSystem.h"
#include <glm.hpp>
#include "Memory/PoolAllocator.h"
#include "Memory/StackAllocator.h"
#include "GlobalMemory.h"
#include "IO/FileSystem.h"
#include "IO/FileWatcher.h"
#include "Input/Input.h"
#include "Input/InputContext.h"
#include "Core/Event/EventHandler.h"
#include "Resource/ShaderCache.h"
#include "Resource/TextLoader.h"
#include "Resource/StbLoader.h"
#include "Resource/TextureCache.h"
#include "Resource/AssetCommon.h"
#include <iostream>
#include "Graphics/RenderDevice.h"

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

Terminus::Graphics::RenderDevice render_device;

// Init method declarations

void SetupCube();
void SetupMatrices();
void SetupGraphicsResources();
void CleanUpGraphicsResources();
void DrawScene();

int main(void)
{
    // Engine init
    
    Terminus::Memory::Initialize();

    FileSystem::add_directory("Assets");
	
    FileWatcher::add_directory("Assets");
    
    if(!PlatformBackend::Initialize(800, 600))
        return -1;
    
    Input::Initialize();
    
	render_device.Initialize(nullptr, 0);
	imgui_backend::initialize();
    
    SetupCube();
    SetupMatrices();
    SetupGraphicsResources();

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
        //glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        //glClear(GL_COLOR_BUFFER_BIT);
        
        PlatformBackend::Update();
        FileWatcher::update();
        Terminus::EventHandler::Update();

        DrawScene();
		render_device.SwapBuffers();
    }
    
    CleanUpGraphicsResources();
    
	imgui_backend::shutdown();
    PlatformBackend::Shutdown();
    
    Terminus::Memory::Shutdown();
    
    return 0;
}


void SetupCube()
{
    // Cube Vertex data
    
    verticesList = new Vertex[8];
    indicesList = new unsigned int[36];
    
    verticesList[0].m_Position = Vector3(-1.0f, -1.0f, 1.0f); // Front Bottom Left
    verticesList[0].m_TexCoord = Vector2(0.0f, 1.0f);
    
    verticesList[1].m_Position = Vector3(-1.0f, 1.0f, 1.0f);  // Front Top Left
    verticesList[1].m_TexCoord = Vector2(0.0f, 0.0f);
    
    verticesList[2].m_Position = Vector3(1.0f, -1.0f, 1.0f);  // Front Bottom Right
    verticesList[2].m_TexCoord = Vector2(1.0f, 1.0f);
    
    verticesList[3].m_Position = Vector3(1.0f, 1.0f, 1.0f);   // Front Top Right
    verticesList[3].m_TexCoord = Vector2(1.0f, 0.0f);
    
    verticesList[4].m_Position = Vector3(-1.0f, -1.0f, -1.0f); // Back Bottom Left
    verticesList[4].m_TexCoord = Vector2(0.0f, 1.0f);
    
    verticesList[5].m_Position = Vector3(-1.0f, 1.0f, -1.0f);  // Back Top Left
    verticesList[5].m_TexCoord = Vector2(0.0f, 0.0f);
    
    verticesList[6].m_Position = Vector3(1.0f, -1.0f, -1.0f);  // Back Bottom Right
    verticesList[6].m_TexCoord = Vector2(1.0f, 1.0f);
    
    verticesList[7].m_Position = Vector3(1.0f, 1.0f, -1.0f);   // Back Top Right
    verticesList[7].m_TexCoord = Vector2(1.0f, 0.0f);
    
    indicesList[0] = 0; // Front
    indicesList[1] = 2;
    indicesList[2] = 1;
    indicesList[3] = 2;
    indicesList[4] = 3;
    indicesList[5] = 1;
    
    indicesList[6] = 0;  // Left
    indicesList[7] = 5;
    indicesList[8] = 4;
    indicesList[9] = 0;
    indicesList[10] = 1;
    indicesList[11] = 5;
    
    indicesList[12] = 3; // Right
    indicesList[13] = 2;
    indicesList[14] = 7;
    indicesList[15] = 7;
    indicesList[16] = 2;
    indicesList[17] = 6;
    
    indicesList[18] = 5; // Up
    indicesList[19] = 1;
    indicesList[20] = 7;
    indicesList[21] = 7;
    indicesList[22] = 1;
    indicesList[23] = 3;
    
    indicesList[24] = 0; // Down
    indicesList[25] = 4;
    indicesList[26] = 6;
    indicesList[27] = 6;
    indicesList[28] = 2;
    indicesList[29] = 0;
    
    indicesList[30] = 7; // Back
    indicesList[31] = 6;
    indicesList[32] = 4;
    indicesList[33] = 4;
    indicesList[34] = 5;
    indicesList[35] = 7;
}

void SetupMatrices()
{
    // Matrices
    float aspectRatio = static_cast<float>(PlatformBackend::GetWidth()) / static_cast<float>(PlatformBackend::GetHeight());
    
    model = Matrix4();
    
    view = glm::lookAtRH(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    projection = glm::perspectiveRH(45.0f , aspectRatio, 0.1f, 1000.0f);
}

void SetupGraphicsResources()
{
	shaderCache.Initialize(&render_device);
	textureCache.Initialize(&render_device);

    shaderCache.RegisterLoader<Terminus::Resource::TextLoader>();
    textureCache.RegisterLoader<Terminus::Resource::StbLoader>();
    
    // Setup graphics resources
    
    texture = textureCache.Load("brick.png");
    
    samplerState = render_device.CreateSamplerState(TextureFilteringMode::LINEAR_ALL, TextureFilteringMode::LINEAR_ALL, TextureWrapMode::REPEAT, TextureWrapMode::REPEAT, TextureWrapMode::REPEAT);
    
	vertexBuffer  = render_device.CreateVertexBuffer(&verticesList[0], sizeof(Vertex) * 8, BufferUsageType::STATIC);
	indexBuffer   = render_device.CreateIndexBuffer(&indicesList[0], sizeof(unsigned int) * 36, BufferUsageType::STATIC);
	vertexArray   = render_device.CreateVertexArray(vertexBuffer, indexBuffer, InputLayoutType::STANDARD_VERTEX);
	uniformBuffer = render_device.CreateUniformBuffer(NULL, sizeof(Matrix4) * 3, BufferUsageType::DYNAMIC);

    depthStencilState = render_device.CreateDepthStencilState();
	rasterizerState = render_device.CreateRasterizerState();
    
	render_device.BindDepthStencilState(depthStencilState);
	render_device.BindRasterizerState(rasterizerState);
    
	shaderProgram = shaderCache.Load("Shaders/Basic_Vertex.glsl", "Shaders/Basic_Pixel.glsl");
    
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
	render_device.BindTexture(texture, ShaderType::PIXEL);
    // Bind Uniform Buffer
	render_device.BindUniformBuffer(uniformBuffer, ShaderType::VERTEX, 0);
    
    // Bind Vertex Array
	render_device.BindVertexArray(vertexArray);

	render_device.DrawIndexed(36);
}

void CleanUpGraphicsResources()
{
	delete[] verticesList;
	delete[] indicesList;

	render_device.DestroyDepthStencilState(depthStencilState);
	render_device.DestroySamplerState(samplerState);
	render_device.DestroyRasterizerState(rasterizerState);
    render_device.DestroyIndexBuffer(indexBuffer);
    render_device.DestroyVertexBuffer(vertexBuffer);
	render_device.DestroyVertexArray(vertexArray);
    render_device.DestroyUniformBuffer(uniformBuffer);
}
