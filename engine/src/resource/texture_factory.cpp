#include <resource/texture_factory.h>
#include <resource/stb_image_loader.h>
#include <core/context.h>

namespace terminus
{
    const char* cube_sides[] = {
                                "_posx.tga",
                                "_negx.tga",
                                "_posy.tga",
                                "_negy.tga",
                                "_posz.tga",
                                "_negz.tga"
                                };
    
    namespace texture_factory
    {
        struct CreateTextureTaskData
        {
            asset_common::ImageLoadData* load_data;
            Texture2D**                   texture;
        };
        
        void create_texture_task(void* data)
        {
            CreateTextureTaskData* task_data = (CreateTextureTaskData*)data;
            
            Texture2DCreateDesc desc;
            
            desc.width = task_data->load_data->width;
            desc.height = task_data->load_data->height;
            desc.data = task_data->load_data->bytes;
            desc.format = TextureFormat::R8G8B8A8_UNORM;
            desc.generate_mipmaps = true;
            
            *task_data->texture = context::get_render_device().create_texture_2d(desc);
            
            free(task_data->load_data->bytes);
            free(task_data->load_data);
        }
        
        struct CreateTextureCubeTaskData
        {
            TextureCubeCreateDesc desc;
            TextureCube**         texture;
        };
        
        void create_texture_cube_task(void* data)
        {
            CreateTextureCubeTaskData* task_data = (CreateTextureCubeTaskData*)data;
            *task_data->texture = context::get_render_device().create_texture_cube(task_data->desc);
        }

        
        Texture* create(String image_name)
        {
            String file_name = filesystem::get_file_name_and_extention(image_name);
            asset_common::ImageLoadData* data = stb_image_loader::load(file_name);
            
            if(!data)
                return nullptr;
            
            Task task;
            Texture2D* texture;
            
            CreateTextureTaskData* texture_task_data = task_data<CreateTextureTaskData>(task);
            
            texture_task_data->load_data = data;
            texture_task_data->texture = &texture;
            
            task._function.Bind<&create_texture_task>();
            
            submit_gpu_upload_task(task);
            
            std::cout << "Loaded Texture : " << image_name << std::endl;
            
            return texture;
        }
        
        TextureCube* create_cubemap(StringBuffer32 cube_base_name)
        {
            Task task;
            TextureCube* texture = nullptr;
            
            CreateTextureCubeTaskData* texture_task_data = task_data<CreateTextureCubeTaskData>(task);
            texture_task_data->texture = &texture;
            
            asset_common::ImageLoadData* load_data[6];
            
            for(int i = 0; i < 6; i++)
            {
                StringBuffer32 temp_name = cube_base_name;
                StringBuffer32 file_name = temp_name + cube_sides[i];
                std::cout << "Loading cube map side : " << file_name.c_str() << std::endl;
                
                load_data[i] = stb_image_loader::load(file_name.c_str());
                
                if(load_data[i] != nullptr)
                {
                    texture_task_data->desc.data[i] = (void*)load_data[i]->bytes;
                    texture_task_data->desc.width = load_data[i]->width;
                    texture_task_data->desc.height = load_data[i]->height;
                    texture_task_data->desc.format = TextureFormat::R8G8B8A8_UNORM;
                    texture_task_data->desc.create_render_target_view = false;
                    texture_task_data->desc.generate_mipmaps = true;
                }
                else
                {
                    std::cout << "Failed to load cube map side : " << file_name.c_str() << std::endl;
                    return nullptr;
                }
            }
            
            task._function.Bind<&create_texture_cube_task>();
            
            submit_gpu_upload_task(task);
            
            std::cout << "Loaded Texture Cube : " << cube_base_name.c_str() << std::endl;
            
            for(int i = 0; i < 6; i++)
            {
                if(load_data[i])
                {
                    free(load_data[i]->bytes);
                    free(load_data[i]);
                }
            }
            
            return texture;

        }
    }
}
