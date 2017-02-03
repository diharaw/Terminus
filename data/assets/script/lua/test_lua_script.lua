require "assets.script.lua.script_component"

TestLuaScript = ScriptComponent:new
{
	property = 
	{
		iThing = 5,
		nNum = 1,
		fSomething = 5.3,
		bThing = false,
		bAnotherThing = true
	}
}

function TestLuaScript:initialize()
	T_LOG_INFO("initialize from TestLuaScript")
end

function TestLuaScript:update(dt)
	if self.property.bAnotherThing == true then
		T_LOG_INFO("update from TestLuaScript")
		self.property.bAnotherThing = false
	end
end

function TestLuaScript:shutdown()
	T_LOG_INFO("shutdown")
end