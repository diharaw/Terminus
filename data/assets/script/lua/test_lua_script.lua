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
		listenerId,
		actionListenerId,
		axisListenerId
	},
	firstNew = true,
	default_player,
	test_input_map
}

function TestLuaScript:OnSceneLoadComplete(event)
	log_info("Scene load complete notification")
end

function TestLuaScript:OnActionInput(event)
	log_info("Action input recieved!")
end

function TestLuaScript:OnAxisInput(event)
	log_info("Axis input recieved!")
end

function TestLuaScript:initialize()
	log_info("initialize from TestLuaScript")
	self.property.listenerId = event_handler.register_listener(EventType.SCENE_LOAD, self.OnSceneLoadComplete, self)
	self.property.actionListenerId = event_handler.register_listener(EventType.INPUT_ACTION, self.OnActionInput, self)
	self.property.axisListenerId = event_handler.register_listener(EventType.INPUT_AXIS, self.OnAxisInput, self)

	self.default_player = Input.get_default_player_context()
	log_info("loading input map")
	self.test_input_map = Input.load_input_map(self.default_player, "test_input_map.json")
	Input.set_active_input_map(self.default_player, self.test_input_map)
end

function TestLuaScript:update(dt)
	test_cmp = self._scene:get_transform_component(self._entity)

	log_info("Lua Hot Reload!")
	new_pos = test_cmp._position
	new_pos.x = new_pos.x + dt * 20.0

	transform.set_position(test_cmp, Vector3:new(1.0, 1.0, 1.0))
	camera_cmp = self._scene:get_camera_component(self._entity)

	cam_speed = dt * 10.0

	camera.set_position(camera_cmp, Vector3:new(0.0, 0.0, 10.0))

	if self.property.bAnotherThing == true then
		log_info("update from TestLuaScript")
		log_info("My entity Id : " .. self._entity._id)
		log_info("My scene name : " .. self._scene._name)

		self.property.bAnotherThing = false
	end
end

function TestLuaScript:shutdown()
	log_info("shutdown")
end