require "component.script_component"

AIComponent = ScriptComponent:new
{
	property = 
	{
		iThing = 5,
		vPosition = Vector3.new(0,0,0),
		nNum = 1,
		fSomething = 5.3,
		bThing = false,
		bAnotherThing = true
	}
}

function AIComponent:initialize()
	print("initialize from AIComponent" .. self.entity)
	--register_event_listener(self.onEntityCreated, self);
end

function AIComponent:update(dt)
	print("update from AIComponent " .. dt)
	print("HOT RELOAD HOT RELOAD")
end

function AIComponent:shutdown()
	print("shutdown from AIComponent")
end