#include <Core/rendering_thread.h>
#include <Core/context.h>
#include <Graphics/imgui_backend.h>
#include <Core/config.h>

namespace terminus
{
    // TEST TEST TEST
    
    int cmd_buf_idx;
    
    String scene_path = "";
    bool file_dialog = false;
    
    // TEST TEST TEST
    
    RenderingThread::RenderingThread()
    {
        
    }
    
    RenderingThread::~RenderingThread()
    {
    
    }
    
    void RenderingThread::run()
    {
        _thread = std::thread(&RenderingThread::render_loop, this);
    }
    
    void RenderingThread::enqueue_upload_task(Task& task)
    {
        
    }
    
    void RenderingThread::shutdown()
    {
        Context& context = Global::get_context();
        
        context._renderer.shutdown();
        context._render_device.Shutdown();
        
        _thread.join();
    }
    
    void RenderingThread::render_loop()
    {
        Context& context = Global::get_context();
        
        context._main_ready_sema.wait();
        
        context._render_device.Initialize();
        context._renderer.initialize();
        
#if defined(TERMINUS_WITH_EDITOR)
        ImGuiBackend::initialize();
#endif
        
        // TEST TEST TEST
        
        cmd_buf_idx = context::get_renderer().create_command_buffer();
        
        // TEST TEST TEST
        
        context._render_ready_sema.notify();
        
        while (!context._shutdown)
        {
            // temp
            // TEST TEST TEST
            
            CommandBuffer& cmd_buf = context::get_renderer().command_buffer(cmd_buf_idx);
            
            BindFramebufferCmdData cmd1;
            cmd1.framebuffer = nullptr;
            
            cmd_buf.Write(CommandType::BindFramebuffer);
            cmd_buf.Write(&cmd1, sizeof(cmd1));
            
            ClearFramebufferCmdData cmd2;
            cmd2.clear_target = FramebufferClearTarget::ALL;
            cmd2.clear_color = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
            
            cmd_buf.Write(CommandType::ClearFramebuffer);
            cmd_buf.Write(&cmd2, sizeof(cmd2));
            
            cmd_buf.WriteEnd();
            
            // TEST TEST TEST
            
#if defined(TERMINUS_WITH_EDITOR)
            ImGuiBackend::new_frame();
            static bool testWin = true;
            ImGui::ShowTestWindow(&testWin);
            
            // TEST
            
            ImGui::SetNextWindowSize(ImVec2(550,680), ImGuiSetCond_FirstUseEver);
            ImGui::Begin("Scene Load", &testWin, 0);
            
            ImGui::Text("%s", scene_path.c_str());
            
            ImGui::SameLine();
            
            if(ImGui::Button("Browse..."))
            {
                file_dialog = true;
            }
            if(ImGui::Button("Load Scene"))
            {
                if(scene_path != "")
                {
                    String trimmed_path = filesystem::get_file_name_and_extention(scene_path);
                    std::cout << "Trimmed Path : " << trimmed_path << std::endl;
                    context::get_scene_manager().Preload(trimmed_path);
                }
                else
                {
                    std::cout << "Invalid Path" << std::endl;
                }
            }
            
            ImGui::End();
            // TEST
            
#endif

            
            // submit api calls
            context._renderer.submit();
            
            // do resource uploading. one task per frame for now.
            if(!concurrent_queue::empty(_graphics_upload_queue))
            {
                Task upload_task = concurrent_queue::pop(_graphics_upload_queue);
                upload_task._function.Invoke(&upload_task._data[0]);
            }
            
            // notify done
            context._render_done_sema.notify();
            
            // wait for swap done
            context._swap_done_sema.wait();
            
        }
        
        shutdown();
    }
}
