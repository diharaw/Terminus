#include "RenderConfigUI.h"
#include "../Platform/PlatformBackend.h"
#include "RenderSystem.h"
#include "RenderCommon.h"
#include <iostream>
#include <string>

#define PPCAT_NX(A, B) A ## B
#define PPCAT(A, B) PPCAT_NX(A, B)
#define STRINGIZE_NX(A) #A
#define STRINGIZE(A) STRINGIZE_NX(A)
#define UNIQUE_STRING(X, Y) STRINGIZE(PPCAT(X, Y))

namespace render_config_ui
{
	bool m_Open = true;
	bool m_NewRenderPass = false;
	bool m_AddRenderPass = false;
	bool m_RemoveRenderPass = false;
	RenderPass* m_CurrentRenderPass = nullptr;
    int m_ToBeRemovedRTIndex = -1;
    int m_ToBeRemovedFBIndex = -1;
    int m_ToBeRemovedSPIndex = -1;
    std::vector<char> m_tempstr;

	void new_render_pass()
	{
		if (m_NewRenderPass)
		{
			ImGui::SetNextWindowPos(ImVec2(0.0f, 20.0f), ImGuiSetCond_FirstUseEver);
			ImGui::SetNextWindowSize(ImVec2(600.0f, PlatformBackend::GetHeight() - 100.0f));

			if (!ImGui::Begin("New Render Pass", &m_NewRenderPass))
			{
				ImGui::End();
				return;
			}

			ImGui::InputText("Render Pass Name", m_CurrentRenderPass->m_NameChar.data(), 32);

			if (ImGui::CollapsingHeader("Framebuffers"))
			{
                for (int i = 0; i < m_CurrentRenderPass->m_Framebuffers.size(); i++)
                {
                    FramebufferInfo* current = m_CurrentRenderPass->m_Framebuffers[i];
                    
                    ImGui::Separator();
                    ImGui::PushID(i);
                    ImGui::InputText("Name", m_CurrentRenderPass->m_Framebuffers[i]->m_NameChar.data(), 25);
                    ImGui::InputInt("Width", &m_CurrentRenderPass->m_Framebuffers[i]->m_Width);
                    ImGui::InputInt("Height", &m_CurrentRenderPass->m_Framebuffers[i]->m_Height);
                    
                    if (ImGui::TreeNode("Render Targets"))
                    {
                        for (int j = 0; j < current->m_RenderTargets.size(); j++)
                        {
                            RenderTargetInfo* currentRt = current->m_RenderTargets[j];
                            ImGui::Separator();
                            ImGui::PushID(j);
                            ImGui::InputText("Name", currentRt->m_NameChar.data(), 25);
                            if(ImGui::Button("Remove"))
                            {
                                m_ToBeRemovedRTIndex = j;
                            }
                            ImGui::PopID();
                        }
                        
                        if(m_ToBeRemovedRTIndex != -1)
                        {
                            delete current->m_RenderTargets[m_ToBeRemovedRTIndex];
                            current->m_RenderTargets.erase(current->m_RenderTargets.begin() + m_ToBeRemovedRTIndex);
                            m_ToBeRemovedRTIndex = -1;
                        }
                        
                        ImGui::Separator();
                        
                        if(ImGui::Button("Add", ImVec2(80.0f, 30.0f)))
                        {
                            RenderTargetInfo* rtInfo = new RenderTargetInfo();
                            rtInfo->m_NameChar.resize(25);
                            current->m_RenderTargets.push_back(rtInfo);
                        }
                        
                        ImGui::TreePop();
                    }
                    
                    if(ImGui::Button("Remove"))
                    {
                        m_ToBeRemovedFBIndex = i;
                    }
                    
                    ImGui::PopID();
                }
                
                if(m_ToBeRemovedFBIndex != -1)
                {
                    delete m_CurrentRenderPass->m_Framebuffers[m_ToBeRemovedFBIndex];
                    m_CurrentRenderPass->m_Framebuffers.erase(m_CurrentRenderPass->m_Framebuffers.begin() + m_ToBeRemovedFBIndex);
                    m_ToBeRemovedFBIndex = -1;
                }
                
                ImGui::Separator();
                
                if(ImGui::Button("Add FBO", ImVec2(80.0f, 30.0f)))
                {
                    FramebufferInfo* fbo = new FramebufferInfo();
                    fbo->m_NameChar.resize(25);
                    m_CurrentRenderPass->m_Framebuffers.push_back(fbo);
                    
                }
			}
            
			if (ImGui::CollapsingHeader("Sub Passes"))
			{
                ImGui::PushID("SubPass");
                for (int i = 0; i < m_CurrentRenderPass->m_SubPasses.size(); i++)
                {
                    RenderSubPass* current = m_CurrentRenderPass->m_SubPasses[i];
                    ImGui::PushID(i);
                    ImGui::InputText("Name", current->m_NameChar.data(), 25);
                    ImGui::Button("Browse..");
                    ImGui::SameLine();
                    ImGui::Text("Vertex Shader");
                    
                    if(ImGui::Button("Remove"))
                    {
                        m_ToBeRemovedSPIndex = i;
                    }
                    
                    ImGui::PopID();
                }
                ImGui::Separator();
                
                if(m_ToBeRemovedSPIndex != -1)
                {
                    //delete m_CurrentRenderPass->m_SubPasses[m_ToBeRemovedSPIndex];
                    m_CurrentRenderPass->m_SubPasses.erase(m_CurrentRenderPass->m_SubPasses.begin() + m_ToBeRemovedSPIndex);
                    m_ToBeRemovedSPIndex = -1;
                }
                
                if(ImGui::Button("Add Sub Pass", ImVec2(80.0f, 30.0f)))
                {
                    RenderSubPass* sp = RenderSystem::create_render_subpass();
                    sp->m_NameChar.resize(25);
                    m_CurrentRenderPass->m_SubPasses.push_back(sp);
                }
                
                ImGui::PopID();
			}
            
            ImGui::Separator();
            ImGui::PushID("NewRenderPass");
            ImGui::Button("Save");
            ImGui::SameLine();
            ImGui::Button("Cancel");
            ImGui::PopID();
            
			ImGui::End();
		}
	}

	void load_render_pass()
	{

	}

	void remove_render_pass()
	{

	}

	void render_pipeline()
	{
		ImGui::SetNextWindowPos(ImVec2(PlatformBackend::GetWidth() - 300.0f, 20.0f), ImGuiSetCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(300.0f, PlatformBackend::GetHeight() - 100.0f));

		if (!ImGui::Begin("Render Pipeline", &m_Open))
		{
			ImGui::End();
			return;
		}

		ImGui::Text("RenderPass List");

		if (ImGui::Button("New", ImVec2(80.0f, 30.0f)))
		{
			m_NewRenderPass = true;
			m_CurrentRenderPass = RenderSystem::create_render_pass();
            m_CurrentRenderPass->m_NameChar.resize(25);
		}
			
		if (ImGui::Button("Load", ImVec2(80.0f, 30.0f)))
			load_render_pass();

		if (ImGui::Button("Remove", ImVec2(80.0f, 30.0f)))
			remove_render_pass();
			
		ImGui::End();

		new_render_pass();
	}
}