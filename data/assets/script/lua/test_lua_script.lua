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
	look_mode = false,
	camera_boom_length = 15,
	character_offset = Vector3:new(0.0, 10.0, 0.0)
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

		is_dirty = false
		cam_offset = Vector3:new()

		camera_cmp = self._scene:get_camera_component(self._entity)
		transform_cmp = self._scene:get_transform_component(self._entity)
	
		if Utility.hash_equals("mouse_look_x", event:get_axis_hash()) then
			is_dirty = true
			cam_offset.y = event.delta * self.delta_time
		end

		if Utility.hash_equals("mouse_look_y", event:get_axis_hash()) then
			is_dirty = true
			cam_offset.x = event.delta * self.delta_time
		end
		
		if is_dirty then
			Camera.offset_euler(camera_cmp, cam_offset)
			Camera.set_position(camera_cmp, transform_cmp._position + self.character_offset + camera_cmp.transform._forward * -self.camera_boom_length)
		end

	end

	if Utility.hash_equals("forward", event:get_axis_hash()) then
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
	camera_cmp = self._scene:get_camera_component(self._entity)

	position_offset = Vector3:new(0.0, 0.0, 0.0)

	position_offset = camera_cmp.transform._forward * self.forward_speed * dt
	Transform.offset_position(transform_cmp, position_offset)

	position_offset = Math.cross(Vector3:new(0.0, 1.0, 0.0), camera_cmp.transform._forward) * self.sideways_speed * dt
	Transform.offset_position(transform_cmp, position_offset)

	transform_cmp._position.y = 0.0

	Camera.set_position(camera_cmp, transform_cmp._position + self.character_offset + camera_cmp.transform._forward * -self.camera_boom_length)
end

function TestLuaScript:shutdown()
	log_info("shutdown")
end