#include <resource/pipeline_state_object_factory.h>
#include <resource/text_loader.h>
#include <core/context.h>

namespace terminus
{
    namespace pipeline_state_object_factory
    {
        struct CreatePipelineState
        {
            PipelineStateObjectCreateDesc desc;
            PipelineStateObject** pso;
        };
        
        void create_pipeline_state_task(void* data)
        {
            CreatePipelineState* task_data = (CreatePipelineState*)data;
            *task_data->pso = context::get_render_device().create_pipeline_state_object(task_data->desc);
        }
        
        void parse_cmp_func(String& str, ComparisonFunction& cmp)
        {
            if(str == "NEVER")
                cmp = ComparisonFunction::NEVER;
            else if(str == "LESS")
                cmp = ComparisonFunction::LESS;
            else if(str == "EQUAL")
                cmp = ComparisonFunction::EQUAL;
            else if(str == "LESS_EQUAL")
                cmp = ComparisonFunction::LESS_EQUAL;
            else if(str == "GREATER")
                cmp = ComparisonFunction::GREATER;
            else if(str == "NOT_EQUAL")
                cmp = ComparisonFunction::NOT_EQUAL;
            else if(str == "GREATER_EQUAL")
                cmp = ComparisonFunction::GREATER_EQUAL;
            else if(str == "ALWAYS")
                cmp = ComparisonFunction::ALWAYS;
        }
        
        void parse_stencil_op(String& str, StencilOperation& op)
        {
            if(str == "KEEP")
                op = StencilOperation::KEEP;
            else if(str == "ZERO")
                op = StencilOperation::ZERO;
            else if(str == "REPLACE")
                op = StencilOperation::REPLACE;
            else if(str == "INCR_SAT")
                op = StencilOperation::INCR_SAT;
            else if(str == "DECR_SAT")
                op = StencilOperation::DECR_SAT;
            else if(str == "INVERT")
                op = StencilOperation::INVERT;
            else if(str == "INCR")
                op = StencilOperation::INCR;
            else if(str == "DECR")
                op = StencilOperation::DECR;
        }
        
        void parse_depth_stencil_state(JsonDocument& json, DepthStencilStateCreateDesc& desc)
        {
            if(json.HasMember("depth_stencil_state"))
            {
                rapidjson::Value& ds = json["depth_stencil_state"];
                
                desc.enable_depth_test = ds["depth_test"].GetBool();
                desc.enable_stencil_test = ds["stencil_test"].GetBool();
                desc.depth_mask = ds["depth_mask"].GetBool();
                
                String depth_cmp_func = ds["depth_cmp_func"].GetString();
                parse_cmp_func(depth_cmp_func, desc.depth_cmp_func);
                
                String front_stencil_fail = ds["front_stencil_fail"].GetString();
                parse_stencil_op(front_stencil_fail, desc.front_stencil_fail);
                
                String front_stencil_pass_depth_fail = ds["front_stencil_pass_depth_fail"].GetString();
                parse_stencil_op(front_stencil_pass_depth_fail, desc.front_stencil_pass_depth_fail);
                
                String front_stencil_pass_depth_pass = ds["front_stencil_pass_depth_pass"].GetString();
                parse_stencil_op(front_stencil_pass_depth_pass, desc.front_stencil_pass_depth_pass);
                
                String front_stencil_cmp_func = ds["front_stencil_cmp_func"].GetString();
                parse_cmp_func(front_stencil_cmp_func, desc.front_stencil_cmp_func);
                
                String back_stencil_fail = ds["back_stencil_fail"].GetString();
                parse_stencil_op(back_stencil_fail, desc.back_stencil_fail);
                
                String back_stencil_pass_depth_fail = ds["back_stencil_pass_depth_fail"].GetString();
                parse_stencil_op(back_stencil_pass_depth_fail, desc.back_stencil_pass_depth_fail);
                
                String back_stencil_pass_depth_pass = ds["back_stencil_pass_depth_pass"].GetString();
                parse_stencil_op(back_stencil_pass_depth_pass, desc.back_stencil_pass_depth_pass);
                
                String back_stencil_cmp_func = ds["back_stencil_cmp_func"].GetString();
                parse_cmp_func(back_stencil_cmp_func, desc.back_stencil_cmp_func);
                
                desc.stencil_mask = ds["stencil_mask"].GetInt();
            }
        }
        
        void parse_rasterizer_state(JsonDocument& json, RasterizerStateCreateDesc& desc)
        {
            if(json.HasMember("rasterizer_state"))
            {
                rapidjson::Value& rs = json["rasterizer_state"];
                
                String cull_mode = rs["cull_mode"].GetString();
                
                if(cull_mode == "FRONT")
                    desc.cull_mode = CullMode::FRONT;
                else if(cull_mode == "BACK")
                    desc.cull_mode = CullMode::BACK;
                else if(cull_mode == "FRONT_BACK")
                    desc.cull_mode = CullMode::FRONT_BACK;
                else if(cull_mode == "NONE")
                    desc.cull_mode = CullMode::NONE;
                
                String fill_mode = rs["fill_mode"].GetString();
                
                if(fill_mode == "SOLID")
                    desc.fill_mode = FillMode::SOLID;
                else if(fill_mode == "WIREFRAME")
                    desc.fill_mode = FillMode::WIREFRAME;
                
                String winding_order = rs["winding_order"].GetString();
                
                if(winding_order == "CCW")
                    desc.front_winding_ccw = true;
                else if(winding_order == "CW")
                    desc.front_winding_ccw = false;
                
                desc.multisample = rs["multisample"].GetBool();
                desc.scissor = rs["scissor"].GetBool();
            }
        }
        
        void parse_blend_state(JsonDocument& json, BlendStateCreateDesc& desc)
        {
            
        }
        
        void parse_draw_primitive(JsonDocument& json, DrawPrimitive& prim)
        {
            String primitive = json["primitive_type"].GetString();
            
            if(primitive == "TRIANGLES")
                prim = DrawPrimitive::TRIANGLES;
            else if(primitive == "TRIANGLE_STRIP")
                prim = DrawPrimitive::TRIANGLE_STRIP;
            else if(primitive == "LINES")
                prim = DrawPrimitive::LINES;
            else if(primitive == "LINE_STRIP")
                prim = DrawPrimitive::LINE_STRIP;
            else if(primitive == "POINTS")
                prim = DrawPrimitive::POINTS;
            
        }
        
        PipelineStateObject* create(String pso)
        {
            asset_common::TextLoadData* load_data = text_loader::load(pso);
            
            JsonDocument doc;
            doc.Parse(load_data->buffer);
            
            Task task;
            PipelineStateObject* pipeline_state = nullptr;
            CreatePipelineState* pso_task_data = task_data<CreatePipelineState>(task);
            pso_task_data->pso = &pipeline_state;
            task._function.Bind<&create_pipeline_state_task>();
            
            parse_depth_stencil_state(doc, pso_task_data->desc.depth_stencil_state);
            parse_rasterizer_state(doc, pso_task_data->desc.rasterizer_state);
            parse_blend_state(doc, pso_task_data->desc.blend_state);
            parse_draw_primitive(doc, pso_task_data->desc.primitive);
            
            submit_gpu_upload_task(task);
            
            if(!pipeline_state->depth_stencil_state || !pipeline_state->rasterizer_state)
            {
                T_LOG_ERROR("Failed to created Pipeline State Object!");
                delete pipeline_state;
                return nullptr;
            }
            
            return pipeline_state;
        }
    }
}
