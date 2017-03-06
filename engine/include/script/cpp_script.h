#pragma once

#include <utility/serializable.h>
#include <core/macro.h>
#include <gameplay/entity.h>
#include <script/script_interface.h>

namespace terminus
{
    class Scene;
    
	class CppScript : public Serializable
	{
    public:
        Entity _entity;
        Scene* _scene;
		terminus::ScriptInterface* _script_interface;
        
	public:
		CppScript() {}
		virtual ~CppScript() {}
		virtual void setup_environment(terminus::ScriptInterface* script_interface) = 0;
		virtual void initialize() = 0;
		virtual void update(double dt) = 0;
		virtual void shutdown() = 0;
	};
}
