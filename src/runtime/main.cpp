

class Scene
{
	TransformComponent    _transform;
	SkeletalMeshComponent _skeletal_mesh;
	StaticMeshComponent   _static_mesh;
	RigidBodyComponent	  _rigid_body;
	ColliderComponent	  _collider;
	CameraComponent		  _camera;
	LuaScriptComponenet	  _lua_script;
	CppScriptComponenet	  _cpp_script;

public:
	void update(double dt)
	{
		_transform.update(dt); 		// generates transform matrices and writes them out to the Simulation packet.
		_camera.update(dt);	   		// updates view, projection matrices and writes them out to the Simulation packet as Views.
		_static_mesh.update(dt);	// writes out vertex array handles, submesh offsets into Sim packet.
		_skeletal_mesh.update(dt);	// updates animations and writes out to sim packet.
		_rigid_body.update(dt);
		_lua_script.update(dt);
		_cpp_script.update(dt);
	}
}

// An Entity without a Rigid Body Component can be attached to an Entity with a Transform regardless of the fact that it contains a Rigid Body or not.
// An Entity WITH a Rigid Body Component cannot be attached to any Entity, since they do not inherit any transforms through the Transform System.