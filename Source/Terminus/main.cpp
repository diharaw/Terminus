#include "Platform/PlatformBackend.h"
#include "Graphics/RenderBackend.h"
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

ResourceHandle vertexBuffer;
ResourceHandle indexBuffer;
ResourceHandle uniformBuffer;
ResourceHandle vertexArray;
ResourceHandle shaderProgram;
ResourceHandle depthStencilState;
ResourceHandle rasterizerState;
ResourceHandle texture;
ResourceHandle samplerState;

CommandData::DrawIndexed indexedDrawCmdData;
CommandData::UniformBufferCopy copyCmdData;
CommandData::ClearRenderTarget clearRenderTargetData;

ShaderCache shaderCache;
TextureCache textureCache;

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
    
    RenderBackend::Initialize();
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
        
		RenderBackend::SwapBuffers();
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
    shaderCache.RegisterLoader<TextLoader>();
    textureCache.RegisterLoader<StbLoader>();
    
    // Setup graphics resources
    
    GL_CHECK_ERROR(texture = textureCache.Load("brick.png"));
    
    GL_CHECK_ERROR(samplerState = RenderBackend::CreateSamplerState(LINEAR_ALL, LINEAR_ALL, REPEAT, REPEAT, REPEAT));
    
	vertexBuffer  = RenderBackend::CreateVertexBuffer(&verticesList[0], sizeof(Vertex) * 8, USAGE_STATIC);
	indexBuffer   = RenderBackend::CreateIndexBuffer(&indicesList[0], sizeof(unsigned int) * 36, USAGE_STATIC);
	vertexArray   = RenderBackend::CreateVertexArray(vertexBuffer, indexBuffer, LAYOUT_STANDARD_VERTEX);
	uniformBuffer = RenderBackend::CreateUniformBuffer(NULL, sizeof(Matrix4) * 3, USAGE_DYNAMIC);

    depthStencilState = RenderBackend::CreateDepthStencilState();
	rasterizerState = RenderBackend::CreateRasterizerState();
    
    RenderBackend::SetDepthStencilState(depthStencilState);
	RenderBackend::SetRasterizerState(rasterizerState);
    
	shaderProgram = shaderCache.Load("Shaders/Basic_Vertex.glsl", "Shaders/Basic_Pixel.glsl");
    
    indexedDrawCmdData.IndexCount = 36;
    
    char* ptr = (char*)malloc(sizeof(Matrix4) * 3);
    
    memcpy(ptr, &model, sizeof(glm::mat4));
    memcpy(ptr + sizeof(glm::mat4), &view, sizeof(glm::mat4));
    memcpy(ptr + sizeof(glm::mat4) * 2, &projection, sizeof(glm::mat4));
    
    clearRenderTargetData.ClearColor = Vector4(0.3f, 0.3f, 0.3f, 1.0f);
    clearRenderTargetData.Target = FramebufferClearTarget::FB_TARGET_ALL;
    
    copyCmdData.Data = (void*)ptr;
    copyCmdData.Buffer = uniformBuffer;
    copyCmdData.Size = sizeof(Matrix4) * 3;
	copyCmdData.MapType = BufferMapType::MAP_WRITE;
    
	GPUCommand::UniformBufferCopy(&copyCmdData);
}

void DrawScene()
{
	float seconds = glfwGetTime();

	model = glm::rotate(glm::radians(seconds * 25.0f), Vector3(0.0f, 1.0f, 1.0f));

	char* memPtr = (char*)copyCmdData.Data;

	memcpy(memPtr, &model, sizeof(glm::mat4));
	memcpy(memPtr + sizeof(glm::mat4), &view, sizeof(glm::mat4));
	memcpy(memPtr + sizeof(glm::mat4) * 2, &projection, sizeof(glm::mat4));

	RenderBackend::BindFramebuffer();
	RenderBackend::SetViewport(PlatformBackend::GetWidth(), PlatformBackend::GetHeight(), 0, 0);
    
	GPUCommand::ClearRenderTarget(&clearRenderTargetData);
    
    // Bind Shader Program
    RenderBackend::BindShaderProgram(shaderProgram);
    GL_CHECK_ERROR(RenderBackend::BindSamplerState(samplerState, 0, SHADER_PIXEL));
    GL_CHECK_ERROR(RenderBackend::BindTexture2D(texture, SHADER_PIXEL));
    // Bind Uniform Buffer
	GPUCommand::UniformBufferCopy(&copyCmdData);
	RenderBackend::BindUniformBuffer(uniformBuffer, SHADER_VERTEX, 0);
    
    // Bind Vertex Array
	RenderBackend::BindVertexArray(vertexArray);

    GPUCommand::DrawIndexed(&indexedDrawCmdData);
}

void CleanUpGraphicsResources()
{
    RenderBackend::DestroyIndexBuffer(indexBuffer);
    RenderBackend::DestroyVertexBuffer(vertexBuffer);
	RenderBackend::DestroyVertexArray(vertexArray);
    RenderBackend::DestroyUniformBuffer(uniformBuffer);
	RenderBackend::DestroyShaderProgram(shaderProgram);
}
