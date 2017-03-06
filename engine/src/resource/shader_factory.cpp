#include <resource/scene_factory.h>
#include <io/filesystem.h>
#include <core/context.h>

namespace terminus
{
    namespace shader_factory
    {
        struct CreateShaderTaskData
        {
            ShaderType      _type;
            char*           _buffer;
            Shader**        _shader;
        };
        
        struct CreateProgramTaskData
        {
            Shader* _vs;
            Shader* _ps;
            Shader* _tcs;
            Shader* _tes;
            Shader* _gs;
            ShaderProgram** _program;
        };
        
        void create_shader_task(void* data)
        {
            CreateShaderTaskData* task_data = (CreateShaderTaskData*)data;
            *task_data->_shader = context::get_render_device().CreateShader(task_data->_type, task_data->_buffer);
        }
        
        void create_program_task(void* data)
        {
            CreateProgramTaskData* task_data = (CreateProgramTaskData*)data;
            *task_data->_program = context::get_render_device().CreateShaderProgram(task_data->_vs, task_data->_ps);
        }
        
        Shader* create(asset_common::TextLoadData* data)
        {
            Shader* shader = nullptr;
            Task task;
            CreateShaderTaskData* shader_task_data = task_data<CreateShaderTaskData>(task);
            
            shader_task_data->_buffer = data->buffer;
            shader_task_data->_type = data->shader_type;
            shader_task_data->_shader = &shader;
            task._function.Bind<&create_shader_task>();
            
            submit_gpu_upload_task(task);
            
            if (!shader)
                return nullptr;
            
            return shader;
        }
        
        Shader* create(StringList& defines, ShaderType type)
        {
            String shader_name;
            String extension;
            
#if defined(TERMINUS_OPENGL)
            extension = ".glsl";
#else
            extension = ".hlsl";
#endif
            
            switch (type)
            {
                case ShaderType::VERTEX:
                    shader_name = "VS_Template";
                    break;
                case ShaderType::GEOMETRY:
                    shader_name = "GS_Template";
                    break;
                case ShaderType::TESSELLATION_CONTROL:
                    shader_name = "TCS_Template";
                    break;
                case ShaderType::TESSELLATION_EVALUATION:
                    shader_name = "TES_Template";
                    break;
                case ShaderType::PIXEL:
                    shader_name = "PS_Template";
                    break;
                    
                default:
                    assert(false);
                    break;
            }
            
            shader_name += extension;
            
            FileHandle handle = filesystem::read_file(shader_name);
            String shader_source = String(handle.buffer);
            
            String pre_processed_source = StringUtility::GenerateSource(shader_source, defines);
            
            Shader* shader = context::get_render_device().CreateShader(type, pre_processed_source.c_str());
            
            if (!shader)
                return nullptr;
            
            return shader;
        }
        
        Shader* create(ShaderType type, String shader_template, StringList defines)
        {
            String pre_processed_source = StringUtility::GenerateSource(shader_template, defines);
            
            Shader* shader = nullptr;
            Task task;
            CreateShaderTaskData* shader_task_data = task_data<CreateShaderTaskData>(task);
            
            shader_task_data->_buffer = (char*)pre_processed_source.c_str();
            shader_task_data->_type = type;
            shader_task_data->_shader = &shader;
            task._function.Bind<&create_shader_task>();
            
            submit_gpu_upload_task(task);
            return shader;
        }
        
        ShaderProgram* create_program(Shader* vs, Shader* ps, Shader* tcs, Shader* tes, Shader* gs)
        {
            ShaderProgram* program = nullptr;
            Task task;
            CreateProgramTaskData* program_task_data = task_data<CreateProgramTaskData>(task);
            
            program_task_data->_vs = vs;
            program_task_data->_ps = ps;
            program_task_data->_tcs = tcs;
            program_task_data->_tes = tes;
            program_task_data->_gs = gs;
            program_task_data->_program = &program;
            
            task._function.Bind<&create_program_task>();
            
            submit_gpu_upload_task(task);
            return program;
        }
    }
}
