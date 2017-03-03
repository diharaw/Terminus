ScriptComponent = 
{ 
	_entity,
	_scene
}

function ScriptComponent:new(o)
    o = o or {}
    setmetatable(o, self)
    self.__index = self
	return o
end

function ScriptComponent:initialize()
	print("No overridden initialize method")
end

function ScriptComponent:update(dt)
	print("No overridden update method")
end

function ScriptComponent:shutdown()
	print("No overridden shutdown method")
end