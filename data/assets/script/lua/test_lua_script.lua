require "assets.script.lua.script_component"

TestLuaScript = ScriptComponent:new
{
	property = 
	{
		stateListenerId,
		axisListenerId
	},
	default_player,
	test_input_map,
	delta_time = 0,
	forward_speed = 0,
	sideways_speed = 0,
	look_mode = false
}

function TestLuaScript:OnStateInput(event)
	
	if event.value == 0 then
		self.look_mode = false
		Platform.set_relative_mouse(false)
	else
		self.look_mode = true
		Platform.set_relative_mouse(true)
	end

end

function TestLuaScript:OnAxisInput(event)

	if self.look_mode then

		camera_cmp = self._scene:get_camera_component(self._entity)
	
		if Utility.hash_equals("mouse_look_x", event:get_axis_hash()) then
			Camera.offset_yaw(camera_cmp, event.delta * self.delta_time)
		end

		if Utility.hash_equals("mouse_look_y", event:get_axis_hash()) then
			Camera.offset_pitch(camera_cmp, event.delta * self.delta_time)
		end

	end

	if Utility.hash_equals("forward", event:get_axis_hash()) then
		log_info("forward value " .. event.value)
		self.forward_speed = event.value * 10
	end

	if Utility.hash_equals("sideways", event:get_axis_hash()) then
		self.sideways_speed = event.value * 10
	end

end

function TestLuaScript:initialize()

	log_info("initialize from TestLuaScript")

	self.property.stateListenerId = event_handler.register_listener(EventType.INPUT_STATE, self.OnStateInput, self)
	self.property.axisListenerId = event_handler.register_listener(EventType.INPUT_AXIS, self.OnAxisInput, self)

	self.default_player = Input.get_default_player_context()

	log_info("loading input map")

	self.test_input_map = Input.load_input_map(self.default_player, "test_input_map.json")
	Input.set_active_input_map(self.default_player, self.test_input_map)

end

function TestLuaScript:update(dt)

	self.delta_time = dt

	transform_cmp = self._scene:get_transform_component(self._entity)

	position_offset = Vector3:new(0.0, 0.0, 0.0)
	position_offset.x = self.sideways_speed * dt
	position_offset.z = self.forward_speed * dt

	Transform.offset_position(transform_cmp, position_offset)

end

function TestLuaScript:shutdown()
	log_info("shutdown")
end