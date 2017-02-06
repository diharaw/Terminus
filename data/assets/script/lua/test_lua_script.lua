require "assets.script.lua.script_component"

TestLuaScript = ScriptComponent:new
{
	property = 
	{
		iThing = 5,
		nNum = 1,
		fSomething = 5.3,
		bThing = false,
		bAnotherThing = true,
		listenerId
	}
}

function TestLuaScript:OnSceneLoadComplete(event)
	log_info("Scene load complete notification")
end

function TestLuaScript:initialize()
	log_info("initialize from TestLuaScript")

	v1 = Vector3:new(3, 4, 5)
	v2 = Vector3:new(1, 2, 3)
	v3 = math.cross(v1, v2)

	log_info("v3 = (" .. v3.x .. ",".. v3.y .. ",".. v3.z .. ")")

	self.property.listenerId = event_handler.register_listener(0x8bd7b76e, self.OnSceneLoadComplete, self)
end

function TestLuaScript:update(dt)
	if self.property.bAnotherThing == true then
		log_info("update from TestLuaScript")
		self.property.bAnotherThing = false
	end
end

function TestLuaScript:shutdown()
	log_info("shutdown")
end